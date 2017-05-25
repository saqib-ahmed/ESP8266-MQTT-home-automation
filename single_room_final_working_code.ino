/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

All GPIO's at output are used in sink mode so HIGH means OFF and LOW means ON. It is due to HIGH pulse at device startup on some pins. Similarly 
for states(st1,st2 etc.) 0 means ON and 1 Means OFF 
Only Status LED(at GPIO2) is used in source mode bcoz builtin LED(on normally) is attached to it which is in sink mode.  
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <EEPROM.h>

//defining pins for inputs and outputs
#define LED1 16
#define LED2 5
#define LED3 4
#define LED4 0
#define sw1 14 
#define sw2 12
#define sw3 13
#define sw4 15
#define status_LED 2

//initially both current state and previous state are set high(outputs off). 
//At startup data is loaded from EEPROM so states are changed accordingly,
//whenever state is changed the current states are published,
//both initial states are declared same because puplishing is required whenever ON state is read from EEPROM
  
volatile byte st1 = HIGH;      // current state of LED1 (used in function for state change)
byte pr_st1 = HIGH;           // previous state of LED1 (used in function for state change)
volatile byte st2 = HIGH;      // current state of LED2 (used in function for state change)
byte pr_st2 = HIGH;           // previous state of LED2 (used in function for state change)
volatile byte st3 = HIGH;      // current state of LED3 (used in function for state change)
byte pr_st3 = HIGH;           // previous state of LED3 (used in function for state change)
volatile byte st4 = HIGH;      // current state of LED4 (used in function for state change)
byte pr_st4 = HIGH;           // previous state of LED4 (used in function for state change)
char *pointer_to_created_string;
char msgBuffer[1]; 

// Update these with values suitable for your network.
const char* mqtt_server = "broker.mqtt-dashboard.com"; //adress of broker

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup_wifi() {
// WPS works in STA (Station mode) only.
  WiFi.mode(WIFI_STA);
  delay(1000);
  // Called to check if SSID and password has already been stored by previous WPS call.
  // The SSID and password are stored in flash memory and will survive a full power cycle.
  // Calling ("",""), i.e. with blank string parameters, appears to use these stored values.
  WiFi.begin("","");
  // Long delay required especially soon after power on.
  delay(4000);
  // Check if WiFi is already connected and if not, begin the WPS process. 
  while(WiFi.status() != WL_CONNECTED){
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("\nAttempting connection ...");
      WiFi.beginWPSConfig();
      // Another long delay required.
      delay(3000);
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println("Connected!");
        Serial.println(WiFi.localIP());
        Serial.println(WiFi.SSID());
        Serial.println(WiFi.macAddress());
        }
      else {
        Serial.println("Connection failed!");
        }
      }
    else {
    Serial.println("\nConnection already established.");
    }

  }
}

void callback(char* topic, byte* payload, unsigned int length) {

  // to check if switch for LED1 on Mobile Phone App is operated 
  if(strcmp(topic,"EEL/client_001/LED1_A") == 0)
  {
    // Switch on the LED if an 1 was received as first character against LED1
    if ((char)payload[0] == '1') 
    {
      //turning on LED1
      digitalWrite(LED1, LOW);   
      //state is also changed, Publishing is done in state change function
      st1 = LOW;
      EEPROM.write(0x01, st1); //Stores the state of LED1 if switched from Mobile Phone App
      EEPROM.commit();    //Store data to EEPROM
    } 
    else 
    {
      //turning off LED1
      digitalWrite(LED1, HIGH);  
      st1 = HIGH;
      EEPROM.write(0x01, st1);   
      EEPROM.commit();    //Store data to EEPROM
    }
  }

  // to check if switch for LED2 on Mobile Phone App is operated 
  if(strcmp(topic,"EEL/client_001/LED2_A") == 0)
  {
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') 
    {
      digitalWrite(LED2, LOW);   
      st2 = LOW;
      EEPROM.write(0x02, st2); //Stores the state of LED1 if switched from Mobile Phone App
      EEPROM.commit();    //Store data to EEPROM
    } 
    else 
    {
      digitalWrite(LED2, HIGH);  
      st2 = HIGH;
      EEPROM.write(0x02, st2);   
      EEPROM.commit();    //Store data to EEPROM
    }
  }

   // to check if switch for LED3 on Mobile Phone App is operated 
  if(strcmp(topic,"EEL/client_001/LED3_A") == 0)
  {
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') 
    {
      digitalWrite(LED3, LOW);   
      st3 = LOW;
      EEPROM.write(0x03, st3); //Stores the state of LED1 if switched from Mobile Phone App
      EEPROM.commit();    //Store data to EEPROM
    } 
    else 
    {
      digitalWrite(LED3, HIGH);  
      st3 = HIGH;
      EEPROM.write(0x03, st3);   
      EEPROM.commit();    //Store data to EEPROM
    }
  }

   // to check if switch for LED4 on Mobile Phone App is operated 
  if(strcmp(topic,"EEL/client_001/LED4_A") == 0)
  {
    // Switch on the LED if an 1 was received as first character
    if ((char)payload[0] == '1') 
    {
      digitalWrite(LED4, LOW);   
      st4 = LOW;
      EEPROM.write(0x04, st4); //Stores the state of LED1 if switched from Mobile Phone App
      EEPROM.commit();    //Store data to EEPROM
    } 
    else 
    {
      digitalWrite(LED4, HIGH);  
      st4 = HIGH;
      EEPROM.write(0x04, st4);   
      EEPROM.commit();    //Store data to EEPROM
    }
  }
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) 
    {
      Serial.println("connected");
      digitalWrite(status_LED,HIGH);
      client.subscribe("EEL/client_001/#"); 
      //pointer_to_created_string = dtostrf(!st1, 1, 0, msgBuffer);
      //client.publish("EEL/client_001/LED1", pointer_to_created_string,true);
    } 
    
    else 
      {
        digitalWrite(status_LED,LOW);
        Serial.print("failed, rc=");
        Serial.print(client.state());
        Serial.println(" try again in 5 seconds");
        // Wait 5 seconds before retrying
        delay(5000);
      }
  }
}

