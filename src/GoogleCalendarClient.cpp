#include "Arduino.h"
#include "GoogleCalendarClient.h"

GoogleCalendarClient::GoogleCalendarClient(String &authURI, String& apiKey, String& clientID, String& clientSecret) {
    this->authURI = authURI;
    this->apiKey = apiKey;
    this->clientID = clientID;
    this->clientSecret = clientSecret;
    this->refreshToken = prefs.getString("refresh_token");
    refreshAccessToken();
	prefs.begin("calendar");
}
bool GoogleCalendarClient::refreshEvents(int numTries, String timeMin="1969-12-31T16:00:00-08:00") {
    String request = "";
    int tries = 0;
    while(request == "" && tries++ < numTries) {
		request = getRequest(timeMin);
		delay(10);
    }
    if(request == "") return false;
    events = JSON.parse(request);
    return true;
}
bool GoogleCalendarClient::getAccessToken() {
	client.setInsecure();
	http.begin(client, authURI);
	int code = http.GET();
    if(code > 0) {
		JSONVar token = JSON.parse(http.getString());
		if(token.hasOwnProperty("access_token") && token.hasOwnProperty("refresh_token")) {
			accessToken = String(token["access_token"]);
			refreshToken = String(token["refresh_token"]);
			prefs.putString("refresh_token", refreshToken);
			return true;
		} 
    }
	accessToken = "";
    refreshToken = "";
    return false;
}
bool GoogleCalendarClient::refreshAccessToken() {
    if(refreshToken == "") refreshToken = prefs.getString("refresh_token");
    client.setInsecure();
    http.begin(client, refreshURI);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int code = http.POST("client_id=" + clientID + "&client_secret=" + clientSecret + "&grant_type=refresh_token&refresh_token=" + refreshToken);
    if(code > 0) {
		JSONVar token = JSON.parse(http.getString());
        if(token.hasOwnProperty("access_token")) {
			accessToken = String(token["access_token"]);
			return true;
        } 
    }
    accessToken = "";
    return false;
}
String GoogleCalendarClient::getRequest(String timeMin) {
    client.setInsecure();
    http.begin(client, requestURI + apiKey + "&timeMin=" + timeMin + "&access_token=" + accessToken);
    int code = http.GET();
    if(code == 200) {
		return http.getString();
    } 
    return "";
}

