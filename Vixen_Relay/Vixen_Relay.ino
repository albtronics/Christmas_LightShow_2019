/*There are a few #define lines on top to set the serial speed, the number of channels to use, and whether the relays activate with a high or low signal:
  -Set the number of channels that you will use with the line #define CHANNEL_COUNT 16

  -Set the mode line to "#define MODE INVERTED" or "#define MODE NOT_INVERTED" as needed. If your channels are doing the opposite than expected, change it to the other mode.

  -Set the com port speed (currently to 57600) #define VIXEN_COM_SPEED 57600

  -Set the timeout (in milliseconds) for Random mode with #define TIME_OUT 1000

  It only uses 1 library for the watchdog timer, included in the arduino IDE, so just copy the code to a new sketch, change the channels, speed, and relay mode settings, and upload to your Arduino.

  NOTE: If you use an Arduino Uno, remove all the #define lines for channels after 18, and remove those channels from the line with "int channels[] = {CH01..." as all those pins are only valid in the Arduino Mega and would give a compilation error in an Arduino Uno.

  It works in the following way:

  -Vixen needs to be setup to use a header of: ~!
  -Sets all the channels to off in startup.
  -Wait for up to 2 seconds (can be changed for longer) and if there is no valid input, it goes into random mode, blinking some lights on and off every second.
  -If it detects valid input (with ~! header), it writes to the relays, as long as Vixen is working.
  -If Vixen goes off for more than 2 seconds, back to random mode.

  Here is the code:*/

// This code was written by albtronics.com
// To adapt the code to your case, just change this top section, with the #define lines.

// Includes the watchdog timer library
#include <avr/wdt.h>

// This sets how many channels will vixen be sending. Can be set to any number from 1 to 48 for Arduino Mega, and 1 to 18 for Arduino Uno.
#define CHANNEL_COUNT 48

// speed for the com port for talking with vixen. From 9600 to 115200. Use the same speed as set in Vixen.
#define VIXEN_COM_SPEED 9600

// Timeout waiting for serial input before going to random mode (in milliseconds).
#define TIME_OUT 1000

// If the relays turn On and Off opposite to Vixen sequence, change "#define MODE NOT_INVERTED" for "#define MODE INVERTED"
#define NOT_INVERTED 0
#define INVERTED 1
#define MODE INVERTED

// which pins control which channels
// You can change these assignment to use different pins, but be very careful to not repeat the same pin number for 2 channels.
// DO NOT use pins 0 and 1, as those are for the serial port to talk to the computer.
//MEGA PINS

#define CH01 2
#define CH02 3
#define CH03 4
#define CH04 5
#define CH05 6
#define CH06 7
#define CH07 8
#define CH08 9
#define CH09 10
#define CH10 11
#define CH11 12
#define CH12 13
#define CH13 A0
#define CH14 A1
#define CH15 A2
#define CH16 A3
#define CH17 A4
#define CH18 A5

// Up to here for Arduino uno.

#define CH19 A6
#define CH20 A7
#define CH21 A8
#define CH22 A9
#define CH23 A10
#define CH24 A11
#define CH25 A12
#define CH26 A13
#define CH27 A14
#define CH28 A15
#define CH29 22
#define CH30 23
#define CH31 24
#define CH32 25
#define CH33 26
#define CH34 27
#define CH35 28
#define CH36 29
#define CH37 30
#define CH38 31
#define CH39 32
#define CH40 33
#define CH41 34
#define CH42 35
#define CH43 36
#define CH44 37
#define CH45 38
#define CH46 39
#define CH47 40
#define CH48 41

int channels[] = {CH01, CH02, CH03, CH04, CH05 , CH06, CH07, CH08, CH09,
                  CH10, CH11, CH12, CH13, CH14, CH15, CH16, CH17, CH18, CH19, CH20, CH21, CH22,
                  CH23, CH24, CH25, CH26, CH27, CH28, CH29, CH30, CH31, CH32, CH33, CH34, CH35,
                  CH36, CH37, CH38, CH39, CH40, CH41, CH42, CH43, CH44, CH45, CH46, CH47, CH48
                 };

int incomingByte[CHANNEL_COUNT];

int i = 0; // Loop counter
volatile unsigned long timer_a = 0; // new line

//setup the pins/ inputs & outputs
void setup() {

  // enable the watchdog timer with a time of 1 second. If the board freezes, it will reset itself after 1 second.
  wdt_enable(WDTO_1S);

  // specifically for the UNO
  sei();

  // initalize PWM Channels / Pins
  for (i = 0; i < CHANNEL_COUNT; i++) {
    pinMode(channels[i], OUTPUT);
  }

  // set all the realys to off to start with
  if (MODE == NOT_INVERTED) { //INVERTED
    for (i = 0; i < CHANNEL_COUNT; i++) {
      digitalWrite(channels[i], LOW);

    }
  }

  else {
    for (i = 0; i < CHANNEL_COUNT; i++) {
      digitalWrite(channels[i], HIGH);
    }
  }

  testSequence();

  // set up Serial according to the speed defined above.
  Serial.begin(VIXEN_COM_SPEED);
}

void loop()
{
  if (Serial.available() >= (CHANNEL_COUNT + 2)) {
    wdt_reset(); // resets the watchdog
    timer_a = millis (); // new line
    int uno = Serial.read();
    if (uno == 126) {

      int dos = Serial.read();
      if (dos == 33) {

        for (i = 0; i < CHANNEL_COUNT; i++) {
          // read each byte
          incomingByte[i] = Serial.read();
        }
        if (MODE == NOT_INVERTED) { //INVERTED
          for (i = 0; i < CHANNEL_COUNT; i++) {
            int value = incomingByte[i];
            if (value <= 127) {
              digitalWrite(channels[i], LOW);
            }
            else {
              digitalWrite(channels[i], HIGH);
            }
          }
        }
        else {
          for (i = 0; i < CHANNEL_COUNT; i++) {
            int value = incomingByte[i];
            if (value < 127) {
              digitalWrite(channels[i], HIGH);
            }
            else {
              digitalWrite(channels[i], LOW);
            }
          }
        }

      }
    }
  }
  // Random mode code. Random mode starts if no serial input has been received in TIME_OUT millisenconds
  else {
    wdt_reset(); // resets the watchdog
    unsigned long diff = millis() - timer_a;
    if (diff >= TIME_OUT) {
      timer_a = millis ();
      int random_a = 0;
      for (i = 0; i < CHANNEL_COUNT; i++) {
        random_a = random(0, 2);
        if (random_a == 0) {
          digitalWrite(channels[i], LOW);
        }
        else {
          digitalWrite(channels[i], HIGH);
        }
      }
    }
  }
}

void testSequence() {

  if (MODE == NOT_INVERTED) { //INVERTED
    for (i = 0; i < CHANNEL_COUNT; i++) {
      wdt_reset(); // resets the watchdog
      digitalWrite(channels[i], HIGH);
      delay (500);
      digitalWrite(channels[i], LOW);
    }
  }

  else {
    for (i = 0; i < CHANNEL_COUNT; i++) {
      wdt_reset(); // resets the watchdog
      digitalWrite(channels[i], LOW);
      delay (500);
      digitalWrite(channels[i], HIGH);
    }
  }
}
