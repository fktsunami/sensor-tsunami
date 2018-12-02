/*
 Name:		AWS.ino
 Created:	12/1/2018 9:24:37 AM
 Author:	longn
*/


#include <SPI.h>
#include <Ethernet.h>
//#include <SoftReset.h>
#include <PubSubClient.h> 
#include <Wire.h> 
#include <ArduinoJson.h>


// Set these to run example.
#define FIREBASE_HOST "example.firebaseio.com"
#define FIREBASE_AUTH "token_or_secret"

//Comment to disable debug output
#define GENERAL_DEBUG

#ifdef GENERAL_DEBUG
#define DEBUG_PRINT(string) (Serial.println(string))
#endif

#ifndef GENERAL_DEBUG
#define DEBUG_PRINT(String)
#endif



/************************* Ethernet Client Setup *****************************/
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0D, 0x1F, 0x48 };

//Uncomment the following, and set to a valid ip if you don't have dhcp available.
//IPAddress ip(192, 168, 100, 99);
//If you uncommented either of the above lines, make sure to change "Ethernet.begin(mac)" to "Ethernet.begin(mac, iotIP)" or "Ethernet.begin(mac, iotIP, dnsIP)"

char myIPAddress[20];


/************************* MQTT Client Config ********************************/


const char* mqttserver = "192.168.1.50";               // Local Broker
const int mqttport = 1883;                             // MQTT port
String subscriptionTopic = "awsiot_to_localgateway";   // Get messages from AWS
String publishTopic = "localgateway_to_awsiot";        // Send messages to AWS 


/************************* MQTT Connection Monitoring ************************/

long connecionMonitoringFrequency = 10000UL;  // (1000 = 1 sec)
unsigned long lastConnectionCheck = 0;
int connectionAttempt = 0;
const int maxconnectionAttempt = 5;           // Reset after x connection attempt
const int maxConnectionCycles = 100;//100;    // Security Reset (Soft Reet)
const int maxCiclosDiarios = 5000;//500;      // Full Reset
int ConnectionCyclesCount = 0;
int totalCyclesCount = 0;


/********************* MQTT Broker Callback Function **************************
 *
 *  Process MQTT Messages (subscription)
 *
 *  Parameters
 *  topic - the topic the message arrived on (const char[])
 *  payload - the message payload (byte array)
 *  length - the length of the message payload (unsigned int)
 */



/************************** PubSubClient Declaration *******************************/


EthernetClient client;
PubSubClient mqttclient(mqttserver, mqttport, callback, client);   //Callback function must be declared before this line


/************************** Sketch Code ********************************************/

void setup() {


#if defined(GENERAL_DEBUG)
	Serial.begin(9600);
#endif

	//Configure mqttClient
	mqttclient.setServer(mqttserver, mqttport);
	mqttclient.setCallback(callback);

	// start the Ethernet connection:
	Ethernet.begin(mac);

	DEBUG_PRINT("<SETUP> : Board IP = " + (String)getIpReadable(Ethernet.localIP()));

	// Connect to Broker
	if (reconnect()) {
		DEBUG_PRINT("<SETUP> : Connected to broker");
	}
	else {
		DEBUG_PRINT("<SETUP> : Initial Connection Failed");
	}

	DEBUG_PRINT("<SETUP> : End Config..");

}

void loop() {

	//Check connection Status
	if (millis() - lastConnectionCheck > connecionMonitoringFrequency) {
		lastConnectionCheck = millis();
		CheckConnection();
	}

	mqttclient.loop();  //end cycle

}


