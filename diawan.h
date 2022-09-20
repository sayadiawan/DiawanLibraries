#ifndef DIAWAN_H
#define DIAWAN_H
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>


class Parameter {
  public:
    Parameter();
    Parameter(char* name, float value);
    char* getName() const;
    float getValue() const; 
    char* _name ; 
    float _value ; 
      
};
void connectDiawan2( String link, String email, String pass, String userId, String idDevice,Parameter all_parameter[], String *name, float *offsite1,int *restart ,int *reset );
void geturlDiawanTrial(String idDevice, String *link,String *name, float *offsite1, float *koreksi1);
void connectDiawanParcial( String link, String email, String pass, String userId, String idDevice,Parameter all_parameter[], String *name, float *offsite1,int *restart ,int *reset, int jumlah_parameter);
void geturlDiawan(String idDevice, String *link,String *name, float *offsite1, float *koreksi1);
void connectDiawan( String link, String email, String pass, String userId, String idDevice,float *tempC, String *name, float *offsite1,int *restart ,int *reset );
void connectDiawanTrial( String link, String email, String pass, String userId, String idDevice,float tempC, String *name, float *offsite1,int *restart ,int *reset );
#endif