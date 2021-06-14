/*
   This program scans kaywinnet every few seconds and publishes the RSSI value n MQTT.
*/

//--------------- WiFi declarations ---------------
#define SKETCH_NAME "WiFiScan2"
#define SKETCH_VERSION "1.0"
#include <ESP8266WiFi.h>        // Not needed if also using the Arduino OTA Library...
#include <Kaywinnet.h>          // WiFi credentials
char macBuffer[24];             // Holds the last three digits of the MAC, in hex.
char hostName[24];              // Holds nodeName + the last three bytes of the MAC address.



//--------------- MQTT declarations ---------------
#include <ESP8266WiFi.h>        // Connect (and reconnect) an ESP8266 to the a WiFi network.
#include <PubSubClient.h>       // connect to a MQTT broker and publish/subscribe messages in topics.
// Declare an object of class WiFiClient, which allows to establish a connection to a specific IP and port
// Declare an object of class PubSubClient, which receives as input of the constructor the previously defined WiFiClient.
// The constructor MUST be unique on the network. (Does it?)
WiFiClient xyzzyz;
PubSubClient client(xyzzyz);

// Declare strings for the topics. Topics will be created in setup_mqtt().
char statusTopic[20];
char cmndTopic[20];                           // Incoming commands, payload is a command.
// Other topics as needed
char rssiTopic[20];

const char *mqttServer = MQTT_SERVER;         // Local broker defined in Kaywinnet.h
const int mqttPort = 1883;
char nodeName[] = SKETCH_NAME;                // Give this node a name




//--------------- OTA declarations ---------------
#include <ArduinoOTA.h>





//========================= setup =========================
void setup() {
  beginSerial();
  setup_wifi();                   // MUST be before setupMqtt()
  start_OTA();
  setup_mqtt();                   // Generate the topics

  // Call the setServer method on the PubSubClient object
  client.setServer(mqttServer, mqttPort);
  mqttConnect();
}


//========================= loop =========================
void loop() {
  ArduinoOTA.handle();
  mqttReconnect();         //Make sure we stay connected to the mqtt broker

  long myRssi;
  String rssi_str;
  char temp[50];

  myRssi = WiFi.RSSI();                               //RSSI rturns a long int.
  rssi_str = String(myRssi);                          //Convert myRssi to a string
  rssi_str.toCharArray(temp, rssi_str.length() + 1);  //packaging up the data to publish to mqtt

  Serial.print(F("RSSI: "));
  Serial.println(temp);
  client.publish(rssiTopic, temp);
  // Wait a bit before scanning again
  delay(5000);
}
