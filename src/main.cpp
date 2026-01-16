#ifndef BOARD_HAS_PSRAM
#error "Please enable PSRAM !!!"
#endif

#include <Arduino.h>
#include "config.h"
#include <esp_task_wdt.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "epd_driver.h"
#include "NK5715b.h"
#include "NK5724b.h"
#include "NK5748b.h"
#include "NK5772b.h"
#include "Meteocons96.h"
#include <Wire.h>
#include <sys/time.h>
#include "WiFi.h"
#include "WiFiUdp.h"
#include <NTPClient.h>
#include <PubSubClient.h>
#include "SimpleWeather.h"
#include "namedays.h"

#define EPD_WIDTH 960
#define EPD_HEIGHT 540
#define H_MARGIN 20
#define V_MARGIN 20
#define MQTT_MSG_HEIGHT 25

const uint CLOCK_X = H_MARGIN;
const uint CLOCK_Y = 165;  // Moved up 10px
const uint DATE_X = EPD_WIDTH - H_MARGIN;
const uint DATE_Y1 = 95;   // Day of week
const uint DATE_Y2 = 150;  // Month (moved up 10px)
const uint DATE_Y3 = 195;  // Name day (below month)
const uint START_TIME_X = EPD_WIDTH - H_MARGIN;
const uint START_TIME_Y = EPD_HEIGHT - V_MARGIN;
// Weather section - moved up 15px from previous position
const uint WICON_X = H_MARGIN;
const uint WICON_Y = 385;
const uint CTEMP_X = 235;
const uint CTEMP_Y = 385;
const uint FTEMP_X = 235;
const uint FTEMP_Y = 285;
const uint WIND_X = EPD_WIDTH - H_MARGIN;
const uint WIND_Y = 285;
const uint HUMID_X = WIND_X;
const uint HUMID_Y = 335;
const uint WUPDATE_X = WIND_X;
const uint WUPDATE_Y = 385;
// MQTT message area - left aligned, full width available
const uint MQTT_AREA_WIDTH = EPD_WIDTH - (2 * H_MARGIN);
const uint MQTT_X = H_MARGIN;  // Left aligned
const uint MQTT_Y = EPD_HEIGHT - 35;

/**
 * WICON_AREA is used when erasing the weather icon prior to redrawing.
 * I may have messed up when generating the Meteocons font, or it may just be
 * that it's an unusual font. Either way, the clearString function wasn't
 * consistently erasing the entire previous icon. Here we'll just define a
 * large area that definitely encompasses the whole thing.
 */
const Rect_t WICON_AREA = {
	.x = H_MARGIN,
	.y = 205,  // Below date area, moved up 15px
	.width = (int32_t)(CTEMP_X - H_MARGIN - 5),
	.height = 200,  // Covers weather icon area
};

// MQTT message area - full width
const Rect_t MQTT_AREA = {
	.x = H_MARGIN - 5,  // Slight padding on left
	.y = EPD_HEIGHT - MQTT_MSG_HEIGHT - 45,  // Extra vertical padding for clearing
	.width = (int32_t)MQTT_AREA_WIDTH + 10,
	.height = MQTT_MSG_HEIGHT + 20,  // More height for complete clearing
};

bool _drawDate = false;
bool _drawWeather = false;
bool _drawWicon = false;
bool _drawTemp = false;
bool _drawFtemp = false;
bool _drawWind = false;
bool _drawHumidity = false;
bool _drawMqttMsg = false;
char _tod[10];
char _dow[20];
char _mdy[50];
char _nameDay[30];
char _wIcon[5];
char _wTemp[10];
char _wFeels[20];
char _wWind[25];
char _wHumidity[20];
char _wUpdated[20];
char _mqttMsg[128];
time_t waketime;
enum alignment { LEFT, RIGHT, CENTER };

RTC_DATA_ATTR bool firstRun = true;
RTC_DATA_ATTR int minute = -1;
RTC_DATA_ATTR int dayOfWeek = -1;
RTC_DATA_ATTR char tod[10];
RTC_DATA_ATTR char dow[20];
RTC_DATA_ATTR char mdy[50];
RTC_DATA_ATTR char nameDay[30];
RTC_DATA_ATTR char wIcon[5];
RTC_DATA_ATTR char wTemp[10];
RTC_DATA_ATTR char wFeels[20];
RTC_DATA_ATTR char wWind[25];
RTC_DATA_ATTR char wHumidity[20];
RTC_DATA_ATTR char wUpdated[20];
RTC_DATA_ATTR time_t lastNtpUpdate;
RTC_DATA_ATTR time_t lastWeatherUpdate;
RTC_DATA_ATTR time_t lastMqttUpdate;  // Track MQTT check timing
RTC_DATA_ATTR time_t lastRedraw;
RTC_DATA_ATTR char mqttMsg[128];  // Persisted MQTT message

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
GFXfont currentFont;
weatherData w;
OpenWeather weather(OWM_KEY, OWM_LAT, OWM_LON);

