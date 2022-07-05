#define BLYNK_TEMPLATE_ID "TMPLUGbtGP1e"
#define BLYNK_DEVICE_NAME "Plant"
#define BLYNK_AUTH_TOKEN "-P8_LxYs3mnOF29cEOcb7s7VJzw6H6I8"


// Comment this out to disable prints and save space
#define BLYNK_PRINT Serial



#define soil_D D2 //pin for digital input from soil humidity sensor
#define waterL A0 //pin for analog input from water level sensor
#define pump D7 //pin to tell arduino to power the water pump


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "ThingSpeak.h"

char auth[] = BLYNK_AUTH_TOKEN;

WiFiClient  client;
unsigned long myChannelNumber = 1771832;//thingspeak channel number
const char * myWriteAPIKey = "93YXUN8JNYR0DDV4";//thingspeak api key

const long interval = 20000;  // interval at which to update thingspeak
unsigned long previousMillis = 0; //used to determine if enought time has passed from previous sensor reading
unsigned long currentMillis;

int water_level, humidity;//variables to store sensor values
int val;//blynk button state

//WIFI SETTINGS
char ssid[] = "ONEPLUS_TPO";
char password[] = "ABABABAB";
BlynkTimer timer;
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); //get button value from blynk
  val = pinValue; 
  Serial.print("V1 Slider value is: ");
  Serial.println(val);
}



void Event()
{
   water_level = analogRead(waterL);//read sensor values
   humidity = digitalRead(soil_D);
   Blynk.virtualWrite(V5, water_level);
   Blynk.virtualWrite(V3, humidity);
   Serial.print("humidity:");
   Serial.print(humidity);
   Serial.print("        water_level:");
   Serial.print(water_level);
   Serial.print("        button:");
   Serial.println(val);
   if(humidity==1||val==1)
   {
     Serial.println("pump_ON");
     digitalWrite(pump, HIGH); //send signal to arduino to power the pump
     delay(2000);//wait
     digitalWrite(pump, LOW);//disable pump
   }
  if(currentMillis - previousMillis >= interval) {    
    // save the last time thingspeak was updated
    updateThingspeak();
    previousMillis = currentMillis;
  }
    
    

}

void updateThingspeak()
{
  ThingSpeak.setField(1, humidity);//set fields
  ThingSpeak.setField(2, water_level);
  ThingSpeak.setField(3, val);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(pump, OUTPUT); // Set GPIO13 as an OUTPUT.
  pinMode(soil_D, INPUT);// Set GPIO4 as an INPUT.
  digitalWrite(pump, 0); // Start off.
  Blynk.begin(auth, ssid, password);
  // Setup a function to be called every second
  timer.setInterval(1000L, Event);


  Serial.println("Connecting ");
  ThingSpeak.begin(client);
  WiFi.begin(ssid, password);

  // Show ... until connected:
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");

  // Print the IP address of the device:
  Serial.println(WiFi.localIP());
  
  water_level=-1;
  humidity=-1;//placeholder values to determine if sensors have been read
  val=0;
}


void loop() {
  currentMillis = millis();
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}
