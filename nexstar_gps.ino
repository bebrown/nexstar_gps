#include <TinyGPS++.h>
#include <SoftwareSerial.h>
// #include <Arduino.h>

#include "define.h"

#define RXPIN 3
#define TXPIN 2

TinyGPSPlus gps;
SoftwareSerial ss(RXPIN, TXPIN);

#define PK_MAX_LEN 12
unsigned char packet[PK_MAX_LEN];   // Is 12 enough? What is the largest expected packet?
enum pk_state { PREAMBLE_WAIT, LENGTH_WAIT, DATA, CKSUM, DONE, VALID };
enum pk_state pkstate;
int pklen;
int pkidx;
int16_t cksum_accumulator;

void setup() {
  // put your setup code here, to run once:
  pkstate = PREAMBLE_WAIT;
  pklen = 0;
  pkidx = 0;
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  // Feed characters from the GPS module into TinyGPS
  while (ss.available())
    gps.encode(ss.read());

  // Feed characters from the serial port into the packet decoder
  while (Serial.available())
    packet_decode(Serial.read());

  // Check if packet is valid
  if (pkstate != VALID) return;

  // Check that destination is for me
  if (packet[2] != DEV_GPS) return;

  // It's for me! What's the command?
  switch(packet[3])
  {
    case GPS_LINKED:
    if (gps.sentencesWithFix() > 0)
      pk_send(packet[1], 1);
    else
      pk_send(packet[1], 0);
    break;

    case GPS_TIME_VALID:
    break;

    case GPS_GET_TIME:
    break;

    case GPS_GET_HW_VER:
    break;

    case GPS_GET_YEAR:
    break;

    case GPS_GET_DATE:
    break;

    case GPS_GET_LAT:
    break;

    case GPS_GET_LONG:
    break;

    case GPS_GET_SAT_INFO:
    break;

    case GPS_GET_RCVR_STATUS:
    break;

    case GPS_GET_COMPASS:
    break;

    case GPS_GET_VER:
    break;
  }
}

void packet_decode(int c)
{
  switch (pkstate)
  {
    case PREAMBLE_WAIT:
    if (c == 0x3b) {
      pkstate = LENGTH_WAIT;
    }
    break;

    case LENGTH_WAIT:
    if (c < PK_MAX_LEN) {
      pklen = c;
      pkstate = DATA;
    }
    else
      pkstate = PREAMBLE_WAIT;
    break;

    case DATA:
    packet[pkidx] = c;
    pkidx++;
    if (pkidx == pklen - 1)
      pkstate = CKSUM;
    break;

    case CKSUM:
    packet[pkidx] = c;
    pkstate = DONE;
    break;

    case DONE:
    if (pk_checksum())
      pkstate = VALID;
    else
      pkstate = PREAMBLE_WAIT;
    break;
  }
}

bool pk_checksum()
{
  char sum = 0;
  for (int i = 0; i < pklen; i++) sum += packet[i];
  return (packet[pklen] == -sum);
}

inline void cksum_init()
{
  cksum_accumulator = 0;
}

inline void cksum_update(uint8_t b)
{
  cksum_accumulator += b;
}

inline int8_t cksum_final()
{
  return (-cksum_accumulator) & 0xff;
}

// Send a 1-byte response
void pk_send(const uint8_t dest, const uint8_t byte0)
{
  cksum_init();
  // Send preamble
  ss.write(0x3b);
  // Send length 3
  cksum_update(0x03);
  ss.write(0x03);
  // Send src
  cksum_update(DEV_GPS);
  ss.write((uint8_t)DEV_GPS);
  // Send dest
  cksum_update(dest);
  ss.write(dest);
  // Send byte0
  cksum_update(byte0);
  ss.write(byte0);
  // Send checksum
  ss.write(cksum_final());
}

// Send a 2-byte response
void pk_send(const uint8_t dest, const uint8_t byte0, const uint8_t byte1)
{
  
}

// Send a 3-byte response
void pk_send(const uint8_t dest, const uint8_t byte0, const uint8_t byte1, const uint8_t byte2)
{
  
}

