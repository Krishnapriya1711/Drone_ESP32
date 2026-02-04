This project is for "Air Quality Monitoring using F450 Drone". For this project, I have integrated MQ135, MQ7, PM2.5 GP2Y1010AU0F and DHT11 sensors with ESP32 for real-time monitoring.
First set up the dust sensor - PM2.5 GP2Y1010AU0F with the Arduino and then step by step add the other sensors. You can follow any of the youtube videos for the connections, but follow the pin arrangement as per the code given in the file - dust.ino for the ESP32
The final code is given in final.ino

## Features
- ESP32-based sensor interfacing
- Real-time data acquisition
- Wireless communication
- Low-power embedded design principles

## Hardware & Software
- ESP32
- MQ-series / environmental sensors - MQ135, MQ7, 
- Embedded C / Arduino framework

## Applications
- IoT-based monitoring systems
- Embedded sensor networks
- Smart device prototyping

## Future Scope
- Cloud dashboard integration
- Edge analytics using ML models


Connect the ESP32 and the laptop/desktop to the same hotspot/WiFi. DON'T connect to any public/college connection, it won't work.
To host the webpage: 
1. Include Necessary Libraries: In your Arduino sketch, you'll need the WiFi.h and WebServer.h libraries.
2. Define Network Credentials: Specify your local Wi-Fi network's SSID and password in your code.
3. Write the Code: The code will connect to your Wi-Fi network in setup(), obtain an IP address, and start the web server. In loop(), it will handle client requests and serve the HTML content, which can be embedded directly in the Arduino sketch as a string or stored in the ESP32's filesystem (SPIFFS).
4. Upload and Access: Connect the ESP32 to your computer and upload the code.
5. Open the Serial Monitor to find the ESP32's assigned IP address (e.g., 192.168.1.100).
6. Type this IP address into a web browser on a device connected to the same network to view your webpage. 
The final output be viewed in the webpage.

