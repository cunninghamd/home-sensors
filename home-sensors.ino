// esp includes
#include <ESP8266WiFi.h>
#include <EmonLib.h>

// display includes
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// config vars
#include "config.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// wifi networks
const char *homeWifi = configHomeWifi;
const char *homePassword = configHomePassword;
const char *officeWifi = configOfficeWifi;
const char *officePassword = configOfficePassword;
const char *remoteWifi = configRemoteWifi;
const char *remotePassword = configRemotePassword;

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const int sda = 4; // GPIO4 = D2
const int scl = 5; // GIPO5 = D1

void setup() {
    Serial.begin(115200); // open serial for debugging
    while (!Serial); // wait for serial device to attach

    Serial.println("display parameters:");
    Serial.printf("sda: %d\n", sda);
    Serial.printf("scl: %d\n", scl);
    setupDisplay(sda, scl);
    wifiConnecting();
    delay(500);

    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; i++) {
        if (WiFi.SSID(i) == homeWifi ) {
            wifiMessage(const_cast<char *>(homeWifi));
            WiFi.begin(homeWifi, homePassword);
            break;
        }
        if (WiFi.SSID(i) == officeWifi) {
            wifiMessage(const_cast<char *>(officeWifi));
            WiFi.begin(officeWifi, officePassword);
            break;
        }
        if (WiFi.SSID(i) == remoteWifi) {
            wifiMessage(const_cast<char *>(remoteWifi));
            WiFi.begin(remoteWifi, remotePassword);
            break;
        }
    }

    while(WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
}

void setupDisplay(int sda, int scl) {
    Wire.begin(sda, scl);
    
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // don't proceed, loop forever
    }
    display.clearDisplay();
    display.display(); // call .display() after all changes
}

void wifiConnecting() {
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.print("Connecting to WIFI:");
    display.display();
}

void wifiMessage(char *message) {
    display.setCursor(5, 30);
    display.print(message);
    display.display();

    delay(2000);
}

void loop() {
    // Serial.print(millis());
    // Serial.println(" looping");

    display.clearDisplay();
    display.setTextSize(4);
    display.setTextColor(WHITE);
    display.setCursor(3, 0);
    display.print("00:00");
    display.display();
    delay(1000);
}
