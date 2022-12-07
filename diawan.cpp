#include "diawan.h"
#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

Parameter::Parameter() {
}
Parameter::Parameter(char* name, float value, float offsite) {
    _name  = name;
    _value  = value;
     _offsite  = offsite;
}
Parameter::Parameter(char* name, float offsite) {
    _name  = name;
    _offsite  = offsite;
}

char* Parameter::getName() const
{
    return _name;
}

void Parameter::setVar(char* name, float offsite, float correction, float a, float b, float c) {
  _name  = name;
  _offsite  = offsite;
  _correction  = correction;
  _value  = NULL;
  _valueString  = "";
  _a  = a;
  _b  = b;
  _c  = c;      
}

void Parameter::setValueString(String valueString) {
  if(_a!=NULL || _b!=NULL || _c!=NULL){
    float value = (_a * (atof(strdup(((String)value).c_str())))) + (_b * (pow((atof(strdup(((String)value).c_str()))), 2))) + _c;
    _valueString  = strdup(((String)value).c_str());
  }else{
    _valueString  = strdup((valueString).c_str());
  }
}

void Parameter::setValue(float value) {

  if(_a!=NULL || _b!=NULL || _c!=NULL){
    _value = (_a * value) + (_b * (pow(value, 2))) + _c;
    _valueString  = strdup(((String)_value).c_str());
  }else{
    _value  = value;
    _valueString  = strdup(((String)value).c_str());
  }
}

void Parameter::setMin(float min) {
   _min  = min;
}

void Parameter::setA(float a) {
   _a  = a;
}

void Parameter::setB(float b) {
   _b  = b;
}

void Parameter::setC(float c) {
   _c  = c;
}

void Parameter::setMax(float max) {
   _max  = max;
}

void Parameter::setOffsite(float offsite) {
   _offsite  = offsite;
}
float Parameter::getOffsite() const
{
    return _offsite;
}
float Parameter::getCorrection() const
{
    return _correction;
}
float Parameter::getValue() const
{
    return _value;
}

float Parameter::getMax() const
{
    return _max;
}

float Parameter::getMin() const
{
    return _min;
}

float Parameter::getA() const
{
    return _a;
}

float Parameter::getB() const
{
    return _b;
}

float Parameter::getC() const
{
    return _c;
}

String Parameter::getValueString() const
{
    return String(_valueString);
}

void geturlDiawanTrial(String idDevice, String *link,String *name, float *offsite1, float *koreksi1) {
  WiFiClient client;
  HTTPClient http;
  String serverPath =  "http://diawan.io/api/get_url/" + idDevice;
  http.begin(client, serverPath.c_str());
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
    DynamicJsonDocument doc(1024);
    String input = payload;
    deserializeJson(doc, input);
    JsonObject obj = doc.as<JsonObject>();
    *link = obj["url"]["push"].as<String>();
    *offsite1 = obj["offsite"]["offsite_data1"].as<float>(); // Bisa di Edit (tambahkan data menyesuaikan jumlah parameter yang digunakan)
    *koreksi1 = obj["correction"]["correction_data1"].as<float>(); // Bisa di Edit (tambahkan data menyesuaikan jumlah parameter yang digunakan)
   *name = obj["name"].as<String>();
  }else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  delay(3000);
}

void geturlDiawan(String idDevice, String *link,String *name, Parameter **parameter) {
  WiFiClient client;
  HTTPClient http;
  String serverPath =  "http://diawan.io/api/get_url/" + idDevice;
  http.begin(client, serverPath.c_str());
  int httpResponseCode = http.GET();

  if (httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
    DynamicJsonDocument doc(3024);
    String input = payload;
    deserializeJson(doc, input);
    JsonObject obj = doc.as<JsonObject>();
    *link = obj["url"]["push"].as<String>();
    int count = obj["count"].as<int>();
    *parameter = (Parameter *) malloc(sizeof(Parameter) * (100+1));

    int i;
    for (i = 0; i < count; i++) {
      int iterasi = i+1;
      float offset = obj["offsite"]["offsite_data"+String(iterasi)].as<float>();
      float correction = obj["correction"]["correction_data"+String(iterasi)].as<float>();

      float a = obj["regression"]["data"+String(iterasi)]["a"].as<float>();
      float b = obj["regression"]["data"+String(iterasi)]["b"].as<float>();
      float c = obj["regression"]["data"+String(iterasi)]["c"].as<float>();

      

      (*parameter)[i].setVar(strdup(("data"+String(iterasi)).c_str()),offset,correction,a,b,c);
    }
    for (i = i; i < 100; i++) {
      int iterasi = i+1;
      (*parameter)[i].setVar(strdup(("data"+String(iterasi)).c_str()),NULL,NULL,NULL,NULL,NULL);
    }
        
   *name = obj["name"].as<String>();
  }else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();
  delay(3000);
}