// MQTT client
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
bool mqttMessageReceived = false;

// MQTT callback - called when a message arrives
void mqttCallback(char* topic, byte* payload, unsigned int length) {
	// Truncate if message is too long
	unsigned int copyLen = (length < sizeof(_mqttMsg) - 1) ? length : sizeof(_mqttMsg) - 1;
	memcpy(_mqttMsg, payload, copyLen);
	_mqttMsg[copyLen] = '\0';
	mqttMessageReceived = true;
}

int setUnixtime(int32_t unixtime) {
	timeval epoch = {unixtime, 0};
	return settimeofday((const timeval*)&epoch, 0);
}

void setFont(GFXfont const &font) {
	currentFont = font;
}

void clearString(int x, int y, const char* text, alignment align) {
	int x1, y1;
	int w, h;
	int xx = x, yy = y;
	get_text_bounds(&currentFont, (char*)text, &xx, &yy, &x1, &y1, &w, &h, NULL);
	if (align == RIGHT)  x = x - w;
	if (align == CENTER) x = x - w / 2;
	// Add padding to prevent artifacts from partial refresh
	const int hPadding = 15;  // Extra horizontal padding
	const int vPadding = 5;   // Extra vertical padding
	Rect_t area = {
		.x = x - hPadding,
		.y = y - h - vPadding,
		.width = w + (2 * hPadding),
		.height = h + (2 * vPadding),
	};
	epd_clear_area(area);
}

void drawString(int x, int y, const char* text, alignment align) {
	int x1, y1;
	int w, h;
	int xx = x, yy = y;
	get_text_bounds(&currentFont, (char*)text, &xx, &yy, &x1, &y1, &w, &h, NULL);
	if (align == RIGHT)  x = x - w;
	if (align == CENTER) x = x - w / 2;
	writeln((GFXfont *)&currentFont, (char*)text, &x, &y, NULL);
}

void drawString(int x, int y, const char* text, const char* old_text, alignment align) {
	if (!firstRun) {
		clearString(x, y, old_text, align);
	}
	drawString(x, y, text, align);
}

void redrawClock() {
	setFont(NK5772B);
	drawString(CLOCK_X, CLOCK_Y, tod, LEFT);
	setFont(NK5724B);
	drawString(DATE_X, DATE_Y1, dow, RIGHT);
	setFont(NK5715B);  // Smaller font for date
	drawString(DATE_X, DATE_Y2, mdy, RIGHT);
	drawString(DATE_X, DATE_Y3, nameDay, RIGHT);  // Name day
}

void drawClock() {
	setFont(NK5772B);
	drawString(CLOCK_X, CLOCK_Y, _tod, tod, LEFT);
	if (_drawDate) {
		setFont(NK5724B);
		drawString(DATE_X, DATE_Y1, _dow, dow, RIGHT);
		setFont(NK5715B);  // Smaller font for date
		drawString(DATE_X, DATE_Y2, _mdy, mdy, RIGHT);
		drawString(DATE_X, DATE_Y3, _nameDay, nameDay, RIGHT);  // Name day
	}
}

void getClock() {
	struct tm now;
	getLocalTime(&now);
	strftime(_tod, 10, "%H:%M", &now);
	if (dayOfWeek != now.tm_wday) {
		_drawDate = true;
		dayOfWeek = now.tm_wday;
		strftime(_dow, 20, "%A", &now);
		strftime(_mdy, 50, "%B %d", &now);  // Full month name, no year
		// Get name day for today
		const char* nd = getNameDay(now.tm_mon + 1, now.tm_mday);
		strncpy(_nameDay, nd, sizeof(_nameDay) - 1);
		_nameDay[sizeof(_nameDay) - 1] = '\0';
	}
}

void setClock() {
	strcpy(tod, _tod);
	if (_drawDate) {
		strcpy(dow, _dow);
		strcpy(mdy, _mdy);
		strcpy(nameDay, _nameDay);
	}
}

