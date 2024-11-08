//e2852iot@gotgel.org
//Info@2024

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <Wire.h>
#include <LCD_I2C.h>
LCD_I2C lcd(0x27);



#define BLYNK_TEMPLATE_ID "TMPL3VP60_iFp"
#define BLYNK_TEMPLATE_NAME "SEWAGE GAS MONITORING"
#define BLYNK_AUTH_TOKEN "CF1nrIpwpfWSJeSCqxjjtdTdndKfXfUe"

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "IOT";
char pass[] = "123456789";

//Mux control pins
int s0 = D8;
int s1 = D7;
int s2 = D6;
int s3 = D5;

//Mux in "SIG" pin
int SIG_pin = A0;


int gas1, gas2, gas3;
void setup()
{
  Serial.begin(9600);
  lcd.begin();                 //Init the LCD
  lcd.backlight();            //Activate backlight
  lcd.home();
  lcd.setCursor(0, 0);
  lcd.print("SEWAGE MONITORING");

  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);

  digitalWrite(s0, LOW);
  digitalWrite(s1, LOW);
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);

  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  lcd.clear();
}

void loop()
{
  gas1 = readMux(0);
  gas2 = readMux(1);
  gas3 = readMux(2);



  gas1 = map(gas1, 300, 1023, 0, 100);
  if (gas1 <= 0) {
    gas1 = 0;
  }
  lcd.setCursor(0, 0);
  lcd.print("G1:");
  if (gas1 <= 9) {
    lcd.print("00");
    lcd.print(gas1);
  }
  else if (gas1 <= 99) {
    lcd.print("0");
    lcd.print(gas1);
  }
  else if (gas1 <= 999) {
    lcd.print(gas1);
  }


  gas2 = map(gas2, 300, 1023, 0, 100);
  if (gas2 <= 0) {
    gas2 = 0;
  }
  lcd.setCursor(8, 0);
  lcd.print("G2:");
  if (gas2 <= 9) {
    lcd.print("00");
    lcd.print(gas2);
  }
  else if (gas2 <= 99) {
    lcd.print("0");
    lcd.print(gas2);
  }
  else if (gas2 <= 999) {
    lcd.print(gas2);
  }


  gas3 = map(gas3, 300, 1023, 0, 100);
  if (gas3 <= 0) {
    gas3 = 0;
  }
  lcd.setCursor(0, 1);
  lcd.print("G3:");
  if (gas3 <= 9) {
    lcd.print("00");
    lcd.print(gas3);
  }
  else if (gas3 <= 99) {
    lcd.print("0");
    lcd.print(gas3);
  }
  else if (gas3 <= 999) {
    lcd.print(gas3);
  }


  delay(300);



  Blynk.virtualWrite(V0, gas1);
  Blynk.virtualWrite(V1, gas2);
  Blynk.virtualWrite(V2, gas3);
  Blynk.run();
}












float readMux(int channel) {
  int controlPin[] = {s0, s1, s2, s3};

  int muxChannel[16][4] = {
    {0, 0, 0, 0}, //channel 0
    {1, 0, 0, 0}, //channel 1
    {0, 1, 0, 0}, //channel 2
    {1, 1, 0, 0}, //channel 3
    {0, 0, 1, 0}, //channel 4
    {1, 0, 1, 0}, //channel 5
    {0, 1, 1, 0}, //channel 6
    {1, 1, 1, 0}, //channel 7
    {0, 0, 0, 1}, //channel 8
    {1, 0, 0, 1}, //channel 9
    {0, 1, 0, 1}, //channel 10
    {1, 1, 0, 1}, //channel 11
    {0, 0, 1, 1}, //channel 12
    {1, 0, 1, 1}, //channel 13
    {0, 1, 1, 1}, //channel 14
    {1, 1, 1, 1} //channel 15
  };

  //loop through the 4 sig
  for (int i = 0; i < 4; i ++) {
    digitalWrite(controlPin[i], muxChannel[channel][i]);
  }

  //read the value at the SIG pin
  int val = analogRead(SIG_pin);

  //return the value
  // float voltage = (val * 5.0) / 1024.0;
  return val;
}
