#include <SPI.h>
#include <RH_RF95.h>

// LoRa pins for Arduino
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Relay pins
int relay1 = 4;
int relay2 = 5;
int relay3 = 6;
int relay4 = 7;

void setup() {
  Serial.begin(9600);

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);

  if (!rf95.init()) {
    Serial.println("LoRa init failed!");
    while (1);
  }
  rf95.setFrequency(433.0);  // Match transmitter frequency
  Serial.println("LoRa Receiver Ready");
}

void loop() {
  if (rf95.available()) {
    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    if (rf95.recv(buf, &len)) {
      buf[len] = '\0';  // End string
      Serial.print("Received: ");
      Serial.println((char*)buf);

      int relayNum, state;
      if (sscanf((char*)buf, "%d:%d", &relayNum, &state) == 2) {
        switch (relayNum) {
          case 1: digitalWrite(relay1, state ? HIGH : LOW); break;
          case 2: digitalWrite(relay2, state ? HIGH : LOW); break;
          case 3: digitalWrite(relay3, state ? HIGH : LOW); break;
          case 4: digitalWrite(relay4, state ? HIGH : LOW); break;
        }
      }
    }
  }
}