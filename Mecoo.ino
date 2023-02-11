/*
 * Created 11 February 2023
 * By Aurchie
 * 
 * Project: SootheU
 * At: FemmeHacks 2023, Pennovation Works, Philadelphia, PA - 19104
 * Project Description: Logs body temperature and humidity (sweat) using DHT11 and Arduino Uno.
 * Especially for women going through menopause and experiencing hot flash, brain fogs and mood swings.
 *
 * Git Link:
 * DevPost:
 */

#include <LiquidCrystal.h>
#include "DHT.h"
#include <Servo.h>

#define DHTPIN 7
#define DHTTYPE DHT11

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

DHT dht(DHTPIN, DHTTYPE);
const int buttonPin = 8;  // the number of the pushbutton pin
const int ledGPin = 13;    // the number of the Green LED pin
const int ledRPin = 10;    // the number of the Red LED pin

Servo myservo;  // create servo object to control a servo

bool f_flag = true;
int pos = 0;    // variable to store the servo position
float temp_thresh = 27.0;
float humi_thresh = 15.0;

void setup()
{
  dht.begin();     // initialise the sensor
  lcd.begin(16, 2); 
  // initialize serial:
  Serial.begin(9600);
  myservo.attach(9);  // attaches the servo on pin 9 to the servo object

  // initialize the LED pin as an output:
  pinMode(ledGPin, OUTPUT);
  pinMode(ledRPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop()
{
  delay(500); // wait a few seconds between measurements

  // read the state of the pushbutton value:
   bool buttonState = digitalRead(buttonPin);

  //runs once
  while(f_flag)
  {
    Serial.print("Temperature (Celsius)");
    Serial.print(",");
    Serial.println("Humidity (%)");
    f_flag = false;
  }

  float humi  = dht.readHumidity();    // read humidity
  float tempC = dht.readTemperature(); // read temperature

  lcd.clear();
  // check if any reads failed
  if (isnan(humi) || isnan(tempC)) {
    lcd.setCursor(0, 0);
    lcd.print("Failed");
    Serial.println("Failed");
  } else {
    lcd.setCursor(0, 0);  // start to print at the first row
    lcd.print("Temp: ");
    lcd.print(tempC);     // print the temperature
    lcd.print((char)223); // print ° character
    lcd.print("C");    

    lcd.setCursor(0, 1);  // start to print at the second row
    lcd.print("Humi: ");
    lcd.print(humi);      // print the humidity
    lcd.print("%");
    
    Serial.print(tempC);     // print the temperature
    Serial.print(",");
    Serial.println(humi);      // print the humidity

    if (tempC >= temp_thresh)
    {
      lcd.clear();
      lcd.setCursor(0, 0);  // start to print at the second row
      lcd.print("Temp: ");
      lcd.print(tempC);      // print the humidity
      lcd.print((char)223); // print ° character
      lcd.print("C");
      lcd.setCursor(0, 1);  // start to print at the second row
      lcd.print("Hot! Cooling?");
      // turn Red LED on:
      digitalWrite(ledRPin, HIGH);
    }
    else if (humi >= humi_thresh && tempC >= temp_thresh)
    {
      lcd.clear();
      lcd.setCursor(0, 0);  // start to print at the second row
      lcd.print("Humi: ");
      lcd.print(humi);      // print the humidity
      lcd.print("%");
      lcd.setCursor(0, 1);  // start to print at the second row
      lcd.print("Cooling On?");
      // turn Red LED on:
      digitalWrite(ledRPin, HIGH);
    }
    else
    {
      // turn Red LED off:
      digitalWrite(ledRPin, LOW);
    }
    
    // check if the pushbutton is pressed. If it is, turn cooling on
    if (buttonState == HIGH)
    {
      lcd.clear();
      lcd.setCursor(0, 0);  // start to print at the second row
      lcd.print("Cooling On!");
      // turn Green LED on:
      digitalWrite(ledGPin, HIGH);
      coolingOn(humi);
    }
    // turn Green LED off:
    digitalWrite(ledGPin, LOW);
  }
}

void coolingOn(float h){
  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(5);                       // waits 15 ms for the servo to reach the position
      }
      for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(5);                       // waits 15 ms for the servo to reach the position
      }
}