void setup() {
  //declaring output pins
  pinMode(LED1, OUTPUT);  
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);  
  pinMode(LED4, OUTPUT);  
  //declaring input pins
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
  pinMode(sw3, INPUT);
  pinMode(sw4, INPUT);
  pinMode(status_LED, OUTPUT);
  //status LED should be red at start
  digitalWrite(status_LED,LOW);
  //making all switch inputs as interupt
  attachInterrupt(sw1,toggle1,HIGH);
  attachInterrupt(sw2,toggle2,HIGH);
  attachInterrupt(sw3,toggle3,HIGH);
  attachInterrupt(sw4,toggle4,HIGH);
  EEPROM.begin(512);  //Initialize EEPROM
  
  //Reading EEPROM for LED1 sate
  if(byte(EEPROM.read(0x01))==0){
  digitalWrite(LED1,LOW);
  st1=LOW;}
  else{
  digitalWrite(LED1,HIGH);
  st1=HIGH;}

  //Reading EEPROM for LED2 sate
  if(byte(EEPROM.read(0x02))==0){
  digitalWrite(LED2,LOW);
  st2=LOW;}
  else{
  digitalWrite(LED2,HIGH);
  st2=HIGH;}

  //Reading EEPROM for LED3 sate
  if(byte(EEPROM.read(0x03))==0){
  digitalWrite(LED3,LOW);
  st3=LOW;}
  else{
  digitalWrite(LED3,HIGH);
  st3=HIGH;}

  //Reading EEPROM for LED4 sate
  if(byte(EEPROM.read(0x04))==0){
  digitalWrite(LED4,LOW);
  st4=LOW;}
  else{
  digitalWrite(LED4,HIGH);
  st4=HIGH;}
    
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) 
    {
      reconnect();
    }
  
  client.loop();

  //Publish the state of LED1 if changed
  if(state_change1() == true){
     if(st1 == LOW){
      client.publish("EEL/client_001/LED1", "1",true);
      Serial.println("Publishing LED1 is on");
     }
     else{
      client.publish("EEL/client_001/LED1", "0",true);
      Serial.println("Publishing LED1 is off");
    }    
  }

  //Publish the state of LED2 if changed
  if(state_change2() == true){
     if(st2 == LOW){
      client.publish("EEL/client_001/LED2", "1",true);
      Serial.println("Publishing LED2 is on");
     }
     else{
      client.publish("EEL/client_001/LED2", "0",true);
      Serial.println("Publishing LED2 is off");
    }    
  }

  //Publish the state of LED3 if changed
  if(state_change3() == true){
     if(st3 == LOW){
      client.publish("EEL/client_001/LED3", "1",true);
      Serial.println("Publishing LED3 is on");
     }
     else{
      client.publish("EEL/client_001/LED3", "0",true);
      Serial.println("Publishing LED3 is off");
    }    
  }

  //Publish the state of LED4 if changed
  if(state_change4() == true){
     if(st4 == LOW){
      client.publish("EEL/client_001/LED4", "1",true);
      Serial.println("Publishing LED4 is on");
     }
     else{
      client.publish("EEL/client_001/LED4", "0",true);
      Serial.println("Publishing LED4 is off");
    }    
  }
  
}

//Function for Interupt for sw1
void toggle1(){
  st1 = !st1;
  digitalWrite(LED1, st1);
  EEPROM.write(0x01, st1); //Stores the state of LED1 if sw1 is operated 
  EEPROM.commit();    //Store data to EEPROM
  }

//Function to check state change of LED1
boolean state_change1(){
     boolean change; 
     if(st1 != pr_st1)
      change=true;
     else
      change=false;
     pr_st1=st1;
     return change;
    }

//Function for Interupt for sw2
void toggle2(){
  st2 = !st2;
  digitalWrite(LED2, st2);
  EEPROM.write(0x02, st2); //Stores the state of LED2 if sw2 is operated 
  EEPROM.commit();    //Store data to EEPROM
  }

//Function to check state change of LED2
boolean state_change2(){
     boolean change; 
     if(st2 != pr_st2)
      change=true;
     else
      change=false;
     pr_st2=st2;
     return change;
    }

//Function for Interupt for sw3
void toggle3(){
  st3 = !st3;
  digitalWrite(LED3, st3);
  EEPROM.write(0x03, st3); //Stores the state of LED3 if sw3 is operated 
  EEPROM.commit();    //Store data to EEPROM
  }

//Function to check state change of LED3
boolean state_change3(){
     boolean change; 
     if(st3 != pr_st3)
      change=true;
     else
      change=false;
     pr_st3=st3;
     return change;
    }

//Function for Interupt for sw4
void toggle4(){
  st4 = !st4;
  digitalWrite(LED4, st4);
  EEPROM.write(0x04, st4); //Stores the state of LED4 if sw4 is operated 
  EEPROM.commit();    //Store data to EEPROM
  }

//Function to check state change of LED4
boolean state_change4(){
     boolean change; 
     if(st4 != pr_st4)
      change=true;
     else
      change=false;
     pr_st4=st4;
     return change;
    }
