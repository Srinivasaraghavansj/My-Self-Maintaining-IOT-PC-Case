
//People Counting IR Sensor
#define cin D0
#define cout D1
int count = 100;
float countratio = 0;


//Greecooluit Exhaust fan Control
#define exhaustcontrol D4
bool exhauststate = 0;

//Oxygen Exhaust fan Control
#define oxygencontrol D5
int timemillis = 0;
bool oxystate = 0;

//DHT Sensor
#include "DHT.h" 
#define DHTPIN D6
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

//LM32 Temp Sensor
#define intemppin A0

//UBIDOTS
#include "UbidotsMicroESP8266.h"
#define TOKEN  "A1E-IL1Ju5DHTj6qTBETNhgUpF2qa0z07a"  
#define WIFISSID "Jalabulajangs"
#define PASSWORD "aaaaaaaaa"
Ubidots client(TOKEN);
unsigned long lastMillis = 0;

void setup(){
    Serial.begin(115200);
    pinMode(intemppin , INPUT);
    pinMode(cin , INPUT);
    pinMode(cout , INPUT);
    pinMode(exhaustcontrol , OUTPUT);
    pinMode(oxygencontrol , OUTPUT);
    dht.begin();
    delay(10);
    client.wifiConnection(WIFISSID, PASSWORD);
    digitalWrite(exhaustcontrol, HIGH);
}
void loop(){
  delay(500);
    if(digitalRead(cin) == HIGH)
    {
      count++;
      delay(1500);
    }
    
    if(digitalRead(cout) == HIGH)
    {
      if(count>=0){
      count--;
      delay(1500);}
    }

     countratio = 1200000/count;

     
    if (millis()-timemillis > countratio)
              {
                digitalWrite(oxygencontrol,LOW);
                timemillis = millis();
                oxystate = 1; 
              }
              
              if (millis()-timemillis > countratio/2 && oxystate == 1)
              {
                digitalWrite(oxygencontrol,HIGH);
                timemillis = millis();
                oxystate = 0;
              }

     client.add("People Count",count);
     client.add("Oxygen Pump" , oxystate);
     client.sendAll(true);
     
    if (millis() - lastMillis > 10000) { 

      float humidout = dht.readHumidity();
      float tempout = dht.readTemperature();
      float tempin = (float)analogRead(intemppin)/1023*3.3*0.864*100;
      float tempdiff = tempin-tempout;
      tempdiff = fabs(tempdiff);
      
            if(fabs(tempdiff) >= 8 && humidout <= 85)
            {
              digitalWrite(exhaustcontrol, LOW);
              exhauststate = 1;
             }
             else
             {
             digitalWrite(exhaustcontrol, HIGH);
             exhauststate = 0;
             }

              
             
             
            lastMillis = millis();
            
            client.add("Outside Humidity", humidout);
            client.add("Outside Temperature", tempout);
            client.add("Exhaust Pump" , exhauststate);
            client.sendAll(true);
            Serial.println("\nOutside Temperature: " + (String)tempout + "\nInside Temperature: " + (String)tempin + "\nTemperature Difference: " + (String)tempdiff + "\nHumidity: " + (String)humidout + "\nPeople Count: " + (String)count + "\nOxygen pump: " + (String)oxystate + "\nGreecooluit pump: " + (String)exhauststate);
            
            }
   
}
