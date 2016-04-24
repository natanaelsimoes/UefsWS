#ifndef UefsWS_HTTP_h
#define UefsWS_HTTP_h

#include "Arduino.h"
#include "UefsWS_Consts.h"
#include <SPI.h>
#include <UIPEthernet.h>

class UefsWS_HTTP
{
  public:
    char methodResource[90];
    void startEthernet();
    bool request();
    void processResponse();
    void clearMethodResource();
    void requestTimestamp();
  private:
    uint8_t _rIndex;
    bool _headerEnded;
    uint8_t _crFound;
    uint8_t _cnFound;
    EthernetClient _client;
};

#endif
