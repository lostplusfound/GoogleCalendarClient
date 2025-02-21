# GoogleCalendarClient
Library for Arduino microcontrollers to access the Google Calendar API

## How to Use
### Obtaining Credentials
Follow the steps outlined in this guide to obtain the necessary credentials to make requests to the Google Calendar API: https://lostplusfounddev.blogspot.com/2024/03/google-calendar-integrated-dashboard_28.html
### Using the library
Once you have the authentication url, API key, client ID, and client secret, create the GoogleCalendarClient object.

```cpp
GoogleCalendarClient calendar(AUTH_URI, API_KEY, CLIENT_ID, CLIENT_SECRET);
```

To make a request to the API, do the following:

1. Generate a new OAuth access token, or refresh an existing token
   ```cpp
   // Get a new access token
   bool getAccessToken();
   // Refresh an existing token
   bool refreshAccessToken();
    ```
   The [preferences](https://github.com/vshymanskyy/Preferences) library is used to store the refresh token in flash, so it persists through restarts. It is recommended to first call `refreshAccessToken();` and check the return value. If this method returns true, it means that the cached refresh token yielded a valid fresh access token, and no further action is needed. If the method returns false, then call `getAccessToken();` to generate a new access token. This method will retrieve both an access token and refresh token, and the refresh token will be cached in flash for future boots.
2. Refresh the events on the calendar
   ```cpp
   // numTries is number of times to retry request, timeMin will cut off any events before that time (in RFC 3339 format) from being sent in the response
   bool refreshEvents(int numTries, String timeMin);
   ```
   Call this method to refresh the events on the calendar. Then, access the `JSONVar events;` property of the GoogleCalendarClient object which contains a list of the user's Google Calendar events in JSON format. Consult the [Google Calendar API reference](https://developers.google.com/calendar/api/v3/reference/events/list) for the JSON response format. Once this method returns false, the access token has become invalid, and step 1 must be repeated to either refresh the token or generate a new one.

## Shortcomings
This library has very limited functionality, only supporting one feature: getting a list of the events on a user's Google Calendar. Moreover, it has a very clunky process for obtaining the credentials needed to call the Google Calendar API. Finally, it is incredibly insecure, ignoring SSL certificates and simply calling `client.setInsecure();`. 
   

