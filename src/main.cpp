#include <Arduino.h>
#include <CAN.h>

#define TX_INTR_PIN 5
#define CAN_MSG_ID 0x12
#define SEND_MSG_INTR_PIN 20 //from MEGA RX to MEGA Tx 

uint16_t val=0;
uint8_t val_h=0;
uint8_t val_l=0;

void sendMsg() {
  CAN.beginPacket(CAN_MSG_ID);
  CAN.write(val_h);
  CAN.write(val_l);
  CAN.write(0);
  CAN.write(0);
  CAN.write(0);
  CAN.write(0);
  CAN.write(0);
  CAN.write(0);
  CAN.endPacket();

  //Indicate to Rx that transmission done
  digitalWrite(TX_INTR_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TX_INTR_PIN, LOW);
  Serial.print("Temp: ");
  Serial.print(val);
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(TX_INTR_PIN, OUTPUT);

  Serial.println("CAN Sender");
  CAN.setPins(53, 10); //CS, INT (INT unused)

  // start the CAN bus at 500 kbps
  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }

  attachInterrupt(digitalPinToInterrupt(SEND_MSG_INTR_PIN), sendMsg, FALLING);
}

void loop() {  
  val = analogRead(A0);
  val_h = (uint8_t) (val >> 8);
  val_l = (uint8_t) (val & 0x00FF);
}
