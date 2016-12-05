#ifndef MyTypes_h
#define MyTypes_h

typedef struct {
 int hour;
 int minute;
 int seconds;
 int milliseconds;
 boolean fix;
 float latitude;
 float longitude;
 float heading;
} cur_data_t;


typedef struct {
  float latitude;
  float longitude;
  float heading;
} sp_data_t;

#endif
