#include <Arduino.h>

void setup() {
  Serial.begin(9600);
  delay(500);
  Serial.println("UART_TEST_READY");
}

void loop() {
  Serial.print("T=");
  Serial.println(millis());
  delay(1000);
}