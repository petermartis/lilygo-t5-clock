#include "Arduino.h"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include "SimpleWeather.h"

// Icon mapping lookup table for O(1) access
struct IconMapping {
	const char* owm;
	const char* meteocons;
};

static const IconMapping ICON_MAP[] = {
	{"01d", "B"}, {"01n", "C"},
	{"02d", "H"}, {"02n", "4"},
	{"03d", "N"}, {"03n", "5"},
	{"04d", "Y"}, {"04n", "Y"},
	{"09d", "Q"}, {"09n", "7"},
	{"10d", "R"}, {"10n", "8"},
	{"11d", "O"}, {"11n", "6"},
	{"13d", "W"}, {"13n", "#"},
	{"50d", "M"}, {"50n", "M"},
	{nullptr, nullptr}
};

OpenWeather::OpenWeather(const char* Key, float lat, float longi) {
	snprintf(_url, sizeof(_url),
		"/data/3.0/onecall?lat=%.6f&lon=%.6f&appid=%s&units=metric&exclude=minutely,hourly,daily,alerts",
		lat, longi, Key);
	_errorDetail[0] = '\0';
}

WeatherError OpenWeather::updateStatus(weatherData *w) {

	const char *openweather = "api.openweathermap.org";
	const int httpsPort = 443;
	WiFiClientSecure httpsClient;
	const size_t capacity = 4096;  // OneCall API responses can be 2-4KB
	httpsClient.setInsecure();
	httpsClient.setTimeout(15000);

	// Try to connect
	int r = 0;
	while ((!httpsClient.connect(openweather, httpsPort)) && (r < 4)) {
		delay(100);
		r++;
	}
	if (r > 3) {
		snprintf(_errorDetail, sizeof(_errorDetail), "Connect failed after %d retries", r);
		return WEATHER_ERR_CONNECTION;
	}

	// Build HTTP request using fixed buffer - avoids String heap fragmentation
	char request[384];
	snprintf(request, sizeof(request),
		"GET %s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n",
		_url, openweather);
	httpsClient.print(request);

	// Read and check HTTP status line
	String statusLine = httpsClient.readStringUntil('\n');
	int httpCode = 0;
	if (statusLine.startsWith("HTTP/")) {
		int spaceIndex = statusLine.indexOf(' ');
		if (spaceIndex > 0) {
			httpCode = statusLine.substring(spaceIndex + 1, spaceIndex + 4).toInt();
		}
	}

	// Skip remaining headers
	while (httpsClient.connected()) {
		String line = httpsClient.readStringUntil('\n');
		if (line == "\r") {
			break;
		}
	}

	// Check HTTP status code
	if (httpCode != 200) {
		snprintf(_errorDetail, sizeof(_errorDetail), "HTTP %d", httpCode);
		// Read a bit of the error response for context
		if (httpsClient.connected()) {
			String errBody = httpsClient.readStringUntil('\n');
			// Truncate if too long
			if (errBody.length() > 30) {
				errBody = errBody.substring(0, 30) + "...";
			}
		}
		return WEATHER_ERR_API_ERROR;
	}

	// Read body - this allocation is necessary for JSON parsing
	String response;
	unsigned long readStart = millis();
	while (httpsClient.connected() && (millis() - readStart < 10000)) {
		if (httpsClient.available()) {
			response += httpsClient.readString();
		}
	}

	if (response.length() == 0) {
		snprintf(_errorDetail, sizeof(_errorDetail), "Empty response");
		return WEATHER_ERR_TIMEOUT;
	}

	DynamicJsonDocument doc(capacity);
	DeserializationError err = deserializeJson(doc, response);
	if (err.code() != DeserializationError::Ok) {
		snprintf(_errorDetail, sizeof(_errorDetail), "JSON: %s (len=%d)", err.c_str(), response.length());
		return WEATHER_ERR_JSON_PARSE;
	}

	// Check for API error response
	if (doc.containsKey("cod") && doc.containsKey("message")) {
		const char* msg = doc["message"] | "unknown";
		snprintf(_errorDetail, sizeof(_errorDetail), "API: %s", msg);
		return WEATHER_ERR_API_ERROR;
	}

	if (!doc.containsKey("current")) {
		snprintf(_errorDetail, sizeof(_errorDetail), "No 'current' in response");
		return WEATHER_ERR_NO_DATA;
	}

	// Parse weather data
	if (doc["current"].containsKey("weather") && doc["current"]["weather"].size() > 0) {
		const char* iconStr = doc["current"]["weather"][0]["icon"] | "";
		strncpy(w->icon, iconStr, sizeof(w->icon) - 1);
		w->icon[sizeof(w->icon) - 1] = '\0';
	}
	if (doc["current"].containsKey("temp")) {
		w->current_Temp = doc["current"]["temp"].as<float>();
	}
	if (doc["current"].containsKey("feels_like")) {
		w->feels_like = doc["current"]["feels_like"].as<float>();
	}
	if (doc["current"].containsKey("humidity")) {
		w->humidity = doc["current"]["humidity"].as<int>();
	}
	if (doc["current"].containsKey("wind_speed")) {
		w->wind_speed = doc["current"]["wind_speed"].as<float>();
	}
	if (doc["current"].containsKey("wind_deg")) {
		w->wind_direction = doc["current"]["wind_deg"].as<int>();
	}

	_errorDetail[0] = '\0';
	return WEATHER_OK;
}

// Returns static string - no heap allocation
const char* OpenWeather::getWindDirection(int deg) {
    if (deg >= 337 || deg < 23) return "N";
    if (deg < 68) return "NE";
    if (deg < 113) return "E";
    if (deg < 158) return "SE";
    if (deg < 203) return "S";
    if (deg < 248) return "SW";
    if (deg < 293) return "W";
    return "NW";
}

// O(1) lookup using table - returns static string
const char* OpenWeather::getIcon(const char* i) {
	if (!i || !*i) return "B";  // Default
	for (const IconMapping* m = ICON_MAP; m->owm != nullptr; m++) {
		if (strcmp(m->owm, i) == 0) {
			return m->meteocons;
		}
	}
	return "B";  // Default icon
}
