#include "Arduino.h"
#include "UefsWS_HTTP.h"
#include <SPI.h>
#include <UIPEthernet.h>

void UefsWS_HTTP::startEthernet()
{
  delay(1000);
  Ethernet.begin(UefsWS_mac, UefsWS_staticIp, UefsWS_staticDns, UefsWS_staticGtw, UefsWS_staticSubnet);
}

/**
 * Sends a HTTP request to the server using the selected methodResource
 */
bool UefsWS_HTTP::request() {
  uint8_t retries = 3;
  this->_rIndex = 0;
  while(retries > 0) {
    if(DEBUG_MODE) {
      Serial.println("Con2srv");
      Serial.println();
    }
    if (this->_client.connect(SERVER, PORT)) {
      if(DEBUG_MODE) {
        Serial.print("Rqst:");
        Serial.println(this->methodResource);
        Serial.println();
      }  
      this->_client.print(this->methodResource);
      this->_client.println(" HTTP/1.1");
      this->_client.print("Host: ");
      this->_client.println(SERVER);
      this->_client.println("User-Agent: arduino-ethernet");
      this->_client.println("Connection: close");
      this->_client.println();
      retries = 0;
    } else {
      if(DEBUG_MODE) {
        Serial.println("Fail");
        Serial.println();
      }
      retries--;
      if(retries == 0) {
        clearMethodResource();
        return false;
      }
    }
  }
  clearMethodResource();
  return true;
}

/**
 * Process HTTP responses identifying the header ending and putting
 * the body until its 90th character (if needed). 
 * All HTTP response is printed on Serial port, including header.
 */
void UefsWS_HTTP::processResponse() {
  while (this->_client.available()) {
    char c = this->_client.read();
    if(this->_headerEnded && this->_rIndex < 90) {
      this->methodResource[this->_rIndex] = c;
      this->_rIndex++;
    }
    if(c == '\r') {
      this->_crFound++;
    } else if(c == '\n') {
      this->_cnFound++;
      if(this->_crFound == 2 && this->_cnFound == 2) {
        this->_headerEnded = true;
      }
    } else {
      this->_crFound = 0;
      this->_cnFound = 0;
    }
    Serial.write(c);
  }  
}

void UefsWS_HTTP::clearMethodResource() {
  for(uint8_t i = 0; i < 90; i++) { // Clears methodResource
    this->methodResource[i] = '\0';  
  }
}

void UefsWS_HTTP::requestTimestamp() {
  sprintf(this->methodResource, "GET %stimestamp",API);
  this->request();
}
