// Example for M5Stack StepMotor Module (I2C verison)
// 2018/6/27 JimmyLai
#include <M5Stack.h>
#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  Wire.begin();
  Serial.begin(115200);
  m5.Lcd.setTextColor(WHITE, BLACK);
  m5.Lcd.setTextSize(2);
  m5.lcd.setBrightness(100);
  M5.Lcd.setCursor(4, 10);
  M5.Lcd.println("StepMotor Test: 0x70/0x71");
  M5.Lcd.setCursor(4, 30);
  M5.Lcd.println("Press A: 0x70");
  M5.Lcd.setCursor(4, 50);
  M5.Lcd.println("Press B: 0x71");
}

void SendByte(byte addr, byte b) {
  Wire.beginTransmission(addr);
  Wire.write(b);
  Wire.endTransmission();
}

void SendCommand(byte addr, char *c) {
  Wire.beginTransmission(addr);
  while ((*c) != 0) {
    Wire.write(*c);
    c++;
  }
  Wire.write(0x0d);
  Wire.write(0x0a);
  Wire.endTransmission();
}

void loop() {
  if (digitalRead(39) == LOW)  // A button
  {
    while (digitalRead(39) == LOW) delay(1);
    SendCommand(0x70, "G1 X20Y20Z20 F500");
    SendCommand(0x70, "G1 X0Y0Z0 F400");
  }
  if (digitalRead(38) == LOW)  // B button
  {
    while (digitalRead(38) == LOW) delay(1);
    SendCommand(0x71, "G1 X20Y20Z20 F500");
    SendCommand(0x71, "G1 X0Y0Z0 F400");
  }
  if (digitalRead(37) == LOW)  // C button
  {
    while (1) {
      SendCommand(0x70, "G1 X0Y0Z0 F500");
      SendCommand(0x71, "G1 X0Y0Z0 F500");
      delay(1000);
      M5.Lcd.print(".");
      delay(1000);
      SendCommand(0x70, "G1 X5Y5Z5 F500");
      SendCommand(0x71, "G1 X5Y5Z5 F500");
      delay(1000);
      M5.Lcd.print(".");
      delay(1000);
    }
  }
  // Get Data from Module.
  Wire.requestFrom(0x70, 1);
  if (Wire.available() > 0) {
    int u = Wire.read();
    if (u != 0) Serial.write(u);
  }
  Wire.requestFrom(0x71, 1);
  if (Wire.available() > 0) {
    int u = Wire.read();
    if (u != 0) Serial.write(u);
  }
  delay(1);
  // Send Data to Module.
  while (Serial.available() > 0) {
    int inByte = Serial.read();
    SendByte(0x70, inByte);
    SendByte(0x71, inByte);
  }
}
