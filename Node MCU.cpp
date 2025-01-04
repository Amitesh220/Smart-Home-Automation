#define BLYNK_TEMPLATE_ID "TMPL31iNK77ui"
#define BLYNK_TEMPLATE_NAME "IoT Dashboard"
#define BLYNK_AUTH_TOKEN "2wMwQX7TK0-Z1xbxKQiv3qZHFxYOjIxu"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h> 
#include <DHT.h>
#define WIFI_SSID "WAYNE TOWERS" 
#define WIFI_PASS "987654321"
#define DHTPIN D4
#define DHTTYPE DHT22 DHT dht(DHTPIN, DHTTYPE);
#define RELAY1_PIN D1 
#define RELAY2_PIN D2 
#define MOTION_PIN D6
#define BUZZER_PIN D7 
#define SMOKE_PIN A0 
#define LDR_PIN D5 
#define VPIN_TEMP V0
#define VPIN_HUMIDITY V1 
#define VPIN_SMOKE V2 
#define VPIN_LDR V3
#define VPIN_RELAY1 V4 
#define VPIN_RELAY2 V5 
#define VPIN_MOTION V6 float temperature = 0.0;
float humidity = 0.0;
const float TEMP_THRESHOLD = 30.0; const int SMOKE_THRESHOLD = 400;
bool motorAutoControl = true; 
void setup() 
{ Serial.begin(115200); dht.begin();
pinMode(RELAY1_PIN, OUTPUT); 
pinMode(RELAY2_PIN, OUTPUT); 
pinMode(MOTION_PIN, INPUT); 
pinMode(BUZZER_PIN, OUTPUT); 
pinMode(SMOKE_PIN, INPUT);
pinMode(LDR_PIN, INPUT); 
digitalWrite(RELAY1_PIN, LOW); 
digitalWrite(RELAY2_PIN, LOW); 
digitalWrite(BUZZER_PIN, LOW); 
WiFi.begin(WIFI_SSID, WIFI_PASS);
while (WiFi.status() != WL_CONNECTED)
{ delay(1000); Serial.println("Connecting to WiFi...");
} Serial.println("WiFi connected");
Serial.println("IP address: " + WiFi.localIP().toString());
 Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASS); }
void loop() {
if (Blynk.connected())
{ Blynk.run(); }
temperature = dht.readTemperature(); 
humidity = dht.readHumidity();
if (isnan(temperature) || isnan(humidity)) { Serial.println("Failed to read from DHT sensor!"); return; }
if (motorAutoControl && temperature > TEMP_THRESHOLD) { digitalWrite(RELAY1_PIN, HIGH); Serial.println("Temperature exceeded threshold. Motor ON.");
}
Else if (motorAutoControl) { digitalWrite(RELAY1_PIN, LOW); 
Serial.println("Temperature below threshold. Motor OFF."); }
int smokeValue = analogRead(SMOKE_PIN);
 
if (smokeValue > SMOKE_THRESHOLD) { digitalWrite(BUZZER_PIN, HIGH); 
Blynk.virtualWrite(VPIN_SMOKE, 1);
Serial.println("Smoke detected! Buzzer ON."); }
else { digitalWrite(BUZZER_PIN, LOW);
Blynk.virtualWrite(VPIN_SMOKE, 0);
 }
 int ldrValue = digitalRead(LDR_PIN);
Blynk.virtualWrite(VPIN_LDR, ldrValue);
 Serial.print("LDR: "); 
Serial.println(ldrValue ? "Bright" : "Dark");
int motionDetected = digitalRead(MOTION_PIN); 
Blynk.virtualWrite(VPIN_MOTION, motionDetected); 
Blynk.virtualWrite(VPIN_TEMP, temperature); 
Blynk.virtualWrite(VPIN_HUMIDITY, humidity); 
Serial.print("Temperature: ");
Serial.print(temperature);
 Serial.print(" *C, Humidity: "); 
Serial.print(humidity); Serial.print(" %, Smoke: ");
Serial.print(smokeValue > SMOKE_THRESHOLD ? "Detected" : "Not Detected"); 
Serial.print(", Motion: "); 
Serial.print(motionDetected ? "Detected" : "Not Detected");
Serial.print(", LDR: "); 
Serial.println(ldrValue ? "Bright" : "Dark"); 
delay(2000); }
BLYNK_WRITE(VPIN_RELAY1) { int value = param.asInt(); 
motorAutoControl = (value == 0); 
digitalWrite(RELAY1_PIN, value); 
Serial.println(value ? "Motor ON (Manual)" : "Motor OFF (Manual)"); } 
BLYNK_WRITE(VPIN_RELAY2) { int value = param.asInt(); 
digitalWrite(RELAY2_PIN, value);
Serial.println(value ? "Light ON" : "Light OFF"); }
