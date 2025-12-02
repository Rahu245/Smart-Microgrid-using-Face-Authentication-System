# Smart-Microgrid-using-Face-Authentication-System
Smart Microgrid Using Face Authentication System

This repository contains the hardware design, source code, and documentation for the project developed as part of CSE7107 – Capstone Project.

Team Members
Y R Rahul
Darshan Kumar C
J Monesh

Guide:
Ms. Swetha Rajagopal

Problem Statement
Microgrids are becoming essential for stable, localized power generation and distribution. However, traditional microgrid systems still rely on manual switching, lack intelligent monitoring, and offer no user authentication or access control.
This creates vulnerabilities such as:
Unreliable power switching between transformer and battery
No real-time monitoring of electrical/environmental conditions
Unauthorized users controlling loads
Inefficient energy usage due to absence of sensor-driven automation
The challenge is to design a smart, secure, and automated microgrid controller that performs intelligent source selection, real-time sensing, wireless control, and user authentication—while being feasible for a low-cost hardware implementation.

Overview
This project implements a smart microgrid controller capable of automatic power source switching, IoT-based monitoring, wireless control, and biometric authentication.
The system integrates:
Real-time microgrid monitoring using Arduino Nano
Sensor suite: voltage sensors, ACS712 current sensor, DHT11 temperature–humidity sensor
Automatic source switching between transformer supply and 12V Li-ion battery
Face authentication module using ESP32-CAM / Python OpenCV
Bluetooth-based load control using HC-05 and mobile interface
Relay-based hardware switching for fans, LEDs, USB devices
LCD (I2C) display for real-time voltage, current, temperature, and humidity readings
The system is a fully hardware-based prototype, designed for expandability into larger microgrid structures or IoT cloud dashboards.

Novelty Features
1. Biometric Access Control
Unlike typical microgrid prototypes, this system integrates face authentication using ESP32-CAM/OpenCV—ensuring only authorized users can operate or modify microgrid controls.

3. Intelligent Source Selection
Automatic decision-making algorithm selects between:
Mains/Transformer supply, or 12V Li-ion battery backup based on real-time voltage thresholds (~11.5V).

4. Real-Time Multi-Sensor Monitoring
The controller measures:
Voltage
Current
Temperature
Humidity
Data is displayed both locally (LCD) and accessible via Bluetooth.

5. Wireless Load Control
Using HC-05 Bluetooth module:
User can control fan, LED, and USB loads
Commands sent from mobile app / terminal

5. Fully Automated Load & Source Management
Relay modules handle:
Safe switching of loads
Seamless transition between power sources
Overcurrent protection through ACS712 monitoring

6. Low-Cost, Hardware-Oriented Prototype
Designed using:
Arduino Nano
OpenCV face recognition
Readily available sensors
Relay switching modules
This makes the system easy to deploy and adaptable to smart homes, labs, or renewable microgrids.

7. Expandable Future Architecture
Designed to support future enhancements such as:
Solar energy integration
IoT Cloud dashboards
Predictive load analysis
AI-powered fault detection
