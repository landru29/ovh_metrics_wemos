#include "wifi.h"
#include <ESP8266WiFi.h>
#include <string.h>

#define TIMEOUT 240

#ifdef ESP8266
extern "C" {
  #include "user_interface.h"
}
#endif

WifiConnect::WifiConnect(const char* hostname) {
  WiFi.hostname(hostname);
  Serial.println("Try to connect");
}

bool WifiConnect::startWPSPBC() {
  Serial.printf("\nWPS config start\n");

  // WPS works in STA (Station mode) only -> not working in WIFI_AP_STA !!!
  Serial.printf("Trying to connect to %s with saved config ...", WiFi.SSID().c_str());
  WiFi.mode(WIFI_STA);
  WiFi.begin(WiFi.SSID().c_str(), WiFi.psk().c_str());
  int timeout = 0;
  while ((WiFi.status() == WL_DISCONNECTED) && (timeout++ <TIMEOUT)) {
    delay(500);
    Serial.print(".");
    
  }

  Serial.print("|");

  if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Fail");
      Serial.println("Press WPS button on your router");
      Serial.print("Trying to connect with WPS ...");

      bool wpsSuccess = WiFi.beginWPSConfig();
      if(wpsSuccess) {
          // Well this means not always success :-/ in case of a timeout we have an empty ssid
          String newSSID = WiFi.SSID();
          if(newSSID.length() > 0) {
            // WPSConfig has already connected in STA mode successfully to the new station.
            Serial.printf("WPS finished. Connected successfull to SSID '%s'\n", newSSID.c_str());
            // save to config and use next time or just use - WiFi.begin(WiFi.SSID().c_str(),WiFi.psk().c_str());
          } else {
            wpsSuccess = false;
          }
      }
      if (wpsSuccess) {
        
        Serial.println("SUCCESS");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
      }
      return wpsSuccess;
  }

  if (timeout >= TIMEOUT) {
    Serial.printf("Wifi error\n");
    
    return false;
  } else {
    Serial.printf("Done\n");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    
    return true;
  }
}
