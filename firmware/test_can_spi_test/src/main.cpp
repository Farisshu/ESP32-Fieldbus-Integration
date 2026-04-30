#include <Arduino.h>
// ⚡ DIAGNOSTIC BLINK TEST
void setup() {
  // Setup 4 pin SPI sebagai output
  pinMode(5, OUTPUT);   // CS
  pinMode(18, OUTPUT);  // SCK
  pinMode(23, OUTPUT);  // MOSI
  pinMode(19, OUTPUT);  // MISO
}

void loop() {
  // Semua HIGH selama 100ms
  digitalWrite(5, HIGH);
  digitalWrite(18, HIGH);
  digitalWrite(23, HIGH);
  digitalWrite(19, HIGH);
  delay(100);
  
  // Semua LOW selama 100ms
  digitalWrite(5, LOW);
  digitalWrite(18, LOW);
  digitalWrite(23, LOW);
  digitalWrite(19, LOW);
  delay(100);
}