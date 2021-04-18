# home-automation-system
This project relies on the implementation of the home automation system. 
The main idea is to create a system supporting ZigBee devices and connecting them to the main Hub. 
To implement this, we use the cc2531/30 USB stick as the coordinator. The endpoints are the supported devices. 
ESP32 is used to intercept packages sent from the coordinator and process them on the webserver.