// Status/Message display functions (used for MQTT messages and error/warning display)
void setStatusMsg(const char* msg) {
	strncpy(_mqttMsg, msg, sizeof(_mqttMsg) - 1);
	_mqttMsg[sizeof(_mqttMsg) - 1] = '\0';
	if (strcmp(mqttMsg, _mqttMsg) != 0) {
		_drawMqttMsg = true;
	}
}

void setStatusMsgWithTime(const char* prefix) {
	struct tm now;
	getLocalTime(&now);
	char timeStr[10];
	strftime(timeStr, sizeof(timeStr), "%H:%M", &now);
	snprintf(_mqttMsg, sizeof(_mqttMsg), "%s [%s]", prefix, timeStr);
	if (strcmp(mqttMsg, _mqttMsg) != 0) {
		_drawMqttMsg = true;
	}
}

void clearStatusMsg() {
	_mqttMsg[0] = '\0';
	if (mqttMsg[0] != '\0') {
		_drawMqttMsg = true;
	}
}

// Choose font size based on message length
void setStatusFont(const char* msg) {
	size_t len = strlen(msg);
	if (len > 60) {
		setFont(NK5715B);  // Smallest - for long messages
	} else if (len > 35) {
		setFont(NK5715B);  // Small
	} else {
		setFont(NK5724B);  // Medium - for short messages
	}
}

void redrawStatusMsg() {
	if (mqttMsg[0] != '\0') {
		setStatusFont(mqttMsg);
		drawString(MQTT_X, MQTT_Y, mqttMsg, LEFT);
	}
}

void drawStatusMsg() {
	epd_clear_area(MQTT_AREA);
	if (_mqttMsg[0] != '\0') {
		setStatusFont(_mqttMsg);
		drawString(MQTT_X, MQTT_Y, _mqttMsg, LEFT);
	}
}

void saveStatusMsg() {
	if (_drawMqttMsg) {
		strncpy(mqttMsg, _mqttMsg, sizeof(mqttMsg) - 1);
		mqttMsg[sizeof(mqttMsg) - 1] = '\0';
	}
}

#define WIFI_TIMEOUT_MS 10000  // Reduced from 15s

bool enableWifi() {
    // Use persistent mode for faster reconnection (remembers AP)
    WiFi.persistent(true);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    unsigned long startAttempt = millis();
    while (WiFi.status() != WL_CONNECTED) {
        if (millis() - startAttempt >= WIFI_TIMEOUT_MS) {
            WiFi.disconnect(true);
            WiFi.mode(WIFI_OFF);
            return false;
        }
        delay(100);  // Faster polling
    }
    return true;
}

void disableWifi() {
    WiFi.disconnect(true);  // true = turn off WiFi station mode
    WiFi.mode(WIFI_OFF);
}

// Check MQTT for new messages - requires WiFi to be connected
#ifdef MQTT_SERVER
void checkMqtt(bool wifiAlreadyEnabled = false) {
	if (!wifiAlreadyEnabled) {
		if (!enableWifi()) {
			setStatusMsgWithTime("! WiFi timeout (MQTT)");
			return;
		}
	}

	mqttClient.setServer(MQTT_SERVER, MQTT_PORT);
	mqttClient.setCallback(mqttCallback);

	// Try to connect with shorter timeout (3s instead of 5s)
	unsigned long startAttempt = millis();
	while (!mqttClient.connected() && (millis() - startAttempt < 3000)) {
		#ifdef MQTT_USER
		mqttClient.connect("EPD-Clock", MQTT_USER, MQTT_PASS);
		#else
		mqttClient.connect("EPD-Clock");
		#endif
		if (!mqttClient.connected()) {
			delay(100);  // Faster retry
		}
	}

	if (mqttClient.connected()) {
		mqttClient.subscribe(MQTT_TOPIC);

		// Reduced wait time for retained messages (1s instead of 2s)
		// Retained messages arrive almost immediately
		unsigned long waitStart = millis();
		while (millis() - waitStart < 1000) {
			mqttClient.loop();
			if (mqttMessageReceived) {
				break;
			}
			delay(20);  // Faster polling
		}

		mqttClient.disconnect();
	} else {
		// MQTT broker unreachable
		setStatusMsgWithTime("! MQTT broker unreachable");
	}

	// Check if MQTT message received and changed
	if (mqttMessageReceived && strcmp(mqttMsg, _mqttMsg) != 0) {
		_drawMqttMsg = true;
	}
	mqttMessageReceived = false;

	time(&lastMqttUpdate);  // Track last MQTT check time

	if (!wifiAlreadyEnabled) {
		disableWifi();
	}
}
#endif

