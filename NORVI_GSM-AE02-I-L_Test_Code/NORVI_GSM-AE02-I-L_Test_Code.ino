
/*
 * GSM_AE02_LTE_I final
 *  
 */

#include <SPI.h>
#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_ADS1X15.h>

#define ANALOG_PIN_0 36

#define INPUT1 27
#define INPUT2 34
#define INPUT3 35
#define INPUT4 14
#define INPUT5 13
#define INPUT6 5
#define INPUT7 15
#define INPUT8 19

#define OUTPUT1 12
#define OUTPUT2 2

#define RS485_RX 25
#define RS485_TX 26
#define RS485_FC 22

#define GSM_RX 33
#define GSM_TX 32
#define GSM_RESET 21

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



Adafruit_ADS1115 ads1;
const float mA_Factor = 4.096 / 3269.826 ; 

int analog_value = 0;

  
int readSwitch(){
  analog_value = analogRead(ANALOG_PIN_0);

 
  return analog_value                                                                                                ; //Read analog
}



unsigned long int timer1 = 0;


void setup() {
 
  Serial.begin(115200);

  Serial.println("Hello");

  pinMode(RS485_FC, OUTPUT);
  digitalWrite(RS485_FC, HIGH);    

  
  Serial1.begin(9600, SERIAL_8N1, RS485_RX, RS485_TX); 
  Serial2.begin(115200, SERIAL_8N1, GSM_RX, GSM_TX); 

  pinMode(GSM_RESET, OUTPUT);
  digitalWrite(GSM_RESET, HIGH);    

  
  pinMode(OUTPUT1, OUTPUT);
  pinMode(OUTPUT2, OUTPUT);

  pinMode(INPUT1, INPUT);
  pinMode(INPUT2, INPUT);
  pinMode(INPUT3, INPUT);
  pinMode(INPUT4, INPUT);
  pinMode(INPUT5, INPUT);
  pinMode(INPUT6, INPUT);
  pinMode(INPUT7, INPUT);
  pinMode(INPUT8, INPUT);
  
  Wire.begin(16,17);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();

  if (!ads1.begin(0x48)) {
    Serial.println("Failed to initialize ADS 1 .");
    while (1);
  }
  ads1.setGain(GAIN_ONE);
  
 WiFi.mode(WIFI_MODE_APSTA);
 delay(300);
 wifi_test();
 delay(500);
 GSM_TEST();
 
}



void loop() {

  int16_t adc0, adc1, adc2, adc3;
  float volts0, volts1, volts2, volts3;
 
 
  Serial.print(digitalRead(INPUT1));
  Serial.print(digitalRead(INPUT2));
  Serial.print(digitalRead(INPUT3));
  Serial.print(digitalRead(INPUT4));
  Serial.print(digitalRead(INPUT5));
  Serial.print(digitalRead(INPUT6));
  Serial.print(digitalRead(INPUT7));
  Serial.print(digitalRead(INPUT8));
  Serial.println(""); 

  
  adc0 = ads1.readADC_SingleEnded(0);
  adc1 = ads1.readADC_SingleEnded(1);
  adc2 = ads1.readADC_SingleEnded(2);
  adc3 = ads1.readADC_SingleEnded(3);

  float mA1 = adc0 * mA_Factor; 
  float mA2 = adc1 * mA_Factor; 
  float mA3 = adc2 * mA_Factor; 
  float mA4 = adc3 * mA_Factor; 

  Serial.print("Current 0  : "); Serial.print(mA1); Serial.println(" mA");
  Serial.print("Current 1  : "); Serial.print(mA2); Serial.println(" mA");
  Serial.print("Current 2  : "); Serial.print(mA3); Serial.println(" mA");
  Serial.print("Current 3  : "); Serial.print(mA4); Serial.println(" mA");

  Serial.println(""); 
  Serial.print("Push button  ");Serial.println(readSwitch());
  Serial.println(""); 

  
  digitalWrite(OUTPUT1, HIGH);
  digitalWrite(OUTPUT2, LOW);
  delay(300);
  digitalWrite(OUTPUT1, LOW);
  digitalWrite(OUTPUT2, HIGH);
  delay(300);

 //gsm
 
  while (Serial.available()) {
    int inByte = Serial.read();
    Serial2.write(inByte);
  }

  while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
  }
  
  //RS485
  
  digitalWrite(RS485_FC, HIGH);                    
  delay(100);
  Serial1.println(F("RS485 01 SUCCESS"));    
  delay(100);                                
  digitalWrite(RS485_FC, LOW) ;                   

                                             
  delay(100);     
  
  while (Serial1.available()) {  
    char c = Serial1.read();    
    Serial.write(c);             
  }

 delay(300);
 
   Serial.println("-----------------"); 
}

void GSM_TEST() {

 Serial.println("Testing Modem");
  

 timer1 = millis();
  Serial2.println("AT");
  while(millis()<timer1+2000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }

  timer1 = millis();
  Serial2.println("AT+CPIN?");
  while(millis()<timer1+2000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }
  timer1 = millis();
  Serial2.println("AT+CSQ");
  while(millis()<timer1+2000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }

  timer1 = millis();
  Serial2.println("AT+GSN");
  while(millis()<timer1+2000){
    while (Serial2.available()) {
    int inByte = Serial2.read();
    Serial.write(inByte);
    }
  }


   Serial.println("Testing Modem Done");

}

void wifi_test(){
    Serial.println("");
String str_macAddress;
byte mac[6];

WiFi.macAddress(mac);

   str_macAddress = (String(mac[0] >> 4,  HEX) + String(mac[0] & 0x0F, HEX)) + (":") +
                   (String(mac[1] >> 4,  HEX) + String(mac[1] & 0x0F, HEX)) + (":") +
                   (String(mac[2] >> 4,  HEX) + String(mac[2] & 0x0F, HEX)) + (":") +
                   (String(mac[3] >> 4,  HEX) + String(mac[3] & 0x0F, HEX)) + (":") +
                   (String(mac[4] >> 4,  HEX) + String(mac[4] & 0x0F, HEX)) + (":") +
                   (String(mac[5] >> 4,  HEX) + String(mac[5] & 0x0F, HEX));
  str_macAddress.toUpperCase();
  
  Serial.println("MAC Address: " + str_macAddress);

  String ssid =  str_macAddress; 
  const char* password = "12345678";   

  Serial.println("Setting up the Wi-Fi Access Point...");
  WiFi.softAP(ssid.c_str(), password);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);

  Serial.print("Access Point SSID: ");
  Serial.println(ssid);

  Serial.println("Wi-Fi Access Point is active!");
  Serial.println("");
}
