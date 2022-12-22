#ifndef DIAWAN_H
#define DIAWAN_H
#include <Arduino.h>


#ifdef ESP32
#include <WiFi.h>
#include <HTTPClient.h>
#endif
#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#endif
#include <WiFiClient.h>
#include <ArduinoJson.h>




class Parameter {
  public:
    Parameter();
    Parameter(char* name, float value, float offsite);
    Parameter(char* name, float offsite);
    
    char* getName() ;

    float getValue() ; 
    String getValueString() ; 
    
    float getMin() ;
    float getA() ;
    float getB() ;
    float getC() ;
    float getMax() ; 
    float getOffsite() ; 
    float getCorrection() ; 
    
    void setVar(char* name, float offsite, float correction, float a, float b, float c);
    void setValue(float value);
    void setValueString(String valueString);
    void setOffsite(float offsite);
    void setMin(float min);
    void setA(float a);
    void setB(float b);
    void setC(float c);
    void setMax(float max);
    char* _name ; 
    float _value ;
    char* _valueString ;
    float _offsite ; 
    float _min ; 
    float _max ;
    float _a ;
    float _b ;
    float _c ; 
    float _correction ; 
      
};


void geturlDiawanTrial(String idDevice, String *link,String *name, float *offsite1, float *koreksi1);
void geturlDiawan(String idDevice, String *link,String *name, Parameter **parameter);
void geturlDiawan(String idDevice, String *link,String *name, Parameter **parameter,String *timestamp);
void geturlDiawan(String idDevice, String *link,String *name, Parameter **parameter,int sizeParameter,String *timestamp);
void geturlDiawan(String idDevice, String *link,String *name, Parameter **parameter,String *timestamp,int *interval);
void connectDiawanWifi( String link, String email, String pass, String userId, String idDevice,int wifi, String *name,int *restart ,int *reset, Parameter **parameter);
void connectDiawan( String link, String email, String pass, String userId, String idDevice, String *name,int *restart ,int *reset, Parameter **parameter);
void connectDiawan( String link, String email, String pass, String userId, String idDevice, String *name,int *restart ,int *reset, Parameter **parameter,int sizeParameter,String *timestamp);
void connectDiawan( String link, String email, String pass, String userId, String idDevice, String *name,int *restart ,int *reset, Parameter **parameter,String *timestamp);
void connectDiawanTrial( String link, String email, String pass, String userId, String idDevice,float tempC, String *name, float *offsite1,int *restart ,int *reset );
#endif