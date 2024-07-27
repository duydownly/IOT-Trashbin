#define BLYNK_TEMPLATE_ID "TMPL6eo0hykIC"
#define BLYNK_TEMPLATE_NAME "Kien"
#define BLYNK_AUTH_TOKEN "Q6LE46HGDE3yAmobV99p01ixGO5n2O2t"
// SSID:	FPT Telecom 7865 thuy
// Protocol:	Wi-Fi 4 (802.11n)
// Security type:	WPA2-Personal
// Network band:	2.4 GHz
// Network channel:	5
// Link speed (Receive/Transmit):	216/216 (Mbps)
// Link-local IPv6 address:	fe80::b04e:94d9:924f:c5e9%19
// IPv4 address:	192.168.0.101
// IPv4 DNS servers:	192.168.0.1
// Manufacturer:	Intel Corporation
// Description:	Intel(R) Dual Band Wireless-AC 7265
// Driver version:	19.51.15.3
// Physical address (MAC):	94-65-9C-B0-FC-7A

#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "FPT Telecom 7865 thuy";
char pass[] = "thuythinh";

SoftwareSerial espSerial(2, 3); // RX, TX

bool v0ValueChanged = false; // Flag to mark when the value of V0 changes
int v0Value = 0; // Variable to store the value of V0

void setup() {
  Serial.begin(9600); // Serial port for Arduino Uno
  espSerial.begin(9600); // Virtual Serial port for ESP8266
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Read the state of V0 when starting
  Blynk.syncVirtual(V0);
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
    // Handle response from Arduino Uno here (if needed)
  }

  delay(100);
}

BLYNK_WRITE(V0) {
  v0Value = param.asInt();
  v0ValueChanged = true; // Mark when the value of V0 changes
}