bool ntpUpdate(bool wifiAlreadyEnabled = false) {

	if (!wifiAlreadyEnabled) {
		if (!enableWifi()) {
			setStatusMsgWithTime("! WiFi timeout (NTP)");
			time(&lastNtpUpdate);
			return false;
		}
	}
	timeClient.begin();
	bool updated = timeClient.update();
	timeClient.end();
	if (!wifiAlreadyEnabled) {
		disableWifi();
	}

	if (updated) setUnixtime(timeClient.getEpochTime());

	time(&lastNtpUpdate);
	return updated;
}

void redrawWeather() {
	setFont(Meteocons96);
	drawString(WICON_X, WICON_Y, wIcon, LEFT);
	setFont(NK5748B);
	drawString(CTEMP_X, CTEMP_Y, wTemp, LEFT);
	setFont(NK5715B);
	drawString(FTEMP_X, FTEMP_Y, wFeels, LEFT);
	drawString(WIND_X, WIND_Y, wWind, RIGHT);
	drawString(HUMID_X, HUMID_Y, wHumidity, RIGHT);
	drawString(WUPDATE_X, WUPDATE_Y, wUpdated, RIGHT);
}

void drawWeather() {

	setFont(Meteocons96);
	if (_drawWicon) {
		epd_clear_area(WICON_AREA);
		drawString(WICON_X, WICON_Y, wIcon, LEFT);
	}

	setFont(NK5748B);

	if (_drawTemp) {
		drawString(CTEMP_X, CTEMP_Y, _wTemp, wTemp, LEFT);
	}

	setFont(NK5715B);

	if (_drawFtemp) {
		drawString(FTEMP_X, FTEMP_Y, _wFeels, wFeels, LEFT);
	}

	if (_drawWind) {
		drawString(WIND_X, WIND_Y, _wWind, wWind, RIGHT);
	}

	if (_drawHumidity) {
		drawString(HUMID_X, HUMID_Y, _wHumidity, wHumidity, RIGHT);
	}

	drawString(WUPDATE_X, WUPDATE_Y, _wUpdated, wUpdated, RIGHT);

}

void getWeather(bool wifiAlreadyEnabled = false) {

	if (!wifiAlreadyEnabled) {
		if (!enableWifi()) {
			setStatusMsgWithTime("! WiFi timeout (weather)");
			time(&lastWeatherUpdate);
			return;
		}
	}
	WeatherError err = weather.updateStatus(&w);
	if (!wifiAlreadyEnabled) {
		disableWifi();
	}

	time(&lastWeatherUpdate);
	struct tm now;
	getLocalTime(&now);

	if (err == WEATHER_OK) {

		_drawWeather = true;

		sprintf(_wIcon, "%s", weather.getIcon(w.icon));
		if (strcmp(wIcon, _wIcon) != 0) _drawWicon = true;

		sprintf(_wTemp, "%.1fc", w.current_Temp);
		if (strcmp(wTemp, _wTemp) != 0) _drawTemp = true;

		sprintf(_wFeels, "Feels like %.1fc", w.feels_like);
		if (strcmp(wFeels, _wFeels) != 0) _drawFtemp = true;

		int ws = w.wind_speed * 3.6;
		const char* wd = weather.getWindDirection(w.wind_direction);
		sprintf(_wWind, "Wind: %d km/h %s", ws, wd);
		if (strcmp(wWind, _wWind) != 0) _drawWind = true;

		sprintf(_wHumidity, "Humidity: %d%%", w.humidity);
		if (strcmp(wHumidity, _wHumidity) != 0) _drawHumidity = true;

		strftime(_wUpdated, 20, "Updated: %H:%M", &now);

	} else {
		// Show specific error with details
		char errMsg[128];
		const char* errDetail = weather.getLastErrorDetail();
		switch (err) {
			case WEATHER_ERR_CONNECTION:
				snprintf(errMsg, sizeof(errMsg), "! Weather: connection failed");
				break;
			case WEATHER_ERR_TIMEOUT:
				snprintf(errMsg, sizeof(errMsg), "! Weather: timeout");
				break;
			case WEATHER_ERR_JSON_PARSE:
				snprintf(errMsg, sizeof(errMsg), "! Weather: %s", errDetail);
				break;
			case WEATHER_ERR_NO_DATA:
				snprintf(errMsg, sizeof(errMsg), "! Weather: %s", errDetail);
				break;
			case WEATHER_ERR_API_ERROR:
				snprintf(errMsg, sizeof(errMsg), "! Weather: %s", errDetail);
				break;
			default:
				snprintf(errMsg, sizeof(errMsg), "! Weather: unknown error");
				break;
		}
		setStatusMsg(errMsg);
		strftime(_wUpdated, 20, "! Updated: %H:%M", &now);
	}

}

