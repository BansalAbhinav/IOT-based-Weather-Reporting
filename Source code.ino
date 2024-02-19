//Defining webserver for getting Real-time Data
#define BLYNK_TEMPLATE_ID "TMPL30e0EFaX1"
#define BLYNK_TEMPLATE_NAME "DHT11"
#define BLYNK_AUTH_TOKEN "zvLIIf2MRuJl_Mfljx0l7cv1aOVOYWs2"

// libraries include in the code
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Blynk.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

Adafruit_BMP085 bmp;

float temp, pressure, Altitude, Sealevel;
char auth[] = "zvLIIf2MRuJl_Mfljx0l7cv1aOVOYWs2";  // Enter your Blynk auth token here
char ssid[] = "Jio Fiber 4G";   // Enter your WiFi SSID here
char pass[] = "!behappy!";   // Enter your WiFi password here

BlynkTimer timer;

#define DHTPIN D4          
#define DHTTYPE DHT11     

DHT dht(DHTPIN, DHTTYPE);



// Water Dectection code 

void moisture() {
  int rainSensor = analogRead(A0);
  rainSensor = map(rainSensor, 0, 1023, 0, 100);
  Blynk.virtualWrite(V0, rainSensor);
  Serial.println(rainSensor);
}


// DHT11 sensor code
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}



void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid, pass);
  Blynk.begin(auth, ssid, pass);

  dht.begin();

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  timer.setInterval(1000L, sendSensor);
  timer.setInterval(100L, moisture);

  Serial.print("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("WiFi is connected");
  Serial.println(WiFi.localIP());
}



void loop()
{
  Blynk.run();
  timer.run();

  temp = bmp.readTemperature();
  pressure = bmp.readPressure();
  Sealevel = bmp.readSealevelPressure();
  Altitude = bmp.readAltitude();

  Blynk.virtualWrite(V1, temp);
  Blynk.virtualWrite(V2, pressure);
  Blynk.virtualWrite(V4, Sealevel);
  Blynk.virtualWrite(V3, Altitude);

  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println(" *C");

  Serial.print("Pressure = ");
  Serial.print(pressure);
  Serial.println(" Pa");

  Serial.print("Altitude = ");
  Serial.print(Altitude);
  Serial.println(" meters");

  Serial.print("Pressure at sealevel (calculated) = ");
  Serial.print(Sealevel);
  Serial.println(" Pa");

  delay(2000);
}
