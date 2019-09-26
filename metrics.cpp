#include "metrics.h"
#include <Arduino.h>
#include <ESP8266HTTPClient.h>
#include <base64.h>
#include <string.h>
#include "config.h"


#define URL "/api/put"

// FINGERPRINT is defined in config.h
// HOST is defined in config.h
// TOKEN is defined in config.h

Metrics::Metrics() {
}


void Metrics::sendData(const char* name, double value) {
  
  // Prepare th JSON body
  
  String body("[");

    body += String("{") + 
        "\"metric\":\"" + name + "\"," +
        "\"value\":" + String(value) + "," +
        "\"tags\":{}" +
    "}";

  body += String("]");

  // prepare the Authorization token
  String token = String("DESC:") + TOKEN;
  String base64Token = String("basic ") + base64::encode(token, false); // passing false force the result to be on one line

  // Sending the HTTPS request
  Serial.println("\n------------------------------------------------");
  Serial.println(String("POST ") + HOST + URL);
  Serial.println(body);
  Serial.println("------------------------------------------------\n");

  HTTPClient http;
  
  int beginResult = http.begin(String("https://") + HOST + URL, FINGERPRINT);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("Authorization", base64Token);
  
  int httpCode = http.POST(body);
  String response = http.getString();

  // Grabing the result

  Serial.print("beginResult: ");
  Serial.print(beginResult);
  Serial.println();
  
  Serial.print("http: ");
  Serial.print(httpCode);
  Serial.println();
  
  Serial.print("response: ");
  Serial.println(response);
  Serial.println();
  
  // Terminate the HTTPS request; it is sent right now
  http.end();

}
