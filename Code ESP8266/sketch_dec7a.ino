#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_TEMPLATE_ID "TMPL6c6FuIxSN"
#define BLYNK_AUTH_TOKEN  "uOmziVrwOyk7dQAxHLYNzmeArtDsXCsI"

char ssid[] = "Tran Thinh";
char pass[] = "thinhthuy";

SoftwareSerial espSerial(2, 3); // RX, TX

bool v0ValueChanged = false; // Biến cờ để đánh dấu khi giá trị của V0 thay đổi
int v0Value = 0; // Biến để lưu giá trị của V0

void setup() {
  Serial.begin(9700); // Cổng Serial cho Arduino Uno
  espSerial.begin(9700); // Cổng Serial ảo cho ESP8266
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Đọc trạng thái của V0 khi khởi động
  Blynk.syncVirtual(1);
}

void loop() {
  Blynk.run();

  if (v0ValueChanged) {
    if (v0Value == 1) {
      Serial.println("OPEN");
      espSerial.println("OPEN");
    } else {
      Serial.println("CLOSE");
      espSerial.println("CLOSE");
    }
    v0ValueChanged = false;
  }

  if (espSerial.available() > 0) {
    String response = espSerial.readStringUntil('\n');
    // Xử lý phản hồi từ Arduino Uno tại đây (nếu cần)
  }

  delay(100);
}

BLYNK_WRITE(V0) {
  v0Value = param.asInt();
  v0ValueChanged = true; // Đánh dấu khi giá trị của V0 thay đổi
}