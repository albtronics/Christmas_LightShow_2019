// Code to Turn ON/OFF all Relay Channels.

int i;
#define CHANNEL_COUNT 48
bool Active_LOW_Relay = true;// Change to ' false ' for Active_HIGH_Relay !

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

void setup() {
  // put your setup code here, to run once:
  for (i = 0; i < CHANNEL_COUNT; i++) {
    pinMode(channels[i], OUTPUT);
  }
  for (i = 0; i < CHANNEL_COUNT; i++) {
    digitalWrite(channels[i], !Active_LOW_Relay);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
