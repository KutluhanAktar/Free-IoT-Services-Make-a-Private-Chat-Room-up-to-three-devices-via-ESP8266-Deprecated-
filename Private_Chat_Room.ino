         /////////////////////////////////////////////  
        // IoT Make a Private Chat Room for Three  //
       //         Devices via ESP8266             //
      //       -------------------------         //
     //           NodeMCU (ESP-12E)             //           
    //           by Kutluhan Aktar             // 
   //                                         //
  /////////////////////////////////////////////

// By only subscribing to TheAmplituhedron, you will be able get your data packets from NodeMCU, or any other device that can make an HTTP GET Request, to your IoT Dashboard on your account page.
// TheAmplituhedron IoT Services is an available system for TheAmplituhedron.com subscribers only, and simple to use.
// Follow the steps down below to create and communicate your connection path on which you will display your data packets in every 2 seconds.
// 1) Go to your Dashboard.
// 2) Click Iot Services under Available Systems.
// 3) Read the given instructions and click Create+
// 4) Copy the url to use in the souce code.
// After creating your unique connection path, you can send data packets through NodeMCU by entering your WiFi settings and required information down below.
//
// As a reminder, my website has SSL protection so that you need to identify your NodeMCU connection by entering TheAmplituhedron FingerPrint or ThumbPrint.
// You can learn about it more from the link below.
// https://www.theamplituhedron.com/dashboard/IoTServices/
//
// Connections
// NodeMCU (ESP-12E) :           
//                                4x4 Keypad
// D0  --------------------------- L1
// D7  --------------------------- L2
// D6  --------------------------- L3
// D5  --------------------------- L4
// D4  --------------------------- R1
// D3  --------------------------- R2
// D2  --------------------------- R3
// D1  --------------------------- R4
// D8  --------------------------- S4(Send Button)
// VCC --------------------------- 3V
// GND --------------------------- GND


// Include required libraries to send data to a website, in this case TheAmplituhedron.com.
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

// Include KeyPad Library.
#include <Keypad.h>

// Define your WiFi settings.
const char *ssid = "WiFi_SSID";
const char *password = "WiFi_PASSWORD";

// Create data holders to send data packets.
String connectionPath, USERNAME, HEDRON, data_name_1, data_name_2, data_name_3, data_1, data_2, data_3;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
// Define the symbols on the buttons of the keypads to send a message.
// Of course, you can modify these symbols if you want to.
char hexaKeys[ROWS][COLS] = {
  {'H','E','L','O'},
  {'A','S','D','F'},
  {'X','C','V','R'},
  {'K','Y','U','T'}
};

// Define the row and column pinouts according to your keypad outputs.
#define L1 D0
#define L2 D7
#define L3 D6
#define L4 D5
#define R1 D4
#define R2 D3
#define R3 D2
#define R4 D1

// Define the send button to send the message string.
#define Send D8

// Connect to the row pinouts of the keypad
byte rowPins[ROWS] = {R4, R3, R2, R1}; 
// Connect to the column pinouts of the keypad
byte colPins[COLS] = {L4, L3, L2, L1};

// Initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// Define the message string.
String messageString;

void setup() {

  // Activate one of the control buttons on the keypad as the send button.
  pinMode(Send, INPUT);
 
  // Wait until connected.
  Serial.begin(115200);
 
  // It is just for assuring that if the connection is alive.
  WiFi.mode(WIFI_OFF);
  delay(1000);
  // This mode allows NodeMCU to connect any WiFi directly.
  WiFi.mode(WIFI_STA);        
  // Connect NodeMCU to your WiFi.
  WiFi.begin(ssid, password);
  
  Serial.print("\n\n");
  Serial.print("Try to connect to WiFi. Please wait! ");
  Serial.print("\n\n");
  // Halt the code until connected to WiFi.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("*");
  }
 
  // If connection is successful, write WiFi SSID to serial monitor along with assigned IPAddress.
  Serial.print("\n\n");
  Serial.print("-------------------------------------");
  Serial.print("\n\n");
  Serial.print("Connection is successful!");
  Serial.print("\n\n");
  Serial.print("Connected WiFi SSID : ");
  Serial.print(ssid);
  Serial.print("\n\n");
  Serial.println("Connected IPAddress : ");
  Serial.println(WiFi.localIP());
  Serial.print("\n\n");
  Serial.print("Enter your message fom the keypad to be sent :");
  Serial.print("\n\n");

  // Give time to ESP8266 for rebooting properly.
  delay(3000);

}

void loop() {
  // Initiate this function to collect symbols.
  get_message_string();
 
  // Send the collected string to the server.
  send_message_to_the_server();  

}


// Collect the entered symbols in messageString.
 void get_message_string(){
  // Define a data holder as character to save the last entered symbol.
  char customKey = customKeypad.getKey();
  // If a symbol is entered continue to collect.
   if(customKey){
     messageString += customKey;
     Serial.println(messageString);
   }
 }

// If the send button is pressed, send the message string to your channel through your communication path.
 void send_message_to_the_server(){
  // Do not forget to detach D8 when uploading the source code.
  int sendButton = digitalRead(Send);
  
  if(sendButton == 0){
    
    // Create HTTP object to make a request.
    HTTPClient http;    
 
    // Enter your connection path variables and define a name and a value for each data packet below - up to 6 entry.
    USERNAME = "Account_Username";
    HEDRON = "Account_Hedron";
    // You can change these names if you want to.
    data_name_1 = "Guest_1";
    data_name_2 = "Guest_2";
    data_name_3 = "Guest_3";
    data_1 = messageString;
    data_2 = "Waiting...";
    data_3 = "Waiting...";
  
    connectionPath = "https://www.theamplituhedron.com/dashboard/IoTServices/" + USERNAME + "-" + HEDRON + ".php?dataNameFirst=" + data_name_1 + "&dataNameSecond=" + data_name_2 + "&dataNameThird=" + data_name_3 + "&data1=" + data_1 + "&data2=" + data_2 + "&data3=" + data_3;
  
    http.begin(connectionPath, "xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx xx");
  
    int httpCode = http.GET();           
  
    // Test the page response by uncommenting it.
    //Serial.println(httpCode); 
 
    // End HTTP Get Request.
    http.end();

    // After sending, turn the message string into blank to send a new row.
    messageString = "";
    // Get notified when the module is ready to send the next message.
    Serial.println(" \n ------------------------------------------ \n Message is transferred! Enter a new message... \n ------------------------------------------ \n");
    
    }
 }

