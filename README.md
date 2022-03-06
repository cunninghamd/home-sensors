# Home Sensors

## Intent

Monitor various sensors (temperature, current, motion, more?) and track them in a centralized graphing tool.

## Boards

* ESP8266
* ESP32

## Sensors

* SCT013 current sensor (60A/1V)
* DS18B20 temperature sensors
* K-Type thermocouple with a MAX6675 module temperature sensor
* PIR motion sensor

## User Interface Options

* Mobile apps:
    - Blynk
    - React Native
* 0.96 inc monochromatic OLED module using SSD1306 driver

## Graphing Tool Options

* Grafana

## Deployment

### Boards

* Maually deploy to multiple boards
    - header file for configuration

### Grafana

* Local network-accessible Docker Desktop managed docker instance
