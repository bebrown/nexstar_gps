#define GPS_GET_LAT     0x01
#define GPS_GET_LONG    0x02
#define GPS_GET_DATE    0x03
#define GPS_GET_YEAR    0x04
#define GPS_GET_SAT_INFO  0x07
#define GPS_GET_RCVR_STATUS 0x08
#define GPS_GET_TIME    0x33
#define GPS_TIME_VALID  0x36
#define GPS_LINKED      0x37
#define GPS_GET_HW_VER  0x55
#define GPS_GET_COMPASS 0xa0
#define GPS_GET_VER     0xfe

#define GPS_N   0x0b
#define GPS_NE  0x09
#define GPS_E   0x0d
#define GPS_SE  0x0c
#define GPS_S   0x0e
#define GPS_SW  0x06
#define GPS_W   0x07
#define GPS_NW  0x03

#define GPS_HW_VER   0xab    // Motorola HW version

#define DEV_MAIN 0x01
#define DEV_HC   0x04
#define DEV_AZM  0x10
#define DEV_ALT  0x11
#define DEV_GPS  0xb0
