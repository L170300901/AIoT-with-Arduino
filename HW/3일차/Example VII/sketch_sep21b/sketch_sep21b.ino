/ AsyncTimer를 이용하여 웹으로 측정된 온도 습도 전송하는 코드 단축하기

#include <DHT.h>
#include<Vegemite.h>
#include<AsyncTimer.h>


DHT dht(A1, DHT22);
Vegemite v;
AsyncTimer t;

void setup() {
  Serial.begin(250000);

  dht.begin();

  t.setInterval([]() {
    float humidity =dht.readHumidity();
    float temperature =dht.readTemperature();

    if (!isnan(humidity) && !isnan(temperature)) {
      v.send("temperature",temperature);
      v.send("humidity",humidity);
    }
  },2000);
}

void loop () {
  t.handle();
}
