#include "diawan.h"
#define ARRAY_SIZE(array) ((sizeof(array))/(sizeof(array[0])))

Parameter::Parameter() {
}
Parameter::Parameter(char* name, float value) {
    _name  = name;
    _value  = value;
}


char* Parameter::getName() const
{
    return _name;
}

float Parameter::getValue() const
{
    return _value;
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

void geturlDiawan(String idDevice, String *link,String *name, float *offsite1, float *koreksi1) {
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




void connectDiawanParcial( String link, String email, String pass, String userId, String idDevice,Parameter all_parameter[], String *name, float *offsite1,int *restart ,int *reset, int jumlah_parameter) {
  WiFiClient client;
  HTTPClient http;
  String load = "{}";
  http.begin(client, link);
  String jsonStr = "";

  int array[]={1,2,3,4,4};
  Serial.print("size= ");
  // if(all_parameter[7].getName()[0])
  // {
  //   Serial.println(sizeof(all_parameter[7].getName()));
  //   Serial.println(sizeof(all_parameter[2].getName()));
  //   /* string isn't empty! */
  // }
  Serial.println(sizeof(all_parameter));
   Serial.println(ARRAY_SIZE(all_parameter),DEC);
   Serial.println(sizeof(all_parameter[1]));
  for (byte i = 0; i < jumlah_parameter; i = i + 1) {
    Serial.print("name= ");
    Serial.println(all_parameter[i].getValue());
  }

  http.addHeader("Content-Type", "application/json");
  //EDIT (tambahkan data menyesuaikan jumlah parameter yang digunakan)
  int httpResponseCode = http.POST("{\"email\":\"" + email + "\",\"password\":\"" + pass + "\", \"userId\":\"" + userId + "\",\"idDevice\":\"" + idDevice + "\",\"value\":{\"data1\":1.0}}");
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
    *offsite1 = obj["result"]["offsite"]["offsite_value_data1"].as<float>();
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
  //EDIT (tambahkan data menyesuaikan jumlah parameter yang digunakan)
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
    
    // EDIT (tambahkan data menyesuaikan jumlah parameter yang digunakan)
    *offsite1 = obj["result"]["offsite"]["offsite_value_data1"].as<float>();
    *name = obj["result"]["name"].as<String>();
    *restart = obj["result"]["restart"].as<int>();
    *reset = obj["result"]["reset"].as<int>();
 
    http.end();
    delay(3000);
  }
 }