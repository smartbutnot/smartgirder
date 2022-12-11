
#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <NTPClient_Generic.h>
#include <Timezone_Generic.h>

#include "secrets.h"

//MQTT and WiFi clients
WiFiClient espClient;
PubSubClient client(espClient);
int WiFiStatus = WL_IDLE_STATUS;      // the Wifi radio's status

//NTP setup
TimeChangeRule myDST  = {"BST", Last, Sun, Mar, 1, 60};        // British Summer Time
TimeChangeRule mySTD = {"GMT", Last, Sun, Oct, 2, 0};         // Standard Time
Timezone *myTZ;
TimeChangeRule *tcr;        // pointer to the time change rule, use to get TZ abbrev
WiFiUDP ntpUDP;
//NTP update every 5 mins 5.5 secs
NTPClient timeClient(ntpUDP,ntp_server,0,305500);

#define NTP_DBG_PORT                Serial
#define _NTP_LOGLEVEL_              4


//Place each panel on the chain - panels are 64 wide, we expect 4
const int tflOffset=0;
const int networkOffset=64;
const int dashOffset=128;
const int timerOffset=tflOffset;
const int weatherOffset=192;
const int clockOffset=192;

//Various vars for main functions
boolean high=true;
boolean timerMode=false;
unsigned long timerEpoch;
long ntpForce;
String clientId = "Girder-";
boolean forceRefresh=true;
int graphRotateDelay = 10;

//Control brightness by adjusting RGB values. Brightness is a percentage so brightness=50 turns 250,250,250 into 125,125,125
const int dimHour=22;
const int brightHour=7;
const int dimBrightness=20;
const int brightBrightness=100;
int brightness=dimBrightness;

#include "display.h"
#include "icons.h"
#include "weather.h"
#include "network.h"
#include "transport.h"
#include "dashboard.h"
#include "topichandler.h"
#include "datetime.h"


//Connect to a local MQTT server that provides all the data
void mqttConnect() {

  int mq_retries=1;

  while (!client.connected()) {

    Serial.print("Connect client ");
    Serial.println(clientId);

    //If we've already tried to connect and failed, bounce the WiFi. After that reconnect it every 10 times we fail to connect to MQTT.
    if((mq_retries==2) || ((mq_retries % 10) == 0)){
      WiFi.disconnect();
      WiFi.end();
      WiFiStatus = WL_IDLE_STATUS;
      wifiConnect();
    }

    Serial.print("MQTT connect attempt ");
    Serial.println(mq_retries);
  
    delay(10000);
    
    mq_retries+=1;

    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("sign/#");
      client.subscribe("network/#");
      client.subscribe("TFL/#");
      client.subscribe("weather/#");
      client.subscribe("alexa/#");
      client.subscribe("car/#");
      client.subscribe("planes/#");
      client.subscribe("thermostat/#");
      client.subscribe("plants/#");
      client.subscribe("meater/#");
      
    }else {
      Serial.print("MQTT connection failed, attempt ");
      Serial.print(mq_retries);
      Serial.print(", rc=");
      Serial.println(client.state());
    }
  }
}

void wifiConnect(){
  while (WiFiStatus != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    WiFiStatus = WiFi.begin(ssid, pass);
    delay(10000);
  }
}

void setup() {



  //Turn the LED on the matrixportal red while we're setting up
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);

  Serial.begin(115200);

  timeClient.setUpdateCallback(*NTPcallback);
  timeClient.setRetryInterval(60000);

  //Set an MQTT client name of "Girder-[randomnumber]" to avoid clashes with multiple devices
  randomSeed(analogRead(0));
  MCLK->APBCMASK.bit.TRNG_ = 1; // Enable Main Clock (MCLK) for TRNG
  TRNG->CTRLA.bit.ENABLE = 1; // Enable TRNG
  TRNG->INTENSET.bit.DATARDY = 1; // Enable TRNG interrupt when data ready
  delay(10); // Short delay to allow generation of new random number
  int rand_num = TRNG->DATA.reg;
  clientId += String(rand_num);

  setupDisplay();
  displayDashboard();

  //Wifi
  wifiConnect();
  String wfv = WiFi.firmwareVersion();
  Serial.print(F("\nBoard ")); Serial.print(BOARD_NAME);
  Serial.print(F(" with ")); Serial.print(NTPCLIENT_GENERIC_VERSION);
  Serial.print(F(" and WiFi firmware ")); Serial.println(wfv);
  
  if (wfv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.print("Newer WiFi firmware available: ");
    Serial.println(WIFI_FIRMWARE_LATEST_VERSION);
  }
  printWifiData();

  //MQTT
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);  
  mqttConnect();

  //Time
  myTZ = new Timezone(myDST, mySTD);
  timeClient.begin();
  
  brightness=brightBrightness;

  ntpForce=now()+3600;;

}

void loop() {

  

  //Check we're on WiFi and have an MQTT connection, reconnect if not
  wifiConnect();
  mqttConnect();

  //We'll use this to check various timeouts
  long nowSecs = now();

  //MQTT loop to pick up messages
  client.loop();



  //Replace the TFL panel with a countdown if we pick up an Alexa timer
  if(timerMode){
    displayTimer();
  }
  //Show a flight if one is overhead
  if (showFlight){
    displayFlight(nowSecs);
  }
  //Show a vague car location if we're not showing a flight and it's away from home
  if (showCar and not showFlight){
    displayCarLocation();
  }

  //Show the network graph and rotate it as needed
  displayNetworkGraph(nowSecs);

  //Show the clock and update it as needed
  displayClock();

  //If something requested a refresh, update the slower elements of the display as well
  if (forceRefresh){
    displayTFL();
    displayNetworkStats();
    displayDashboard();
    displayWeather();
    forceRefresh=false;
  }

  //Update the NTP client (this doesn't do anything unless we've hit the setUpdateInterval time)
  timeClient.update();

  //if (nowSecs>ntpForce){
  //  timeClient.forceUpdate();
  //  display.fillRect(0, 0, 256, 32, display.color565(0, 0, 0));
  //  display.fillRect(0, 0, 5, 5, display.color565(255, 0, 0));
  //  delay(10000); 
  //  WiFi.disconnect();
  //  wifiConnect();
  //  mqttConnect();
  //  timeClient.begin();
  //  ntpForce=nowSecs+3600;
  //}
  
  
  //sync_clock();
  //delay(1000);
}

void printWifiData() {
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
}
