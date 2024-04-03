


#define BLYNK_TEMPLATE_ID "TMPL2nyHP2fBY"
#define BLYNK_TEMPLATE_NAME "Air Quality"
#define BLYNK_AUTH_TOKEN "fA0bPZb_dSpoy0p0EjoJE93RZjj7YlbT"


#define BLYNK_TEMPLATE_ID "TMPL2AJvaw4q5"
#define BLYNK_TEMPLATE_NAME "contral LED"
#define BLYNK_AUTH_TOKEN "E3bfxcvzSVXdAemNAUI8c18VfK5KWvEB"


#define BLYNK_PRINT Serial


#include <MQ135.h>

#include <WiFi.h> 
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <ThingSpeak.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>


char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "WE_149E14";  // type your wifi name
char pass[] = "20623862761";  // type your wifi password





String apiKey = "RKE5WGDFRIQ8BGDX"; 
unsigned long myChannelNumber = 2442490;
const char* server = "api.thingspeak.com";

BlynkTimer timer;
WiFiClient client;
float sensorValue;
int air_quality;


#define DHTPIN 4 //Connect Out pin to D4 in NODE MCU
#define DHTTYPE DHT11  
#define AO_PIN 36 
#define MQ2pin 2


const int soilSensorPin = 32;
const int lightSensorPin = 33;


const int LED_PIN = 13;
const int LED_PINN = 12;
const int LED_PINNN = 27;
const int LED_PINNNN = 14;
const int sensorPin= 0;

DHT dht(DHTPIN, DHTTYPE);








//Get the button values
BLYNK_WRITE(V0) {
  bool value1 = param.asInt();
  // Check these values and turn the relay1 ON and OFF
  if (value1 == 0) {
    digitalWrite(LED_PIN, LOW);
  } else {
    digitalWrite(LED_PIN, HIGH);
  }
}

//Get the button values
BLYNK_WRITE(V1) {
  bool value2 = param.asInt();
  // Check these values and turn the relay2 ON and OFF
  if (value2 == 0) {
    digitalWrite(LED_PINN, LOW);
  } else {
    digitalWrite(LED_PINN, HIGH);
  }
}
//Get the button values
BLYNK_WRITE(V2) {
  bool value3 = param.asInt();
  // Check these values and turn the relay2 ON and OFF
  if (value3 == 0) {
    digitalWrite(LED_PINNN, LOW);
  } else {
    digitalWrite(LED_PINNN, HIGH);
  }
}
//Get the button values
BLYNK_WRITE(V3) {
  bool value4 = param.asInt();
  // Check these values and turn the relay2 ON and OFF
  if (value4 == 0) {
    digitalWrite(LED_PINNNN, LOW);
  } else {
    digitalWrite(LED_PINNNN, HIGH);
  }
}



void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); 


  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
    Blynk.virtualWrite(V5, t);
    Blynk.virtualWrite(V6, h);

   
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("°C");
    Serial.print("   &&&  Humidity : ");
    Serial.print(h);
    Serial.print("% .");
    Serial.println(",");


 pinMode(LED_PIN , OUTPUT);
 pinMode(LED_PINN , OUTPUT);
 pinMode(LED_PINNN , OUTPUT);
 pinMode(LED_PINNNN , OUTPUT);
 pinMode(MQ2pin ,INPUT );
 

 if(t> 40){
     
        if(t > 40){
    Blynk.logEvent("event_temperature_alert");
    digitalWrite(LED_PINNN , HIGH);
    Serial.println("alert : ");
    Serial.print("Temperature above 20 degree  ,  ");
  }
   else
         digitalWrite(LED_PINNN , LOW);


 }

  
