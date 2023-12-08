#include <DistanceSRF04.h>
#include <Servo.h>

DistanceSRF04 Dist;
Servo myservo;

#define SERVO       11
#define SRF04_ECHO  12
#define SRF04_TRIG  13

#define GOC_DONG 130 // Góc đóng của servo
#define GOC_MO   0   // Góc mở của servo

// Use the hardware Serial for ESP8266 communication
#define espSerial Serial

// Declare enum MODE_STATE before setup
typedef enum
{
  IDLE_STATE,
  OPEN_STATE,
  CLOSE_STATE,
  ESP_COMMAND_STATE
} MODE_STATE;

int distance;
unsigned long previousMillis = 0;
MODE_STATE modeRun = IDLE_STATE;
uint32_t timeMillis = 0;
uint8_t timeOpen = 2;

void setup() {
  Serial.begin(9700);
  // Echo, Trigger
  Dist.begin(SRF04_ECHO, SRF04_TRIG);
  myservo.attach(SERVO);
  myservo.write(GOC_DONG);
}

void loop() {
  switch (modeRun) {
    case IDLE_STATE:
      if (readSRF04() == 1) {
        modeRun = OPEN_STATE;
        openServo();
        timeMillis = millis();
      }
      break;
    case OPEN_STATE:
      if (millis() - timeMillis > timeOpen * 1000) {
        timeMillis = millis();
        modeRun = CLOSE_STATE;
      }
      if (readSRF04() == 1) {
        timeMillis = millis();
      }
      break;
    case CLOSE_STATE:
      closeServo();
      delay(500);
      
      modeRun = IDLE_STATE;
      break;
  }
  readCommandFromESP();
  delay(100);  // Hoặc thời gian cần thiết để thực hiện công việc khác
}

uint8_t readSRF04() {
  if (millis() - previousMillis >= 10) {
    previousMillis = millis();
    distance = Dist.getDistanceCentimeter();
    return (distance < 10 && distance > 1) ? 1 : 0;
  }
  return 0;
}

void readCommandFromESP() {
  while (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();  // Trim leading and trailing whitespaces

    Serial.print("Received command from ESP8266: ");
    Serial.println(command);

    if (command.equals("OPEN")) {
      Serial.println("Executing OPEN command");
      openServo();
      modeRun = OPEN_STATE;
      timeMillis = millis();
    } else if (command.equals("CLOSE")) {
      Serial.println("Executing CLOSE command");
      modeRun = CLOSE_STATE;
      closeServo();
    } else {
      Serial.println("Unknown command received");
    }
  }
}


void openServo() {
  myservo.write(GOC_MO);
  digitalWrite(13, HIGH);
}

void closeServo() {
  myservo.write(GOC_DONG);
  digitalWrite(13, LOW);
}
