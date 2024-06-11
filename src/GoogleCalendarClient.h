#ifndef GoogleCalendarClient_H
#define GoogleCalendarClient_H

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Arduino_JSON.h>
#include <WiFiClientSecureBearSSL.h>
#include <Preferences.h>

class GoogleCalendarClient {
	public:
		GoogleCalendarClient(String &authURI, String& apiKey, String& clientID, String& clientSecret);
		JSONVar events;
		bool refreshEvents(int numTries, String timeMin);
		bool getAccessToken();
		bool refreshAccessToken();
	private:
		HTTPClient http;
		WiFiClientSecure client;
		Preferences prefs;
		String authURI;
		String apiKey;
		String clientID;
		String clientSecret;
		const String requestURI = "https://www.googleapis.com/calendar/v3/calendars/primary/events?key=";
		const String refreshURI = "https://oauth2.googleapis.com/token";
		String accessToken;
		String refreshToken;
		String getRequest(String timeMin);
};

#endif
