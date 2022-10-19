#include <DHT.h>
#include <AsyncTimer.h>
#include <Vegemite.h>
#include <SoftPWM.h>

auto SOILMOIST_PIN = A6;
auto DHT22_PIN = A1;
auto FAN_PIN = A3;
auto PUMP_PIN = 16;
auto LAMP_PIN = 17;

SOFTPWM_DEFINE_CHANNEL(FAN_PIN); 
DHT dht(DHT22_PIN, DHT22);
AsyncTimer t;
Vegemite v;

bool currentPumpWorking = false;

void setup(){
  Serial.begin(250000);
  SoftPWM.begin(490);
  dht.begin();

  pinMode(SOILMOIST_PIN, INPUT);  
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(LAMP_PIN, OUTPUT);

  v.requestSubscription("config-light");  //웹에서 버튼 작동 감지 후 전송
  v.requestSubscription("config-fan");    //웹에서 슬라이드 작동 감지 후 전송
  v.requestSubscription("pump-water");    //웹에서 펌프업 버튼 작동 감지 후 전송 
  v.requestSubscription("AutoMode");      //오토 버튼 작동 감지 후 전송
  
  

  t.setInterval([]() {
    float humidity =dht.readHumidity();    습도
    float temperature =dht.readTemperature(); 온도

    if (!isnan(humidity) && !isnan(temperature)) {
      v.send("temperature",temperature);온도 웹에 표시
      v.send("humidity",humidity); 습도 웹에 표시
    }
    int soilMoist = map(analogRead(SOILMOIST_PIN), 0, 1023, 100, 0);  /토양습도 값 변환
    v.send("soilmoist", soilMoist);   토양습도 웹에 표
}, 1000);

  t.setInterval([](){시
    float humidity =dht.readHumidity();
    float temperature =dht.readTemperature();
    int pumpWater = int(v.recv("pump-water"));
    int lightConf = int(v.recv("config-light"));
    int fanSpeed = int(v.recv("config-fan"));
    int AutoMode = int(v.recv("AutoMode"));
                      // 웹에서 전송받은 값을 int 형으로 받음
                      
    if(AutoMode == 1){  // 오토모드일때  LED 작동 조건문
      if( temperature < 25){
        digitalWrite(LAMP_PIN,HIGH);        
      }
      else {
        digitalWrite(LAMP_PIN,LOW);}
    }   
    else {
      digitalWrite(LAMP_PIN, lightConf == 1 ? HIGH : LOW);
    }       
    
    
    
    if(AutoMode == 1){ // 오토모드일때  펌프 작동 조건문
      if(humidity <=60 ){
        digitalWrite(PUMP_PIN, HIGH);
        }
      else {
        digitalWrite(PUMP_PIN, LOW);
        }}
    else {     
      if(pumpWater == 1 && !currentPumpWorking){
        currentPumpWorking = true;
        v.send("pump-water", 0);
        digitalWrite(PUMP_PIN, HIGH);
        t.setTimeout([](){
          digitalWrite(PUMP_PIN, LOW);
          currentPumpWorking = false;
        }, 5000);
    }       
   }

     
    if(AutoMode == 1){ // 오토모드일때  팬 작동 조건문
      if(humidity >=75 ){
        SoftPWM.set(100);
  }
      else {
        SoftPWM.set(0);
  }}
    else {
      SoftPWM.set(fanSpeed);
    }}, 500);
}
  


void loop(){
  v.subscribe();
  t.handle();
}
