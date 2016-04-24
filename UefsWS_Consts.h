#ifndef UefsWS_Consts_h
#define UefsWS_Consts_h
#include <UIPEthernet.h>
/** 
 * DHT11 configuration
 */
#define DHT_PIN 8                   // You can change this pin if you want
#define DHT_TYPE DHT11              // Set here the DHT type 11 or 22
/** 
 * 52n Util service configuration
 */
#define SERVER "192.168.100.4"      // IP or name to a 52n Util server
#define PORT 8080                   // Port running your application server
#define API "/52n-sos-util/api/v1/" // Path of 52n Util API
#define STATION "station-uefs-uefs" // It needs to be unique in 52n Util server instance
#define SEND_INTERVAL 3600000       // Set a interval time to send observations made
#define SAMPLES 4                   // Set number of observations to be made within SEND_INTERVAL (the mean value will be sent)
#define OBSERVATION_CACHE 5         // Set number of observations stored to future sending (hours of cache)
#define DEBUG_MODE false            // Set true to have Serial output of all processing
// Ethernet Module w/ ENC28J60
const byte UefsWS_mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};  // It needs to be unique on your local network
const IPAddress UefsWS_staticIp(192, 168, 100, 200);             // IP address
const IPAddress UefsWS_staticGtw(192, 168, 100, 1);              // Gateway
const IPAddress UefsWS_staticSubnet(255, 255, 255, 0);           // Subnet Mask
const IPAddress UefsWS_staticDns(8, 8, 8, 8);                    // DNS
#endif
