#include <U8g2lib.h>
#include "DHT.h"
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define DHTPIN A1
#define DHTTYPE DHT22
#define SOILHUMI A6

DHT dht(DHTPIN, DHTTYPE);

uint32_t DataCaptureDelay = 3000; //ms (3초마다 데이터 측정)
uint32_t StartTime = 0; //Start Time

float Temp;
float Humi;
int Soilhumi = 0;

void setup() {
  dht.begin();
  u8g2.begin();
  Serial.begin(9600);
  pinMode(SOILHUMI, INPUT);
  StartTime = millis();
}

void loop() {
  if ((millis() - StartTime) > DataCaptureDelay) { //3초 간격으로 실행 
    Humi = dht.readHumidity();
    Temp = dht.readTemperature();
    Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);
    
     //Check if any reads failed and exit early (to try again)
    if (isnan(Humi) || isnan(Temp)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    OLEDdraw(); //OLED에 데이터 출력
    StartTime = millis();
  }
}

void OLEDdraw() {
  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_ncenB08_te);
  u8g2.drawStr(1,15,"SMART FRAM");

  u8g2.drawStr(15,36,"Temp.");
  u8g2.setCursor(85,36);
  u8g2.print(Temp);
  u8g2.drawStr(114,36,"\xbO");
  u8g2.drawStr(119,36,"C");
  
  u8g2.drawStr(15,47,"Humidity");
  u8g2.setCursor(85,47); 
  u8g2.print(Humi);
  u8g2.drawStr(116,47,"%");
  
  u8g2.drawStr(15,58,"Soilhumi");
  u8g2.setCursor(85,58);
  u8g2.print(Soilhumi);
  u8g2.drawStr(116,58,"%");


  u8g2.sendBuffer();
}
