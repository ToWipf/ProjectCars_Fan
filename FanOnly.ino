int vRpm = 0, vGear = 0, vSpeed = 0, vTemp = 0, vAbs = 0, vFuel = 0, vEngineState = 0, vLevel = 0, vTime = 0, vSplit = 0;
int LEDSTATE = 0;
char incomingByte = 0;
char data[50];
String sSpeed;
String sRpm;
int counter = 0;
int button = 1;
int state = 0;
String ver = "1.3.2"; // version number
String module = "1"; // module number

const int FanPin = 10;
int FanSpeed = 0;

#include <string.h>

void setup() {

  TCCR1B = TCCR1B & 0b11111000 | 0x01;
  pinMode(FanPin, OUTPUT) ;
  pinMode(2, OUTPUT);

  digitalWrite(2, HIGH);
  analogWrite(FanPin, 100);
  delay(5000);
  analogWrite(FanPin, 20);
  digitalWrite(2, LOW);

  Serial.begin(38400);

}
void loop() {

  if (Serial.available() > 0) {
    incomingByte = Serial.read();

    if (incomingByte != ';') {
      data[counter] = incomingByte;
      data[counter + 1] = '\0';
      counter ++;
    }
    else
    {

      String stringOne = data;

      if (stringOne.substring(0, 2) == "CC")
      {
        sRpm = stringOne.substring(2, 6);
        vRpm = sRpm.toInt();

        //here the right display for RPM,LAP,Water and Oil temp,position
      }
      if (stringOne.substring(0, 3) == "VER")
      {
        //to version check
        Serial.println(ver + ";" + module + ";");
      }
      else if (stringOne.substring(0, 5) == "state")
      {
        //button state
        state = stringOne.substring(5, 6).toInt();

        if (state == 1)
        {
          String button_s = String(button);
          Serial.println(button_s + ":0:0:");
        }
      }
      else if (stringOne.substring(0, 2) == "DD")
      {
        sSpeed = stringOne.substring(2, 6);
        vSpeed = sSpeed.toInt();
        //here the speed display
      }
      else if (stringOne.substring(0, 2) == "BB")
      {
        vGear = stringOne.substring(2).toInt();
        //here the gear setup
        // -3 = clear
        // -2 = -
        // -1 = R
        // 0 = n
        // 1 - 6 = gear number

      }
      else if (stringOne.substring(0, 2) == "CH")
      {
        vLevel = stringOne.substring(2).toInt();

        //here the setup for Fule level data come in deziliter
        //fuel, oil, water pressure date come in Hektopascal

      }
      else if (stringOne.substring(0, 2) == "CA")
      {
        vTime = stringOne.substring(2).toInt();

        //here the setup for last, best and current time data come in Dezisekunde

      }
      else if (stringOne.substring(0, 2) == "CE")
      {
        vSplit = stringOne.substring(2).toInt();

        //here the setup for split times data come in ms

      }
      else if (stringOne.substring(0, 2) == "AA")
      {
        vTemp = stringOne.substring(2, 3).toInt(); //0 or 1 , 0 is off and 1 is on
        vAbs = stringOne.substring(3, 4).toInt(); //0 or 1
        vEngineState = stringOne.substring(4, 5).toInt(); //0 or 1
        vFuel = stringOne.substring(5, 6).toInt(); //0 or 1
        LEDSTATE = stringOne.substring(6, 7).toInt(); //0 - 8

        ////////////////////////////////////////////////////////////////////////////

        digitalWrite(2, vAbs);

        FanSpeed = map(vSpeed * 4, 10, 1023, 30, 255);

        if (FanSpeed > 254)
        {
          FanSpeed = 255;
        }

        analogWrite(FanPin, FanSpeed);

      }
      data[0] = '\0';
      counter = 0;
    }
  }
}

