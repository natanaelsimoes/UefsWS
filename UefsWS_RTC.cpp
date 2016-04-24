#include "Arduino.h"
#include <DS3231.h>
#include "UefsWS_RTC.h"

UefsWS_RTC::UefsWS_RTC(): _rtc(SDA, SCL)
{ }

void UefsWS_RTC::startRTC() 
{
  _retryTime = 0;
  _rtc.begin();
  _ready = (_rtc.getTime().year >= 2016);  
}

bool UefsWS_RTC::isReady() 
{
  return _ready;  
}

void UefsWS_RTC::syncTime(char *response)
{
  uint8_t m,d,h,i,s,dow;
  uint16_t y;
  if(response[20] != '\0') {
    memcpy(_part, &response[0], 1);
    _part[1] = '\0';
    dow = atoi(_part);
    memcpy(_part, &response[2], 2);
    _part[2] = '\0';
    m = atoi(_part);
    memcpy(_part, &response[5], 2);
    _part[2] = '\0';
    d = atoi(_part);
    memcpy(_part, &response[8], 4);
    _part[4] = '\0';
    y = atoi(_part);
    memcpy(_part, &response[13], 2);
    _part[2] = '\0';
    h = atoi(_part);
    memcpy(_part, &response[16], 2);
    _part[2] = '\0';
    i = atoi(_part);
    memcpy(_part, &response[19], 2);
    _part[2] = '\0';
    s = atoi(_part);
    _rtc.setDOW(dow);
    _rtc.setTime(h,i,s);
    _rtc.setDate(d,m,y);
    _ready = true;
  }
}

char* UefsWS_RTC::getDateStr() {
  return this->_rtc.getDateStr();
}

char* UefsWS_RTC::getTimeStr() {
  return this->_rtc.getTimeStr();
}

Time UefsWS_RTC::getTime() {
  return this->_rtc.getTime();
}