if(h>80){
  
     if(h> 80){
    Blynk.logEvent("event_humidity_alert");
    digitalWrite(LED_PINN  , HIGH);
    Serial.print("Humidity above 40 degree   ,      ");
  }
   else
         digitalWrite(LED_PINN , LOW);



}



    int sensorValue    = analogRead(MQ2pin);    
    int gasValue = analogRead(AO_PIN);
 

       Blynk.virtualWrite(V7, gasValue);
      
       Serial.print("MQ2 sensor AO value: ");
       Serial.print(gasValue);
       Serial.print("   &   ");

       MQ135 gasSensor = MQ135(A0);
       air_quality = gasSensor.getPPM();
         Serial.print(" Air Quality PPM: ");
         Serial.print(air_quality);
         Serial.print(" PPM      ,");

         if(gasValue >= 2000){

             
                              if(gasValue >= 2000)  {
                                   Blynk.logEvent("event_gas_alert");
                                   digitalWrite(LED_PIN , HIGH);}
                              else
                                    digitalWrite(LED_PIN , LOW);
   
                                  delay(2000);

         }
       

         float soilMoisture = analogRead(soilSensorPin);
  Blynk.virtualWrite(V8, soilMoisture);
  Serial.print("Soil moisture: ");
  Serial.println(soilMoisture);

 

  //             if(soilMoisture  >= 4095){
  //   Blynk.logEvent("event_humiditysoil_alert");
  //    digitalWrite(LED_PINNNN  , HIGH);
  //   Serial.println("alert : ");
  //   Serial.print("humiditySoil need water  ,  ");
  // }
  //    else{
  //      digitalWrite(LED_PINNNN , LOW);
  //      delay(1000);

  //    }




 
  
  
   
         
    // قراءة قيمة مستوى الضوء
float lightLevel = analogRead(lightSensorPin);
  Blynk.virtualWrite(V9, lightLevel);
  
  // طباعة قيمة مستوى الضوء على واجهة التسلسل
  Serial.print("Light level: ");
  Serial.println(lightLevel);

       if(lightLevel  <=1000){
    Blynk.logEvent("event_raindrops_alert");
    
    Serial.println("alert : ");
    Serial.print("There is rain now  ,  ");
  }
  else{
       delay(1000);

     }
  
// تأخير لثانية واحدة                    


           if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                      {  
                            
                             String postStr = apiKey;
                             postStr +="&field1=";
                             postStr += String(t);
                             postStr +="&field2=";
                             postStr += String(h);
                             postStr +="&field3=";
                             postStr += String(air_quality);
                             postStr +="&field4=";
                             postStr += String(soilMoisture);
                             postStr +="&field5=";
                             postStr += String(lightLevel);
                             postStr += "\r\n\r\n";
 
                             client.print("POST /update HTTP/1.1\n");
                             client.print("Host: api.thingspeak.com\n");
                             client.print("Connection: close\n");
                             client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                             client.print("Content-Type: application/x-www-form-urlencoded\n");
                             client.print("Content-Length: ");
                             client.print(postStr.length());
                             client.print("\n\n");
                             client.print(postStr);
 
                             Serial.println(" ,");
                             Serial.print("    , Temperature : ");
                             Serial.print(t); 
                             Serial.print("°C");
                             Serial.print("  , Humidity : ");
                             Serial.print(h);
                             Serial.print("%");
                             Serial.print("  , Gas Value : ");
                             Serial.print(gasValue);
                             Serial.print("  , ");
                             
                                  delay(2000);
 

                             Serial.print(" Air Quality PPM : ");
                             Serial.print(air_quality);
                             Serial.print(" PPM  ,");
                             Serial.println(" . Send to Thingspeak.");
                        }
                           client.stop();
 
          Serial.println("Waiting...");
            delay(1000);                            

}
void setup()
{   
  
  Serial.begin(115200);
   pinMode(soilSensorPin, INPUT);
   pinMode(lightSensorPin, INPUT);

  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(2000L, sendSensor);
   Serial.println("Connecting to ");
       Serial.println(ssid);
 
 
       WiFi.begin(ssid, pass);
 
  }

void loop()
{
  Blynk.run();

  timer.run();
 }




