/**
 * UefsWS.ino - SWE ready weather station to measure temperature and humidity
 * 
 * Author: Natanael Simoes <natanael.simoes@ifro.edu.br> http://github.com/natanaelsimoes/
 * Master degree in Applied Computing at State University of Feira de Santana (UEFS) http://uefs.br/
 * 
 * Each 15m a new sample is measured. After collecting 4 samples (1h), mean is calculated then sent to a 52n Util server.
 * 
 * This station uses some modules to achieve the proposed goal:
 * - Ethernet Module w/ ENC28J60
 * - RTC Module w/ DS3231
 * - Adafruit DHT11
 * 
 * !WARNING! STATION constant is a station identificator that needs to be unique in 52n Util Server.
 *           It has a limit of 23 characters due to Arduino memory limitations.
 */
#include "UefsWS_Manager.h"

UefsWS_Manager manager;

void setup() {
  manager.setup();
}

/**
 * Grants HTTP response processing, time syncronization (if not done at setup())
 * and controls observation data storing and sending
 */
void loop() {
  manager.loop();
}
