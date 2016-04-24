#ifndef UefsWS_Manager_h
#define UefsWS_Manager_h

#include "Arduino.h"
#include <DHT.h>
#include "UefsWS_Consts.h"
#include "UefsWS_Observation.h"
#include "UefsWS_Sample.h"
#include "UefsWS_HTTP.h"
#include "UefsWS_RTC.h"

class UefsWS_Manager
{
  public:
    UefsWS_Manager();
    void setup();
    void loop();
    void queueObservation();
    void sendObservations();
    void storeSample();
    void checkSendTime();
    void clearSamples();
    void checkRTCtime();
    void calculateMean();
  private:
    UefsWS_HTTP _http;
    UefsWS_RTC _rtc;
    DHT _dht;
    UefsWS_Sample _samples[SAMPLES];
    UefsWS_Observation _observations[OBSERVATION_CACHE];
    uint8_t _saIndex;
    uint8_t _obIndex;
    long _retryRTCtime;
    long _lastMeasureTime;
    uint16_t _tempMean;
    uint16_t _humiMean;
    bool _queueProcessed;
};

#endif
