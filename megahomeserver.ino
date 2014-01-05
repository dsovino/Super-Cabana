// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
#include <NewPing.h>
//#include <SoftwareSerial.h>
#include "DHT.h"

#define DHT1PIN 2     // what pin we're connected to
#define DHT2PIN 3     // what pin we're connected to
#define txPin 10

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

//SoftwareSerial Serial1 = SoftwareSerial(0, txPin);
// since the Serial1 does not send data back to the Arduino, we should only define the txPin
const int Serial1delay=2;  // conservative, 2 actually works

DHT dht1(DHT1PIN, DHTTYPE);
DHT dht2(DHT2PIN, DHTTYPE);

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN    11  // Arduino pin tied to echo pin on the ultrasonic sensor.
const int MAX_DISTANCE = 400; // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.



void setup() {
  Serial.begin(9600); 
  Serial1.begin(9600);
  clearLCD();
  lcdPosition(0,0);
  Serial1.print("Iniciando");
  lcdPosition(1,0);
  Serial1.print("sensores");
  dht1.begin();
  dht2.begin();
  Serial.println();
  delay(500);
  Serial1.print(".");
  delay(500);
  Serial1.print(".");
  delay(500);   
  Serial1.print(".");
  delay(500);
  Serial1.print(".");
  delay(500);
  Serial1.print(".");
  delay(500);   
  Serial1.print(".");

  //backlightOff();
}

int contador=30;

void loop() {
  //  delay(40);                      // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  unsigned int uS = sonar.ping(); // Send ping, get ping time in microseconds (uS).
  int distancia = uS / US_ROUNDTRIP_CM;
  int porcentaje = ((distancia*100)/MAX_DISTANCE);  // Reading temperature or humidity takes about 250 milliseconds!
  Serial.print("Distancia: ");
  Serial.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.print("cm");

  /*Serial1.print("Distancia: ");
  Serial1.print(uS / US_ROUNDTRIP_CM); // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial1.print("cm");*/


  if (contador==0) contador=30;
  lcdPosition(0,0);
  Serial.print(". Light Level 0x7C:  ");   
  Serial.println(contador);    
  Serial1.write(0x7C);   //command flag for backlight stuff
  Serial1.write(contador);    //light level.
  delay(20);

  contador--;


  /*if (distancia!=0) backlightOn();  
   else backlightOff();
   */
   

/*
   clearLCD();
   lcdPosition(0,0);
   Serial1.print("Distancia: ");   
   Serial1.print(distancia);
   Serial1.print("cm");  */
  TempHumGO();
}

void TempHumGO() {

  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  int h1 = dht1.readHumidity();
  int t1 = dht1.readTemperature();
  int h2 = dht2.readHumidity();
  int t2 = dht2.readTemperature();
  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t1) || isnan(h1) || isnan(t2) || isnan(h2) ) 
  {
    clearLCD();
    lcdPosition(0,0);
    Serial1.print("Failed to read from DHT");
  } 
  else 
  {
    clearLCD();
    lcdPosition(0,0);     
    Serial1.print("INT H:"); 
    Serial1.print(h1);
    Serial1.print("% T:");
    Serial1.print(t1);    
    Serial1.print("*C");
    lcdPosition(1,0); 
    Serial1.print("EXT H:"); 
    Serial1.print(h2);
    Serial1.print("% T:");
    Serial1.print(t2);
    Serial1.print("*C");    
/*
    Serial.print("Humidity: "); 
    Serial.print(h1);
    Serial.print(" %\t");
    Serial.print("Temp In: "); 
    Serial.print(t1); 
    Serial.println(" *C");

    Serial.print("Humidity: "); 
    Serial.print(h2);
    Serial.print(" %\t");
    Serial.print("Temp Ext: "); 
    Serial.print(t2);
    Serial.println(" *C");*/
  }
}

// wbp: goto with row & column a
void lcdPosition(int row, int col) {
  Serial1.write(0xFE);   //command flag
  Serial1.write((col + row*64 + 128));    //position 
  delay(Serial1delay);
}
void clearLCD(){
  Serial1.write(0xFE);   //command flag
  Serial1.write(0x01);   //clear command.
  delay(Serial1delay);
}
void backlightOn() {  //turns on the backlight
  Serial1.write(0x7C);   //command flag for backlight stuff
  Serial1.write(27);    //light level.
  delay(Serial1delay);
}
void backlightOff(){  //turns off the backlight
  Serial1.write(0x7C);   //command flag for backlight stuff
  Serial1.write(1);     //light level for off.Serial1
  delay(Serial1delay);
}
void serCommand(){   //a general function to call the command flag for issuing all other commands   
  Serial1.write(0xFE);
}

