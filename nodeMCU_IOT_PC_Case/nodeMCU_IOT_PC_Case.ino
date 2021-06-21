#include "DHT.h" 
#include<Servo.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define SERVOPIN D0
#define DHTPIN1 D1
#define DHTPIN2 D2
#define DHTTYPE DHT11 
#define RGBLEDPIN D3
#define ONBUTTONPIN D4

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

Servo flap;

bool b = 0;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).

char auth[] = "eebbc3cdcb6f47bbaf108e6ff663f9de";

// Your WiFi credentials.
// Set password to "" for open networks.

char ssid[] = "Jalabulajangs";

char pass[] = "aaaaaaaaaa";


BLYNK_WRITE(V3) {
if(param.asInt() == 90)
b=0;
if(param.asInt() >= 180)
b=1;
flap.write(param.asInt());
}


void setup() {
flap.attach(SERVOPIN);
pinMode(DHTPIN1, INPUT);
pinMode(DHTPIN2 , INPUT);
//pinMode(ONBUTTONPIN , OUTPUT);
//pinMode(RGBLEDPIN , OUTPUT);
Blynk.begin(auth, ssid, pass);


}

void loop() {
  delay(2500);

float OUTSIDE_HUMIDITY = dht1.readHumidity();
float OUTSIDE_TEMPERATURE = dht1.readTemperature();
float INCABINET_HUMIDITY = dht2.readHumidity();
float INCABINET_TEMPERATURE = dht2.readTemperature();

if(fabs(OUTSIDE_TEMPERATURE-INCABINET_TEMPERATURE)>=9)
flap.write(180);

if(b == 1)
flap.write(180);

if(!(fabs(OUTSIDE_TEMPERATURE-INCABINET_TEMPERATURE)>=9) && b == 0)
flap.write(90);

Blynk.run();
Blynk.virtualWrite(V0, OUTSIDE_HUMIDITY);
Blynk.virtualWrite(V1, INCABINET_HUMIDITY);
Blynk.virtualWrite(V5, OUTSIDE_TEMPERATURE);
Blynk.virtualWrite(V6, INCABINET_TEMPERATURE);

}
