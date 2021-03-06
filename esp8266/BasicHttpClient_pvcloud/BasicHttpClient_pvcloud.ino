#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

void setup() {

    USE_SERIAL.begin(115200);
    // USE_SERIAL.setDebugOutput(true);

    pinMode(0, INPUT);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for (uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("r2d2", "c3po1234");

}

void loop() {
    // wait for WiFi connection
    if ((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        //http.begin("https://192.168.1.12/test.html", "7a 9c f4 db 40 d3 62 5a 6e 21 bc 5c cc 66 c8 3e a1 45 59 38"); //HTTPS
        //http.begin("http://costaricamakers.com/pvcloud/backend/vse.php/appdata_write_get/108/1aff2bb68fd9039c2d3728bda8319c032015dac7/ESP8266_TEST/HELLO"); //HTTP

        int val = 0;
        val = digitalRead(0);

        if (val == HIGH) {
            http.begin("https://costaricamakers.com/pvcloud/backend/vse.php/appdata_write_get/86/dc0e291b03d34582d63bddbc0a5b98ee5bea913c/GATO_COMIO/1", "C2 6A 62 96 90 DD 5A 68 FA 9E 26 34 83 84 6C EE D5 92 FC 5E"); //HTTPS
            USE_SERIAL.print("[HTTP] GET...\n");
            // start connection and send HTTP header
            int httpCode = http.GET();

            // httpCode will be negative on error
            if (httpCode > 0) {
                // HTTP header has been send and Server response header has been handled
                USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

                // file found at server
                if (httpCode == HTTP_CODE_OK) {
                    String payload = http.getString();
                    USE_SERIAL.println(payload);
                }
            } else {
                USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            }

            http.end();

            delay(5000);
        }
        delay (500);
    }

    
}

