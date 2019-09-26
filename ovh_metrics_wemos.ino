#include <Arduino.h>
#include "wifi.h"
#include "metrics.h"
// #include <variants/generic/common.h> // definition of A0 pin

WifiConnect* myWifi;
Metrics* metricClient;

void setup() {
  
  Serial.begin(9600);
  Serial.println("IOT Metrics on OVH plateform");

  myWifi = new WifiConnect("ovh_metrics");
  metricClient = new Metrics();

  myWifi->startWPSPBC();
}

void loop() {
  double metricValue = 42;
  const char metricName[] = "universe";
  metricClient->sendData(metricName, metricValue);

  // int sensorValue = 0;
  // sensorValue = analogRead(A0);
  // double physicalValue = (double)sensorValue / 10;
  // metricClient->sendData("A0", physicalValue);

  delay(10000);
}
