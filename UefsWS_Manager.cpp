#include "Arduino.h"
#include "UefsWS_Manager.h"

UefsWS_Manager::UefsWS_Manager(): _dht(DHT_PIN, DHT_TYPE)
{}

void UefsWS_Manager::setup()
{
  Serial.begin(9600);
  if(strlen(STATION) > 23) { 
    Serial.println("Constant STATION length needs to be less than or equal to 23 characters.");
    while(1);
  }
  this->_http.startEthernet();
  this->_rtc.startRTC();
  this->_dht.begin();
  if(DEBUG_MODE) {
    Serial.print("IP:");
    Serial.println(Ethernet.localIP());
    Serial.print("RTC:");
    Serial.print(this->_rtc.getDateStr());
    Serial.print(" ");
    Serial.println(this->_rtc.getTimeStr());
  }
}

void UefsWS_Manager::loop()
{
  this->_http.processResponse();
  if(!this->_rtc.isReady()) { // If RTC is not update, then application is not ready to begin
    this->checkRTCtime();
  } else {
    if(this->_saIndex == SAMPLES) {
      this->queueObservation();
      this->sendObservations();
    } else if (millis() - this->_lastMeasureTime > SEND_INTERVAL / SAMPLES) {
      this->storeSample();  
    }
  }
}

void UefsWS_Manager::clearSamples()
{
  for(uint8_t i = 0; i < SAMPLES; i++) {
    this->_samples[i].temperature = 0;
    this->_samples[i].humidity = 0;
  }
  this->_saIndex = 0;
}

void UefsWS_Manager::checkRTCtime()
{
  if(this->_http.methodResource[0] != '\0') { // If response was detected
    this->_rtc.syncTime(this->_http.methodResource); // Then sync date and time with server
  } else if(millis() - this->_retryRTCtime > 15000){
    this->_http.requestTimestamp(); // If no response was detected, try request again
    this->_retryRTCtime = millis();
  }
}

void UefsWS_Manager::queueObservation() {
  Time timestamp;
  if(this->_obIndex <= OBSERVATION_CACHE - 1) {
    calculateMean();
    clearSamples();
    timestamp = this->_rtc.getTime();
    this->_observations[this->_obIndex].temperature = this->_tempMean;
    this->_observations[this->_obIndex].humidity = this->_humiMean;
    this->_observations[this->_obIndex].day = timestamp.date;
    this->_observations[this->_obIndex].month = timestamp.mon;
    this->_observations[this->_obIndex].year = timestamp.year;
    this->_observations[this->_obIndex].hour = timestamp.hour;
    this->_observations[this->_obIndex].minute = timestamp.min;
    this->_observations[this->_obIndex].second = timestamp.sec;
    this->_obIndex++;
  }
  this->_queueProcessed = true;
}

/**
 * Calculates mean temperature and humidity from all observed data
 */
void UefsWS_Manager::calculateMean() {
  uint8_t processedSamples = 0;
  this->_tempMean = this->_humiMean = 0;
  for(uint8_t i = 0; i < SAMPLES; i++) {
    if(this->_samples[i].temperature != 0) {
      processedSamples++;
    }
    this->_tempMean += this->_samples[i].temperature;
    this->_humiMean += this->_samples[i].humidity;
  }
  this->_tempMean /= processedSamples;
  this->_humiMean /= processedSamples;
}

/**
 * Sends mean temperature and humidity observed in the last 1h
 */
void UefsWS_Manager::sendObservations() {  
  if(DEBUG_MODE) {
    Serial.println("Preparing to send observations");
    Serial.println();
  }
  for(int i = this->_obIndex - 1; i >= 0; i--) {
    sprintf(this->_http.methodResource, "POST %sobservation/%s/%i/%i/%i-%02i-%02iT%02i:%02i:%02i", 
    API, STATION, this->_observations[i].temperature, this->_observations[i].humidity,
    this->_observations[i].year, this->_observations[i].month, this->_observations[i].day,
    this->_observations[i].hour, this->_observations[i].minute, this->_observations[i].second);
    if(this->_http.request()) {
      this->_obIndex--;
    } else {
      return;
    }
  }
}

/**
 * Stores observation in Arduino's memory.
 */
void UefsWS_Manager::storeSample() {
  this->_lastMeasureTime = millis();
  this->_samples[this->_saIndex].temperature = this->_dht.readTemperature();
  this->_samples[this->_saIndex].humidity = this->_dht.readHumidity();
  if(DEBUG_MODE) {
    Serial.print("Storing ");
    Serial.println(this->_saIndex);
    Serial.print("T:");
    Serial.println(this->_samples[this->_saIndex].temperature);
    Serial.print("H:");
    Serial.println(this->_samples[this->_saIndex].humidity);
  }
  this->_saIndex++;
}
