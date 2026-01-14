/*
	SimpleWeather.cpp - Library to get weather from Openweather
	Created by Dushyant Ahuja, September 2019.
	Released into the public domain.
*/

#ifndef SimpleWeather_h
#define SimpleWeather_h

#include "Arduino.h"
#include <WiFi.h>

// Error codes for weather API
enum WeatherError {
	WEATHER_OK = 0,
	WEATHER_ERR_CONNECTION = 1,
	WEATHER_ERR_TIMEOUT = 2,
	WEATHER_ERR_JSON_PARSE = 3,
	WEATHER_ERR_NO_DATA = 4,
	WEATHER_ERR_API_ERROR = 5
};

struct weatherData {
	char icon[5];
	int humidity;
	int wind_direction;
	float current_Temp;
	float feels_like;
	float wind_speed;
};

class OpenWeather {
	public:
		OpenWeather(const char* Key, float lat, float longi);
		WeatherError updateStatus(weatherData *w);
		const char* getWindDirection(int deg);
		const char* getIcon(const char* i);
		const char* getLastErrorDetail() { return _errorDetail; }
	private:
		char _url[256];
		char _errorDetail[64];
};

#endif
