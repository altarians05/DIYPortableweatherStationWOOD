# WoodWeatherStationSENA
Development of a portable, open-source environmental monitoring station. This project enables real-time data collection and visualization using low-cost sensors. Designed for educational and research applications, it promotes learning about environmental monitoring technologies and programming. This project represents a simplified version of an environmental monitoring station. It does not include WiFi connectivity and utilizes basic sensors connected to an Arduino Nano for data collection. The station is designed to measure important environmental parameters such as temperature, humidity, air quality, and light levels, making it an ideal solution for basic environmental monitoring applications.

Main Features:
- Variables Measured: Temperature, Humidity, Air Quality, Light Levels.
- DHT11 Sensor: Measures temperature and humidity in a single module.
- MQ135 Sensor: Detects air quality and hazardous gases.
- LDR (Light Dependent Resistor): Measures light intensity in the environment.
- LED Indicator: Provides visual alerts based on sensor readings.
- Data Storage: MicroSD card used as the primary data storage device.
- Real-time Clock: DS1302 RTC module tracks the time of each measurement.
- Data Display: 16x2 LCD screen with I2C communication for displaying environmental data.

Connections:
- DHT11 Sensor: Pin D02 (Data), 5V, GND.
- MQ135 Sensor: Pin A01 (Analog), 5V, GND.
- LDR (Light Sensor): Pin A02 (Analog), 5V, GND.
- LED: Pin D04, GND.
- LCD 16x2 (I2C): SDA -> A04, SCL -> A05, 5V, GND.
- RTC DS1307: SDA -> A04, SCL -> A05, 5V, GND.
- MicroSD Module: CS -> D10, MOSI -> D11, SCK -> D13, MISO -> D12, 5V, GND.

Libraries and Resources:
- DHT11: Library Guide and Tutorial
- MQ135: Library Guide and Tutorial
- LDR (Light Sensor): No library required.
- LCD 16x2 (I2C): Library Guide and Tutorial
- RTC DS1307: Library Guide and Tutorial
- MicroSD Module: Library and Tutorial

Project Purpose: This simplified version of the environmental monitoring station is perfect for learning and experimentation in environmental science, electronics, and programming. It provides a hands-on approach to understanding sensor integration, data logging, and time tracking, making it an excellent project for educational purposes and DIY enthusiasts.

While lacking WiFi connectivity in this version, it can easily be expanded with more advanced modules for remote data collection in future versions.
