# ⚡ Power Guard – Intelligent Energy Monitoring System

![Platform](https://img.shields.io/badge/platform-ESP32%20%7C%20LabVIEW-green)

---

## 📌 Overview

Power Guard is an IoT-based intelligent energy monitoring system that integrates ESP32, LabVIEW, and ThingSpeak to monitor, process, and visualize electrical parameters in real time.

The system captures voltage and current data, computes power consumption, and uploads the results to the cloud for analysis.

---

## 🚀 Features

- Real-time monitoring of voltage and current  
- Power calculation using ESP32  
- Cloud integration using ThingSpeak  
- LED indication for data reception  
- Graphical data visualization  

---

## 🏗️ System Architecture

LabVIEW → Serial Communication → ESP32 → WiFi → ThingSpeak Cloud

---

## 📂 Repository Structure

/all_ppt_works                → Project presentations  
/labviewsimulation_work       → LabVIEW files and simulations  
/simulation_arduino_codes     → ESP32 / Arduino code  

---

## ⚙️ Hardware Requirements

- ESP32 / ESP8266[ESP-12E]  
- Voltage Sensor [PZEM004T]  
- Current Sensor [CT COIL]  
- Breadboard and wires  

---

## 🧰 Software Requirements

- Arduino IDE  
- LabVIEW  
- ThingSpeak account  

---
## 🔧 Setup Instructions

### 2. Upload ESP32 Code
- Open Arduino IDE  
- Select ESP32 board  
- Upload code from the folder: simulation_arduino_codes  

### 3. Run LabVIEW
- Open files from the folder: labviewsimulation_work  
- Start serial communication  

### 4. Configure ThingSpeak
- Create a channel  
- Copy API key  
- Update it in ESP32 code  

---

## 💻 Core Logic

### 🔹 Data Processing

    float power = voltage * current;

- Reads serial data from LabVIEW  
- Extracts voltage and current  
- Calculates power  

---

### 🔹 ThingSpeak Upload

    String url = String(server) + "?api_key=" + apiKey;

- Connects ESP32 to WiFi  
- Sends data to cloud  

---

## 🖥️ LabVIEW Interface

### 🔹 Block Diagram  
![Block Diagram](labviewsimulation_work/block_diagram.png)

### 🔹 Front Panel  
![Front Panel](labviewsimulation_work/front_panel.png)

---

## 📊 ThingSpeak Output  
![ThingSpeak Dashboard](labviewsimulation_work/thingspeak_dashboard.png)

---

## 🧠 Future Enhancements

- Energy usage prediction using ML  
- Mobile app integration  
- Smart load control  
- Fault detection system  

---

## 🙏 Acknowledgements

- Arduino community  
- LabVIEW resources  
- IoT open-source contributors  