void connectDiawan( String link, String email, String pass, String userId, String idDevice, String *name,int *restart ,int *reset, Parameter **parameter)  {
  WiFiClient client;
  HTTPClient http;
  String load = "{}";
  http.begin(client, link);
  String jsonStr = "";

       

  int count=0;
  // Serial.print((*parameter)[26].getValue());
  for (int i = 0; (*parameter)[i].getValue()!=NULL||(*parameter)[i].getValueString()!=""; i = i + 1) {
     
    count++;

  }

   http.addHeader("Content-Type", "application/json");

  String parameter_string="{";
  for (int i = 0; i<count; i = i + 1) {
    if((count-1)==i){
      if((*parameter)[i].getValue()!=NULL){
        parameter_string=parameter_string+"\"data"+(i+1)+"\":"+(*parameter)[i].getValue();
      }else{
        parameter_string=parameter_string+"\"data"+(i+1)+"\":\""+(*parameter)[i].getValueString()+"\"";
      }
    }else{
      if((*parameter)[i].getValue()!=NULL){
        parameter_string=parameter_string+"\"data"+(i+1)+"\":"+(*parameter)[i].getValue()+",";
      }else{
        parameter_string=parameter_string+"\"data"+(i+1)+"\":\""+(*parameter)[i].getValueString()+"\",";
      }
    }
  }
  parameter_string=parameter_string+"}";




  int httpResponseCode = http.POST("{\"email\":\"" + email + "\",\"password\":\"" + pass + "\", \"userId\":\"" + userId + "\",\"idDevice\":\"" + idDevice + "\",\"value\":"+parameter_string+"}");
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode != 200) {
    Serial.println("Data tidak terkirim");
  }else {
    load = http.getString();
    Serial.print(load);
    DynamicJsonDocument doc(3024);
    String input = load;
    deserializeJson(doc, input);
    JsonObject obj = doc.as<JsonObject>();
    
    // EDIT (tambahkan data menyesuaikan jumlah parameter yang digunakan)
     for (int i = 0; (*parameter)[i].getOffsite()!=NULL; i = i + 1) {
      (*parameter)[i].setOffsite(obj["result"]["offsite"]["offsite_value_data"+String(i+1)].as<float>());
      (*parameter)[i].setMin(obj["result"]["min"]["min_data"+String(i+1)].as<float>());
      (*parameter)[i].setMax(obj["result"]["max"]["max_data"+String(i+1)].as<float>());
    }
    *name = obj["result"]["name"].as<String>();

    *restart = obj["result"]["restart"].as<int>();
    *reset = obj["result"]["reset"].as<int>();
 
    http.end();
    delay(3000);
  }
}



void connectDiawanWifi( String link, String email, String pass, String userId, String idDevice,int wifi, String *name,int *restart ,int *reset, Parameter **parameter)  {
  WiFiClient client;
  HTTPClient http;
  String load = "{}";
  http.begin(client, link);
  String jsonStr = "";


   int count=0;
  // Serial.print((*parameter)[26].getValue());
  for (int i = 0; (*parameter)[i].getValue()!=NULL||(*parameter)[i].getValueString()!=""; i = i + 1) {
     
    count++;

  }

  http.addHeader("Content-Type", "application/json");

  String parameter_string="{";
   for (int i = 0; i<count; i = i + 1) {
   
    if((*parameter)[i].getValue()!=NULL){
      parameter_string=parameter_string+"\"data"+(i+1)+"\":"+(*parameter)[i].getValue()+",";
    }else{
      parameter_string=parameter_string+"\"data"+(i+1)+"\":\""+(*parameter)[i].getValueString()+"\",";
    }
    
  }
   parameter_string=parameter_string+"\"wifi\":"+wifi+"}";
   

  int httpResponseCode = http.POST("{\"email\":\"" + email + "\",\"password\":\"" + pass + "\", \"userId\":\"" + userId + "\",\"idDevice\":\"" + idDevice + "\",\"value\":"+parameter_string+"}");
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode != 200) {
    Serial.println("Data tidak terkirim");
  }else {
    load = http.getString();
    Serial.print(load);
    DynamicJsonDocument doc(1024);
    String input = load;
    deserializeJson(doc, input);
    JsonObject obj = doc.as<JsonObject>();
    
    // EDIT (tambahkan data menyesuaikan jumlah parameter yang digunakan)
    for (int i = 0; (*parameter)[i].getName()!=""; i = i + 1) {
      (*parameter)[i].setOffsite(obj["result"]["offsite"]["offsite_value_data"+String(i+1)].as<float>());
      (*parameter)[i].setMin(obj["result"]["min"]["min_data"+String(i+1)].as<float>());
      (*parameter)[i].setMax(obj["result"]["max"]["max_data"+String(i+1)].as<float>());
    }
    *name = obj["result"]["name"].as<String>();

    *restart = obj["result"]["restart"].as<int>();
    *reset = obj["result"]["reset"].as<int>();
 
    http.end();
    delay(3000);
  }
}

void connectDiawanTrial( String link, String email, String pass, String userId, String idDevice,float tempC, String *name, float *offsite1,int *restart ,int *reset ) {
  WiFiClient client;
  HTTPClient http;
  String load = "{}";
  http.begin(client, link);
  
  String jsonStr = "";

  http.addHeader("Content-Type", "application/json");

  int httpResponseCode = http.POST("{\"email\":\"" + email + "\",\"password\":\"" + pass + "\", \"userId\":\"" + userId + "\",\"idDevice\":\"" + idDevice + "\",\"value\":{\"data1\":" + tempC + "}}");
  Serial.print("HTTP Response code: ");
  Serial.println(httpResponseCode);

  if (httpResponseCode != 200) {
    Serial.println("Data tidak terkirim");
  }else {
    load = http.getString();
    Serial.print(load);
    DynamicJsonDocument doc(1024);
    String input = load;
    deserializeJson(doc, input);
    JsonObject obj = doc.as<JsonObject>();
    
    *offsite1 = obj["result"]["offsite"]["offsite_value_data1"].as<float>();
    *name = obj["result"]["name"].as<String>();
    *restart = obj["result"]["restart"].as<int>();
    *reset = obj["result"]["reset"].as<int>();
 
    http.end();
    delay(3000);
  }
 }