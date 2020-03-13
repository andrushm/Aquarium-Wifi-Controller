#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <Scheduler.h>

#ifndef STASSID
#define STASSID "pier2m"
#define STAPSK  "g00dwork"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;
const char* mqtt_server = "m23.cloudmqtt.com";
const char* mqtt_server_user = "wwvrzbtp";
const char* mqtt_server_pass = "2LS9dJ3HEXN7";

const char* pumpTopic = "pump";
const char* lampTopic = "lamp";
const char* lampLevelTopic = "lamp-level";
const char* timeOnTopic = "timeOn";
const char* timeOffTopic = "timeOff";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

Scheduler scheduler(D6);


int lampStatus = LOW;
int lampLevel = 50;
int pumpStatus = LOW;
time_t now = 0;
time_t lastTime = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char cPayload[12];
  TIME setTime;

  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    cPayload[i] = (char)payload[i];
  }
  Serial.println();
  
  String topicStr(topic);
  String payloadStr(cPayload);
  // cPayload = (char)&payload[0];
// Serial.print(payloadStr.toInt());
  // if (topicStr.indexOf("pump")) {
  if (strcmp(topic, pumpTopic) == 0) {
    
    pumpStatus = payloadStr.toInt();
    // if (cPayload == '1') {
      // pumpStatus = 1;
    // } else if (cPayload == '2') {
      // pumpStatus = 0;
    // } else {
      // pumpStatus = 0;
    // }
    // client.publish("pump", (char)pumpStatus);
    // digitalWrite(D7, pumpStatus);
    Serial.print(pumpStatus);
  }

  if (strcmp(topic, lampTopic) == 0) {
  //   // client.publish(lampTopic, lampStatus);
    lampStatus = payloadStr.toInt();
    Serial.print(lampStatus);
  }

  if (strcmp(topic, lampLevelTopic) == 0) {
    lampLevel = payloadStr.toInt();
    analogWrite(D7, lampLevel);
    Serial.print(lampLevel);
  }

  if (strcmp(topic, timeOnTopic) == 0) {
    setTime.hour = payloadStr.substring(0,1).toInt();
    setTime.mins = payloadStr.substring(2,3).toInt();
    scheduler.setTimeOn(setTime);
  }

  if (strcmp(topic, timeOffTopic) == 0) {
    setTime.hour = payloadStr.substring(0,1).toInt();
    setTime.mins = payloadStr.substring(2,3).toInt();
    scheduler.setTimeOff(setTime);
  }



  // Switch on the LED if an 1 was received as first character
  // if ((char)payload[0] == '1') {
    // digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  // } else {
    // digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  // }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), mqtt_server_user, mqtt_server_pass)) {
      Serial.println("connected");
      // Once connected, publish an announcement...3
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("pump");    
      client.subscribe("lamp");
      client.subscribe("lamp-level");
      client.subscribe("timeOn");
      client.subscribe("timeOff");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void setup() {
  pinMode(D7, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  digitalWrite(D7, LOW); 
  Serial.begin(9600);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }


  // Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  // ArduinoOTA.setHostname("myesp8266");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }

    // NOTE: if updating FS this would be the place to unmount FS using FS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Hi");

  // timezone dst
  configTime(2 * 3600, 0 * 0, "pool.ntp.org", "time.nist.gov");

  int MAX_TIME_RETRY = 60;
	int i = 0;
  // wait and determine if we have a valid time from the network. 
	now = time(nullptr);
	i = 0;
	Serial.print("Waiting for network time.");
	while (now <= 1500000000) {
		Serial.print(".");
		delay(1000); // allow a few seconds to connect to network time.
		i++;
		now = time(nullptr);
		if (i > MAX_TIME_RETRY) {
			Serial.println("Gave up waiting for network time(nullptr) to have a valid value.");
			break;
		}
	}
	Serial.println("");

  // if (client.connect("arduinoClient", "testuser", "testpass")) {
    // client.publish("outTopic","hello world");
    // client.subscribe("inTopic");
  // }

  client.setServer(mqtt_server, 11271); // 1883
  client.setCallback(callback);

  Serial.println("scheduler.Test");
  Serial.println(scheduler.Test());
}

void loop() {
  ArduinoOTA.handle();

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

now = time(nullptr);
// int nowHour = hour(now);
// if ((now - lastTime) > 60) {
  
scheduler.tick();

int offset = now % 3600;
int mins = offset / 60;
if (lastTime != mins) {
  lastTime = mins;
  // int dayOff = now % 86400;
  // int day = dayOff / 7;
  // Serial.println(dayOff);
  // Serial.println(day);
  // scheduler.tick();
  Serial.println(now);
  client.publish("time",ctime(&now));
  // Serial.println(scheduler.Test());
}

// delay(1000);
  // long now = millis();
  // if (now - lastMsg > 2000) {
  //   lastMsg = now;
  //   ++value;
  //   snprintf (msg, 50, "hello world #%ld", value);
  //   Serial.print("Publish message: ");
  //   Serial.println(msg);
  //   client.publish("outTopic", msg);
  // }
}



// #include <JeVe_EasyOTA.h> 

// #define WIFI_SSID        "pier2m"
// #define WIFI_PASSWORD    "g00dwork"
// #define ARDUINO_HOSTNAME "ota-aquarium"
// EasyOTA OTA(ARDUINO_HOSTNAME);

// void setup() {
  // put your setup code here, to run once:
  // Serial.begin(9600);
  // This callback will be called when EasyOTA has anything to tell you.
  // OTA.onMessage([](char *message, int line) {
    // Serial.println(message);
  // });
  // OTA.addAP(WIFI_SSID, WIFI_PASSWORD);
// }

// void loop() {
  // put your main code here, to run repeatedly:
  // OTA.loop();
// }