void setWeather() {
	if (_drawWicon) strcpy(wIcon, _wIcon);
	if (_drawTemp) strcpy(wTemp, _wTemp);
	if (_drawFtemp) strcpy(wFeels, _wFeels);
	if (_drawWind) strcpy(wWind, _wWind);
	if (_drawHumidity) strcpy(wHumidity, _wHumidity);
	strcpy(wUpdated, _wUpdated);
}

void redraw() {
	epd_init();
	epd_poweron();
	epd_clear();
	redrawClock();
	redrawWeather();
	redrawStatusMsg();
	epd_poweroff_all();
	time(&lastRedraw);
}

void partialRedraw() {
	epd_init();
	epd_poweron();
	drawClock();
	if (_drawWeather) drawWeather();
	if (_drawMqttMsg) drawStatusMsg();
	epd_poweroff_all();
}

void setup() {

	setenv("TZ", TZ_INFO, 1);
	tzset(); // Assign the local timezone from setenv
	disableCore0WDT(); // Network requests may block long enough to trigger watchdog

	if (firstRun) {
		// Consolidate WiFi operations to avoid redundant enable/disable cycles
		if (enableWifi()) {
			if (!ntpUpdate(true)) {
				setStatusMsgWithTime("! NTP sync failed");
			}
			getWeather(true); // WiFi already enabled
			#ifdef MQTT_SERVER
			checkMqtt(true);  // WiFi already enabled
			#endif
			disableWifi();
		} else {
			// WiFi failed - still try to continue with default time
			setStatusMsgWithTime("! WiFi connection timeout");
			time(&lastNtpUpdate);
			time(&lastWeatherUpdate);
		}
		time(&waketime);
		getClock();
		setClock();
		setWeather();
		saveStatusMsg();
		redraw();
		firstRun = false;
	} else {
		time(&waketime);
		bool r = waketime - lastRedraw >= REDRAW_INTERVAL;

		// Consolidate WiFi operations when multiple updates are needed
		bool needNtp = (waketime - lastNtpUpdate >= NTP_INTERVAL);
		bool needWeather = (waketime - lastWeatherUpdate >= WEATHER_INTERVAL);
		#ifdef MQTT_SERVER
		#ifndef MQTT_INTERVAL
		#define MQTT_INTERVAL 60  // Default: check every minute
		#endif
		bool needMqtt = (waketime - lastMqttUpdate >= MQTT_INTERVAL);
		#else
		bool needMqtt = false;
		#endif

		// Only enable WiFi if we actually need network operations
		if (needNtp || needWeather || needMqtt) {
			if (enableWifi()) {
				if (needNtp) {
					if (!ntpUpdate(true)) {
						setStatusMsgWithTime("! NTP sync failed");
					}
					time(&waketime);
				}
				if (needWeather) getWeather(true);
				#ifdef MQTT_SERVER
				if (needMqtt) checkMqtt(true);
				#endif
				disableWifi();
			} else {
				setStatusMsgWithTime("! WiFi connection timeout");
			}
		}

		getClock();

		// Only power on EPD if there's something to update
		bool needsDisplayUpdate = (strcmp(tod, _tod) != 0) || _drawDate || _drawWeather || _drawMqttMsg;

		if (r) {
			// Full redraw needed
			setClock();
			if (_drawWeather) setWeather();
			if (_drawMqttMsg) saveStatusMsg();
			redraw();
		} else if (needsDisplayUpdate) {
			// Partial update needed
			partialRedraw();
			setClock();
			if (_drawWeather) setWeather();
			if (_drawMqttMsg) saveStatusMsg();
		} else {
			// Nothing to update - skip EPD entirely (saves power!)
			setClock();
		}
	}

	int nextRun = (60 - (waketime % 60));
	if (nextRun < 5) nextRun += 5;

	esp_sleep_enable_timer_wakeup(nextRun  * 1000000);
	esp_deep_sleep_start();

}

void loop() {

}
