#include <SoftPWM.h>
#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define SOILHUMI A6

SOFTPWM_DEFINE_CHANNEL(A3);

float Temp;
int fan = 0;

void setup() {
  // put your setup code here, to run once:
  SoftPWM.begin(490);
  Serial.begin(9600);
  u8g2.begin();
  pinMode(Temp, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Temp = map(analogRead(SOILHUMI), 0, 1023, 100, 0);

  if (Temp >= 30)
  {
    fan = 100;
    SoftPWM.set(fan);
  }
  else if (Temp <= 25)
  {
    fan = 0;
    SoftPWM.set(fan);
  }
  else
    fan = 65;
  SoftPWM.set(fan);

  OLEDdraw();
  Serial.print("현재 온도 : ");
  Serial.println(Temp);
}

void OLEDdraw() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_te);

  u8g2.drawStr(1, 15, "SMART FARM");

  u8g2.drawStr(15, 36, "Temp.");
  u8g2.setCursor(85, 35);
  u8g2.print(Temp);

  u8g2.drawStr(15, 46, "FAN.");
  u8g2.setCursor(85, 45);
  u8g2.print(fan);
  u8g2.drawStr(119, 36, "%");

  u8g2.sendBuffer();
}
