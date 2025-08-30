#define BLYNK_TEMPLATE_NAME "nodemcu"
#define BLYNK_TEMPLATE_ID "TMPL3g4hXvRQv"  // Replace with your actual Template ID
#define BLYNK_AUTH_TOKEN "kJDr7u94Wg596TieI3DqiPqlg8g0KPIy"

#include <ESP8266WiFi.h>
#include<Blynk.h>
#include <BlynkSimpleEsp8266.h>
#include <SPI.h>
#include <RH_RF95.h>

// WiFi credentials
char ssid[] = "Galaxy A220267";
char pass[] = "varshini";

// LoRa RA-02 (SX1278) pins
#define RFM95_CS 15
#define RFM95_RST 16
#define RFM95_INT 5

RH_RF95 rf95(RFM95_CS, RFM95_INT);

// Function to send LoRa command
void sendCommand(const char* command) {
  rf95.send((uint8_t *)command, strlen(command));
  rf95.waitPacketSent();
  Serial.print("Sent via LoRa: ");
  Serial.println(command);
}

// Virtual button handlers
BLYNK_WRITE(V0) { sendCommand(param.asInt() ? "ON1" : "OFF1"); }
BLYNK_WRITE(V1) { sendCommand(param.asInt() ? "ON2" : "OFF2"); }
BLYNK_WRITE(V2) { sendCommand(param.asInt() ? "ON3" : "OFF3"); }
BLYNK_WRITE(V3) { sendCommand(param.asInt() ? "ON4" : "OFF4"); }

void setup() {
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // LoRa setup
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95.init()) {
    Serial.println("LoRa init failed.");
    while (1);
  }

  rf95.setFrequency(433.0); // Or 433.0 / 868.0 MHz depending on your module
  rf95.setTxPower(13, false);
  Serial.println("LoRa Gateway ready.");
}

void loop() {
  Blynk.run();
}