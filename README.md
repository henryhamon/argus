# ArguS: Automated Remote Guardian Unit for Solar panels

![Status](https://img.shields.io/badge/Status-Development-yellow)
![Platform](https://img.shields.io/badge/Platform-ESP32--S3-blue)
![License](https://img.shields.io/badge/License-MIT-green)

**ArguS** is an intelligent IoT edge device designed to monitor the environmental conditions and physical state of photovoltaic plants. By leveraging computer vision and environmental sensors, ArguS acts as a sentinel for energy efficiency, autonomously detecting dust accumulation and triggering cleaning maintenance (e.g., cleaning drones) when necessary.

## 📖 Table of Contents
- [Overview](#overview)
- [Hardware Architecture](#hardware-architecture)
- [Operational Logic](#operational-logic)
- [Connectivity & Protocol](#connectivity--protocol)
- [Getting Started](#getting-started)

---

## 🧐 Overview

Solar panel efficiency can drop significantly due to environmental factors like dust (soiling), bird droppings, or high temperatures. **ArguS** solves the monitoring problem by deploying a low-power, vision-enabled node directly on the field.

Key capabilities:
* **Smart Energy Management:** Detects day/night cycles to conserve battery using Deep Sleep.
* **Environmental Sensing:** Monitors luminosity, particulate matter (dust), temperature, and humidity.
* **Visual Inspection:** Captures high-resolution images of panel surfaces when anomalies are detected.
* **M2M Integration:** Communicates via MQTT to a central server to coordinate cleaning operations.
* **Local Backup:** Stores data on SD Card if connectivity is lost.

---

## 🛠 Hardware Architecture

The system is built around the **Freenove ESP32-S3-WROOM**, chosen for its high capacity (PSRAM) to handle image buffering and processing.

| Component | Model | Function |
| :--- | :--- | :--- |
| **MCU** | **Freenove ESP32-S3** | Dual-core 240MHz, **8MB Flash**, **8MB PSRAM (OPI)**. |
| **Camera** | OV2640 | On-board visual inspection. |
| **Luminosity** | BH1750 | Measures Lux to determine day/night cycles. |
| **Dust/Particulate** | GP2Y101 | Measures air dust concentration (potential soiling). |
| **Temp/Humidity** | DHT22 | Monitors ambient temperature and humidity. |
| **Storage** | SD Card (1GB) | Local data logging (Black Box mode). |

---

## ⚙️ Operational Logic

To maximize autonomy, ArguS operates on a strict state-machine logic:

1.  **Wake Up:** The system wakes from Deep Sleep.
2.  **Day/Night Check (BH1750):**
    * *If Night (< Threshold):* The system enters **Deep Sleep** immediately.
    * *If Day (> Threshold):* The system proceeds to the monitoring phase.
3.  **Environmental Scan:**
    * Reads **DHT22** and **GP2Y101**.
    * Checks for efficiency-reducing conditions.
4.  **Visual Verification:**
    * If sensor data indicates potential soiling, the **Camera** captures an image.
5.  **Data Transmission:**
    * Connects to Wi-Fi.
    * Publishes telemetry (JSON) and Image buffer via **MQTT**.
6.  **Sleep:** Enters sleep mode for a configured interval.

---

## 📡 Connectivity & Protocol

The device uses **MQTT** for lightweight and reliable communication.

### Topic Structure

- **Telemetry:** `argus/device


## 🚀 Getting Started

### 1. Clone Repository

```

git clone https://github.com/your-username/ArgoS.git
cd ArgoS

```

### 2. Setup Credentials



```
# Copy the template

cp secrets_template.h secrets.h

# Edit with your credentials

vim secrets.h  \# or use your preferred editor

```


### 3. Configure secrets.h

Edit `secrets.h` with your actual credentials:

```

\#define SECRET_SSID "YourWiFiNetwork"
\#define SECRET_WIFI_PASSWORD "YourWiFiPassword"
\#define SECRET_MQTT_SERVER "your.mqtt.broker.com"
\#define SECRET_MQTT_USER "your_username"
\#define SECRET_MQTT_PASSWORD "your_password"

```

### 4. Verify .gitignore

Make sure `secrets.h` is NOT tracked:

```

git status

# secrets.h should NOT appear in the list

```

## 📁 Project Structure

```

ArgoS/
├── ArgoS.ino              \# Main file
├── config.h               \# Constants and parameters
├── secrets.h              \# Your credentials (NOT in git)
├── secrets_template.h     \# Template (safe to commit)
└── .gitignore            \# Ensures secrets.h is ignored

```


## 🐛 Troubleshooting

### "secrets.h: No such file or directory"

Solution: Copy `secrets_template.h` to `secrets.h` and fill in your credentials.

```

cp secrets_template.h secrets.h

```

### secrets.h appears in git status

Solution: Make sure `.gitignore` contains:

```

secrets.h

```

Then remove from git cache:

```

git rm --cached secrets.h

```

