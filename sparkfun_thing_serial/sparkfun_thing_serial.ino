// Include the ESP8266 WiFi library. (Works a lot like the
// Arduino WiFi library.)
#include <ESP8266WiFi.h>
// Include the SparkFun Phant library.
#include <WiFiClient.h>

//////////////////////
// WiFi Definitions //
//////////////////////
const char WiFiSSID[] = "opodiym";
const char WiFiPSK[] = "luaus7151";

/////////////////////
// Pin Definitions //
/////////////////////
const int LED_PIN = 5; // Thing's onboard, green LED
const int ANALOG_PIN = A0; // The only analog pin on the Thing
const int DIGITAL_PIN = 12; // Digital pin to be read

//////////////////
// pvCloud Data //
//////////////////

//http://costaricamakers.com/pvcloud_pre/backend/vse_2.php/48/4124ec4b83922b07c7e82069a1636ec19ba249c3/write/PRUEBA2/%7B%22p1%22:%22v1%22,%22p2%22:%22v2%22%7D
const char pvCloudURL[] = "pvcloud_pre/backend/vse_2.php/48/4124ec4b83922b07c7e82069a1636ec19ba249c3/write/ACC/";
// Remote site information
const char http_site[] = "costaricamakers.com";
const int http_port = 80;

/////////////////
// Post Timing //
/////////////////
const unsigned long postRate = 30000;
unsigned long lastPost = 0;
// Global variables
WiFiClient client;


void setup() 
{
  initHardware();
  connectWiFi();
  digitalWrite(LED_PIN, HIGH);
}

void loop() 
{

  
  String content = Serial.readString();
  if(content!=""){
    sendSerialData("I_HAVE_CONTENT");
    sendSerialData("SIZE: ");
    sendSerialData(String(content.length()));
    sendSerialData(content.substring(0,5));
  } else {
    sendSerialData("NO_CONTENT");
    delay(1000);
  }
  
  if (lastPost + postRate <= millis())
  {
    Serial.println("Sending Data...");
    if (getPage()){
      Serial.println("Data Sent Successfully!");
      lastPost = millis();
    } else {
      Serial.println("Not now... trying later...");
      delay(100);    
    }
  }
}

void connectWiFi()
{
  byte ledStatus = LOW;

  // Set WiFi mode to station (as opposed to AP or AP_STA)
  WiFi.mode(WIFI_STA);

  // WiFI.begin([ssid], [passkey]) initiates a WiFI connection
  // to the stated [ssid], using the [passkey] as a WPA, WPA2,
  // or WEP passphrase.
  WiFi.begin(WiFiSSID, WiFiPSK);

  // Use the WiFi.status() function to check if the ESP8266
  // is connected to a WiFi network.
  while (WiFi.status() != WL_CONNECTED)
  {
    // Blink the LED
    digitalWrite(LED_PIN, ledStatus); // Write LED high/low
    ledStatus = (ledStatus == HIGH) ? LOW : HIGH;

    // Delays allow the ESP8266 to perform critical tasks
    // defined outside of the sketch. These tasks include
    // setting up, and maintaining, a WiFi connection.
    delay(100);
    // Potentially infinite loops are generally dangerous.
    // Add delays -- allowing the processor to perform other
    // tasks -- wherever possible.
  }
}

void initHardware()
{
  Serial.begin(9600);
  pinMode(DIGITAL_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  // Don't need to set ANALOG_PIN as input, 
  // that's all it can be.
}

// Perform an HTTP GET request to a remote page
bool getPage() {
  
  // Attempt to make a connection to the remote server
  if ( !client.connect(http_site, http_port) ) {
    return false;
  }
  
  // Make an HTTP GET request
  client.println("GET /pvcloud_pre/backend/vse_2.php/48/4124ec4b83922b07c7e82069a1636ec19ba249c3/write/TEST/TESTVAL HTTP/1.1");
  client.print("Host: ");
  client.println(http_site);
  client.println("Connection: close");
  client.println();
  
  return true;
}

bool sendSerialData(String content) {
  
  // Attempt to make a connection to the remote server
  if ( !client.connect(http_site, http_port) ) {
    return false;
  }
  
  // Make an HTTP GET request
  client.print("GET /pvcloud_pre/backend/vse_2.php/48/4124ec4b83922b07c7e82069a1636ec19ba249c3/write/SERIAL_DATA/");
  client.print(content);
  client.println(" HTTP/1.1");
  client.print("Host: ");
  client.println(http_site);
  client.println("Connection: close");
  client.println();
  
  return true;
}
int sendTopvCloud(){
  
}

