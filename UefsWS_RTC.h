/*
  UEFS-WeatherStationRTC.h - Library for manage UEFS station RTC module
*/
#ifndef UefsWS_RTC_h
#define UefsWS_RTC_h

#include "Arduino.h"
#include <DS3231.h>

class UefsWS_RTC
{
  public:
    UefsWS_RTC();
    void startRTC();
    bool isReady();
    void syncTime(char *response);
    char *getDateStr();
    char *getTimeStr();
    Time getTime();
  private:
    DS3231 _rtc;
    bool _ready;
    char _part[5];
    long _retryTime;
};

#endif
