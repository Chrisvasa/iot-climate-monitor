# IoT Climate Monitor

**IoT Climate Monitor** is an IoT-based multi-zone climate monitoring system designed to measure and visualize temperature and humidity in real-time. The project utilizes multiple ESP32 devices with DHT11 sensors to gather data from different zones, which is then processed by a Raspberry Pi acting as a central gateway. Data is sent to ThingsBoard for visualization and notification management.

## Features
- **Multi-Zone Monitoring:**
  Measure temperature and humidity from multiple locations using ESP32 devices.

- **Centralized Gateway:**
  Raspberry Pi collects data from ESP32 devices and forwards it to ThingsBoard.

- **Real-Time Visualization:**
  Display collected data on a ThingsBoard dashboard for easy monitoring.

- **Notifications:**
  Receive alerts for temperature or humidity levels exceeding predefined thresholds.

- **Energy Efficiency:**
  ESP32 devices use sleep mode to conserve battery power.

- **Customizable Intervals:**
  Set different data collection intervals for each ESP32 device.

## Objectives
The primary goal of this project is to create a functional prototype that demonstrates the capabilities of IoT technology for climate monitoring in environments such as offices, warehouses, or homes.

## Technologies Used
- **Hardware:** ESP32, DHT11 sensors, Raspberry Pi.
- **Software:**
  - ESP-IDF/Arduino for ESP32 programming.
  - Python/Node.js for Raspberry Pi gateway logic.
  - ThingsBoard for data visualization and notifications.
  - MQTT for communication between ESP32 devices and Raspberry Pi.

## Future Scope
- Support for additional sensors (e.g., air quality, pressure).
- Integration with Smart Home systems like Google Home or Alexa.
- Advanced data analysis with machine learning for trend prediction.
- Expanded network capability to handle more zones.

## License
This project is licensed under the MIT License. See `LICENSE` for details.

---
