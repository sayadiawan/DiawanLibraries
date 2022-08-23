#ifndef DIAWAN_H
#define DIAWAN_H
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
void geturl(String idDevice, String *link,String *name, float *offsite1, float *koreksi1);
void connectDiawan( String link, String email, String pass, String userId, String idDevice,float *tempC, String *name, float *offsite1,int *restart ,int *reset );
#endif