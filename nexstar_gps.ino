#include <TinyGPS++.h>
#include <SoftwareSerial.h>
// #include <Arduino.h>

#include "define.h"

#define RXPIN 3
#define TXPIN 2

TinyGPSPlus gps;
SoftwareSerial ss(RXPIN, TXPIN);

TinyGPSCustom satellitesInView(gps, "GPGSV", 3);
TinyGPSCustom fix3D(gps, "GPGSA", 2);    // 1 = no fix, 2 = 2D fix, 3 = 3D fix
TinyGPSCustom fixQuality(gps, "GPGGA", 6);  // 0 = invalid, 1 = GPS, 2 = DGPS, etc...

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
  if (packet[2] != DEV_GPS) {
    pkstate = 0;
    pkidx = 0;
    pklen = 0;
    return;
  }

  // It's for me! What's the command?
  uint8_t dest = packet[1];
  switch(packet[3])
  {
    case GPS_LINKED:
    case GPS_TIME_VALID:
    if (fixQuality.value() > 0)
      pk_send(dest, packet[3], 1);
    else
      pk_send(dest, packet[3], 0);
    break;

    case GPS_GET_TIME:
    pk_send(dest, GPS_GET_TIME, gps.time.hour(), gps.time.minute(), gps.time.second());
    break;

    case GPS_GET_HW_VER:
    pk_send(dest, GPS_GET_HW_VER, GPS_HW_VER);
    break;

    case GPS_GET_YEAR:
    pk_send(dest, GPS_GET_YEAR, gps.date.year() >> 8, gps.date.year() & 0xff);
    break;

    case GPS_GET_DATE:
    pk_send(dest, GPS_GET_DATE, gps.date.month(), gps.date.day());
    break;

    case GPS_GET_LAT:
    break;

    case GPS_GET_LONG:
    break;

    case GPS_GET_SAT_INFO:
    pk_send(dest, GPS_GET_SAT_INFO, satellitesInView.value(), gps.satellites.value());
    break;

    case GPS_GET_RCVR_STATUS:
    break;

    case GPS_GET_COMPASS:
    break;

    case GPS_GET_VER:
    pk_send(dest, GPS_GET_VER, 0, 1);  // Version 0.1
    break;
  }

  pkstate = PREAMBLE_WAIT;
  pklen = 0;
  pkidx = 0;
}

void packet_decode(int8_t c)
{
  Serial.write(c);
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
      packet[0] = c;
      pkidx = 1;
      pkstate = DATA;
    }
    else
      pkstate = PREAMBLE_WAIT;
    break;

    case DATA:
    packet[pkidx] = c;
    pkidx++;
    if (pkidx == pklen + 1)
      pkstate = CKSUM;
    break;

    case CKSUM:
    if (pk_checksum(c))
      pkstate = VALID;
    else
      pkstate = PREAMBLE_WAIT;
    break;
  }
  Serial.write(pkstate);
}

bool pk_checksum(int8_t target)
{
  int sum = 0;
  for (int i = 0; i <= pklen; i++) sum += packet[i];
  Serial.write(sum & 0xff);
  int8_t chk = (-sum) & 0xff;
  return (target == chk);
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
void pk_send(uint8_t dest, uint8_t id, uint8_t byte0)
{
  cksum_init();
  // Send preamble
  Serial.write(0x3b);
  // Send length 4
  cksum_update(0x04);
  Serial.write(0x04);
  // Send src
  cksum_update(DEV_GPS);
  Serial.write((uint8_t)DEV_GPS);
  // Send dest
  cksum_update(dest);
  Serial.write(dest);
  // Send message id
  cksum_update(id);
  Serial.write(id);
  // Send byte0
  cksum_update(byte0);
  Serial.write(byte0);
  // Send checksum
  Serial.write(cksum_final());
}

// Send a 2-byte response
void pk_send(uint8_t dest, uint8_t id, uint8_t byte0, uint8_t byte1)
{
  cksum_init();
  // Send preamble
  Serial.write(0x3b);
  // Send length 5
  cksum_update(0x05);
  Serial.write(0x05);
  // Send src
  cksum_update(DEV_GPS);
  Serial.write((uint8_t)DEV_GPS);
  // Send dest
  cksum_update(dest);
  Serial.write(dest);
  // Send message id
  cksum_update(id);
  Serial.write(id);
  // Send byte0
  cksum_update(byte0);
  Serial.write(byte0);
  // Send byte1
  cksum_update(byte1);
  Serial.write(byte1);
  // Send checksum
  Serial.write(cksum_final());
}

// Send a 3-byte response
void pk_send(uint8_t dest, uint8_t id, uint8_t byte0, uint8_t byte1, uint8_t byte2)
{
  cksum_init();
  // Send preamble
  Serial.write(0x3b);
  // Send length 6
  cksum_update(0x06);
  Serial.write(0x06);
  // Send src
  cksum_update(DEV_GPS);
  Serial.write((uint8_t)DEV_GPS);
  // Send dest
  cksum_update(dest);
  Serial.write(dest);
  // Send message id
  cksum_update(id);
  Serial.write(id);
  // Send byte0
  cksum_update(byte0);
  Serial.write(byte0);
  // Send byte1
  cksum_update(byte1);
  Serial.write(byte1);
  // Send byte2
  cksum_update(byte2);
  Serial.write(byte2);
  // Send checksum
  Serial.write(cksum_final());
}

