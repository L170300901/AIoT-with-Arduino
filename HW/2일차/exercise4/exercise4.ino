#include <U8g2lib.h>
#include "DHT.h"
#include <SoftPWM.h>

SOFTPWM_DEFINE_CHANNEL(A3);
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define DHTPIN A1
#define DHTTYPE DHT22
#define SOILHUMI A6
#define LAMP 17
#define PUMP 16


DHT dht(DHTPIN, DHTTYPE);

uint32_t HdtDelay = 2000;
uint32_t Hdt_ST = 0;

uint32_t Soil_Delay = 3000;
uint32_t Soil_ST = 0;

uint32_t TimeCompare;
uint32_t led_ST = 0;

uint32_t TimeSum;
int Hour = 0;
int Minute = 1;


float Temp;
float Humi;
int Soilhumi = 0;

int fan = 0;
String pump = "";

void setup() {
  // put your setup code here, to run once:
  dht.begin();
  u8g2.begin();
  SoftPWM.begin(490);


  Serial.begin(9600);
  pinMode(SOILHUMI, INPUT);

  pinMode(LAMP, OUTPUT);
  pinMode(PUMP, OUTPUT);
  TimeSum = (uint32_t)2000;

  Hdt_ST = millis();
  Soil_ST = millis();
  led_ST = millis();
}

void loop() {
  // put your main code here, to run repeatedly:
  if ((millis() - Hdt_ST) > HdtDelay) {
    Humi = dht.readHumidity();
    Temp = dht.readTemperature();

    if (isnan(Humi) || isnan(Temp)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    OLEDdraw();
    Hdt_ST = millis();
  }
  if ((millis() - Soil_ST) > Soil_Delay) {
    Soilhumi = map(analogRead(SOILHUMI), 0, 1023, 100, 0);

    OLEDdraw();
    Soil_ST = millis();
  }

  TimeCompare = (millis() - led_ST) / TimeSum;
  if (TimeCompare % 2 == 0)
  {
    digitalWrite(LAMP, LOW);
  }
  else if (TimeCompare % 2 == 1)
  {
    digitalWrite(LAMP, HIGH);
  }

  if (Temp >= 29)
  {
    fan = 100;
    SoftPWM.set(100);
  }
  else if (Temp <= 20)
  {
    fan = 0;
    SoftPWM.set(0);
  }
  else
  {
    fan = 65;
    SoftPWM.set(65);
  }

  if (Soilhumi >= 30 && Soilhumi <= 60)
  {
    digitalWrite(PUMP, HIGH);
    pump = "ON";
  }
  else
  {
    digitalWrite(PUMP, LOW);
    pump = "OFF";
  }
}


  void OLEDdraw(){
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenR08_te);
  
  u8g2.drawStr(8,8,"SMART FARM");
  u8g2.drawStr(8,20,"Temp.");
  u8g2.setCursor(60, 20);
  u8g2.print(Temp);
  u8g2.drawStr(89,20,"\xb0");
  u8g2.drawStr(94,20,"C");
  u8g2.drawStr(8, 30, "Humidity");
  u8g2.setCursor(60, 30); u8g2.print(Humi);
  u8g2.drawStr(89, 30, "%");
  u8g2.drawStr(8, 40, "SoilHumi");
  u8g2.setCursor(80, 40); u8g2.print(Soilhumi);
  u8g2.drawStr(89, 40, "%");

  u8g2.drawStr(8, 53, "Fan.   PMW=");
  u8g2.setCursor(80, 53);
  u8g2.print(fan);

  u8g2.drawStr(8, 63, "PUMP.");
  u8g2.setCursor(45, 63);
  u8g2.print(pump);

  u8g2.drawStr(73, 63, "LED.");
  u8g2.setCursor(102, 63);
  u8g2.print("2Sec");

  u8g2.sendBuffer();
}
