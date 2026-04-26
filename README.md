## About the Project

This project demonstrates a Drive-by-Wire system implemented on a model car using ESP32 microcontrollers. 
Instead of using mechanical linkages, the system uses electronic inputs such as potentiometers and a rotary encoder to control throttle, braking, and steering.

The driver-side unit reads user inputs and transmits control signals wirelessly to the vehicle platform. 
The vehicle-side unit receives these signals and drives DC motors and a servo motor to control motion and steering.

The project focuses on:
- Real-time wireless control between driver and vehicle
- Accurate input-to-motion response
- Modular hardware design
- Practical implementation of embedded control systems


## Project Structure
The repository is organized into the following folders:

### Docs/
Contains all documentation related to the project.
Includes:
- Circuit diagrams (Driver and Vehicle sides)
- Flow diagrams explaining system logic
- Project details and final documentation
- Presentation files 

---

### Codes/
Contains the source code used in the project.

Subfolders:
**Driver/**
- Code for the driver-side ESP32
- Reads potentiometer and rotary encoder inputs
- Transmits control signals to the vehicle

**Vehicle/**
- Code for the vehicle-side ESP32
- Receives control signals
- Controls DC motors and steering servo

---

### Hardware/
Contains hardware-related files.
Subfolders:
**3D_Print/**
- 3D printed design files used for:
  - Car structure
  - Pedal mechanism
  - Steering wheel
**Components/**
- Components list used in the project
- Includes images and specifications of all hardware parts

---

### Images/
Contains real setup images of the project.
Includes:
- Driver-side setup
- Vehicle platform setup
- Complete system setup
- Mechanical and wiring arrangements


## System Overview
The system consists of two main units:
1. Driver Unit  
   Uses potentiometers and a rotary encoder to capture user input.
2. Vehicle Platform  
   Uses DC motors for motion and a servo motor for steering based on received signals.


## Components Used
Key components used in this project include:
- ESP32 (2 units)
- ESP32 Development Boards (2 units)
- Potentiometers (2 units)
- Rotary Encoder (1 unit)
- DC Motors (2 units)
- Servo Motor (1 unit)
- Motor Driver (1 unit)
- Battery and Power System
- 3D Printed Mechanical Parts


## Purpose of This Repository

This repository serves as:

- A complete reference for the Drive-by-Wire system implementation
- A documentation archive for hardware and software design
- A reproducible setup for learning and experimentation
- A demonstration of embedded systems and wireless vehicle control
