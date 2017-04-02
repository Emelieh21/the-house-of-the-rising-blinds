// motion sensor connected to relayr cloud

//Libraries used: To be added manually on the Arduino IDE!
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


//WiFi credentials: 2.4GHz networks only!
//Avoid long SSIDs and passwords, and use preferably only letters and numbers
#define SSID "<YOUR_WIFI_NAME>"
#define PASSWORD "<YOUR_WIFI_PASSWORD>"


//Credentials from the developer dashboard
#define DEVICE_ID "<YOUR_DEVICE_ID>"
#define MQTT_USER "<YOUR_DEVICE_USER_NAME>"
#define MQTT_PASSWORD "<YOUR_PASSWORD>" 
#define MQTT_CLIENTID "<YOUR_CLIENT_ID>" //can be anything else
#define MQTT_TOPIC "<YOUR_TOPIC>"
#define MQTT_SERVER "mqtt.relayr.io"


//This creates the WiFi client and the pub-sub client instance
WiFiClient espClient;
PubSubClient client(espClient);

//Some definitions, including the publishing period
const int led = BUILTIN_LED; //If the LED doesn't work, try changing "BUILTIN_LED" for "D0" or "D4"
int ledState = LOW;


//Function prototypes
void setup_wifi();
void mqtt_connect();
void publish();


#define LUMINOSITY_SENSOR_PIN A0               // choose the input pin (for PIR sensor)
 
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);     // declare LED as output
  pinMode(LUMINOSITY_SENSOR_PIN, INPUT);     // declare sensor as input
  Serial.begin(9600);

  setup_wifi();
  client.setServer(MQTT_SERVER, 1883);

  //200ms is the minimum publishing period
  mqtt_connect();
}

//------------------------------------------------------------------------------------//
// This function connects to the WiFi network, and prints the current IP address      //
//------------------------------------------------------------------------------------//

void setup_wifi() {
  delay(10);
  
  //Trying to connect to the WiFi network
  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


//------------------------------------------------------------------------------------//
// This function establishes the connection with the MQTT server                      //
//------------------------------------------------------------------------------------//

void mqtt_connect()
{
  Serial.println("");
  Serial.println("Connecting to MQTT server...");
  
  if (client.connect(MQTT_CLIENTID, MQTT_USER, MQTT_PASSWORD))
  {
    Serial.println("Connection successful!");
  }

  else
  {
      Serial.println("Connection failed! Check your credentials or the WiFi network");
      //This reports the error code
      Serial.println("rc = ");
      Serial.print(client.state());
      //And it tries again in 5 seconds
      Serial.println("Retrying in 5 seconds...");
      delay(5000);
  }
}

//------------------------------------------------------------------------------------//
// This is the MAIN LOOP, it's repeated until the end of time! :)                     //
//------------------------------------------------------------------------------------//

void loop()
{
  //If we're connected, we can send data...
  if (client.connected())
  {
    client.loop();
    //Publish within the defined publishing period
            //Publishing...
            publish();
            delay(1000);
        }
  //If the connection is lost, then reconnect...
  else
  {
    Serial.println("Retrying...");
    mqtt_connect();
  }      

  //This function prevents the device from crashing
  //since it allows the ESP8266 background functions to be executed
  //(WiFi, TCP/IP stack, etc.)
  yield();
}




//------------------------------------------------------------------------------------//
// Publish function: What we want to send to the relayr Cloud                         //
//------------------------------------------------------------------------------------//

void publish()
{    
  //MQTT_MAX_PACKET_SIZE is defined in "PubSubClient.h", it's 128 bytes by default
  //A modified version with 512 bytes it's available here:
  //https://github.com/uberdriven/pubsubclient
  StaticJsonBuffer<MQTT_MAX_PACKET_SIZE> pubJsonBuffer;
  //Create our JsonArray
  JsonArray& root = pubJsonBuffer.createArray();
//----------------------------------------

//-------------------------------------------------  
  //Third object: presence
  JsonObject& leaf1 = root.createNestedObject();
  leaf1["meaning"] = "luminosity";
  leaf1["value"] = analogRead(LUMINOSITY_SENSOR_PIN);
//-------------------------------------------------
  
  char message_buff[MQTT_MAX_PACKET_SIZE];
  root.printTo(message_buff, sizeof(message_buff));
  client.publish("/v1/"DEVICE_ID"/data", message_buff);
  Serial.println("Publishing " + String(message_buff));
}
