#include <DrivenMotor.h>
#include <SerialReader.h>
#include <ESP32Servo.h>
#include "Claw.h"
#include "Wrist.h"

SerialReader reader;

Claw claw(95, 150, 22);
Wrist wrist(19, 21, 22, 23);

void readCommand(size_t argc, char argv[]) {
   // argc = how many safe characters from the beginning
   // all data contained in argv
   // commands in the form of </xy...>
   // x = subsystem (eg. c, w)
   // y = command type (subsystem specific)
   // ... = data
   if (argc < 3) return;
   if (argc >= 3 && argv[1] == '/') {
      switch (argv[2]) {
         case 'c': // claw
            if (argc == 6 && argv[3] == 'm') { // move
               if (argv[4] == '0') claw.openClaw();
               else {
                  int val = (argv[4] - '1') * 2;
                  claw.move(-val); 
               }
            } 
            break;
         case 'w': // wrist
            if (argc == 7 && argv[3] == 'm') { // move
               float yaw = (argv[4] - '5') * 500.0;
               float pitch = (argv[5] - '5') * 500.0;
               wrist.set(yaw, pitch);
            }
            break;
         default:
            break;
      }
   } else { // comment (comment)
      for (size_t i = 0; i < argc; i++) 
         Serial.print(argv[i]);
      Serial.println();
   }
}

void setup()
{  
   Serial.begin(115200);
   Serial2.begin(115200);
}

void run() {
   wrist.run();
}

void loop()
{
   reader.readSerial(&readCommand);
   run();
   delay(10);
}
