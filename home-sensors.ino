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

// display pins
const int sda = 4; // GPIO4 = D2
const int scl = 5; // GIPO5 = D1

// current sensor & pin
EnergyMonitor em;
const int sct = 12; // GPIO12 = D6

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

    // current sensor
    pinMode(A0, INPUT);
    Serial.println("current sensor parameters:");
    Serial.printf("sct: %d\n", sct);
    em.current(A0, 59); // use same value as the amperage (less one) of the SCT013, in my case: 60A=59
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

void monitorEnergy() {
    // female headphone jack
    // pins:
    //   1 - gnd
    //   2 - mic -- not used
    //   3 - left
    //   4 - right

    // male headphone jack from SCT013
    // ---/--/--
    // out/vac/out -- SCT013
    // gnd/rht/lft -- headphone jack
    //   out - output
    //   vac - vacancy -- not used
    //   gnd - ground
    //   rht - right
    //   lft - left
    //
    // conclusion: use ground and left pins

    display.clearDisplay();

    float pinCurrent = analogRead(1);

    Serial.print("Pin Current:");
    Serial.println(pinCurrent);

    float irms = em.calcIrms(1500);
    // ((2.2amps - 516) * 0.7)/11.9
    float amps = ((irms - 516)*0.707)/11.8337;
    em.serialprint();
    
    Serial.print("Current: ");
    Serial.print(irms);
    Serial.println("A");
    Serial.print("Adj Curr: ");
    Serial.print(amps);
    Serial.println("A");
    display.setTextSize(4);
    display.setTextColor(WHITE);
    display.setCursor(3, 0);
    display.print(amps);
    display.print("A");
    display.display();
    delay(250);
}

void loop() {
    monitorEnergy();

    // Serial.print(millis());
    // Serial.println(" looping");

    // display.clearDisplay();
    // display.setTextSize(4);
    // display.setTextColor(WHITE);
    // display.setCursor(3, 0);
    // display.print("00:00");
    // display.display();


    // delay(1000);
}
