#ifndef UefsWS_Observation_h
#define UefsWS_Observation_h

#include "Arduino.h"

class UefsWS_Observation
{
  public:
    uint8_t temperature;
    uint8_t humidity;
    uint8_t day;
    uint8_t month;
    uint16_t year;
    uint8_t hour;
    uint8_t minute;
    uint8_t second; 
};

#endif
