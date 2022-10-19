#define PUMP 16
#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

#define SOILHUMI A6

SOFTPWM_DEFINE_CHANNEL(A3);


int Solihumi = 0;
String pump = "";

void setup() {
    // put your setup code here, to run once:
   
    u8g2.begin();
    pinMode(SOILHUMI, INPUT);
    Serial.begin(9600);
}

void loop() {
    // put your main code here, to run repeatedly:
    Solihumi = map(analogRead(SOILHUMI),0,1023,100,0);

   
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

    OLEDdraw();
    Serial.print("현재토양습도 : ");
    Serial.println(Solihumi);
}

void OLEDdraw() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_ncenB08_te);

  u8g2.drawStr(1,15,"SMART FARM");
  
  u8g2.drawStr(15,36, "SoilHumi.");
  u8g2.setCursor(85,35);
  u8g2.print(Solihumi);

  u8g2.drawStr(15,46, "DC PUMP.");
  u8g2.setCursor(85,45);
  u8g2.print(pump);
  u8g2.drawStr(119, 36, "%");

  u8g2.sendBuffer();
}
