# Autonomous Competition Robot

An autonomous robot designed and built for competitive gameplay, integrating mechanical, electrical, and software systems. The robot autonomously identifies, transports, and positions scoring objects using infrared beacon tracking, Vive localization, wall-following, and PID motor control.

## Project Overview

**Course**: MEAM 5100 - Design of Mechatronic Systems  
**Institution**: University of Pennsylvania  
**Term**: Fall 2023

### Objective

Design and build an autonomous robot capable of identifying, transporting, and positioning objects on a competition field to maximize scoring potential while operating fully autonomously and handling opponent interference.

## Key Features

### Game Strategy
- Autonomous detection and manipulation of real vs. fake trophies via IR frequency recognition (550 Hz vs 23 Hz)
- Real-time navigation using Vive tracking and Time-of-Flight sensors
- Decision-making hierarchy prioritizing high-value tasks like trophy retrieval and police car pushing
- Adaptive recovery mechanisms for handling sensor or movement errors

### Autonomous Navigation
- PID-based motor control for smooth acceleration/deceleration
- Wall-following using dual TOF sensors for edge alignment and obstacle avoidance
- Real-time localization with Vive tracking and feedback-controlled path correction

### Object Manipulation
- Servo-actuated gripper for trophy handling
- Controlled pushing of the police car based on real-time Vive position updates
- Frequency-based classification for distinguishing object types

### Wireless Communication
- ESP-NOW + UDP networking for position broadcasting and communication multiplier compliance
- Fail-safe message retransmission mechanisms for reliability in high-interference environments

## System Architecture

### Mechanical Design
- Differential drive system (replacing Mecanum wheels) for improved traction
- Two-layer chassis design for weight distribution and stability
- Custom mounts for sensors and electronics
- 3D-printed components designed in SolidWorks

### Electrical System
- **Microcontroller**: ESP32-WROOM with modularized sensor architecture
- **Power System**: Dual-voltage power regulation (14.8 V LiPo → 12 V & 5 V)
- **Sensors**:
  - Dual Time-of-Flight (TOF) sensors (VL53L0X) for wall-following
  - Infrared sensors for beacon tracking and frequency detection
  - Encoders for motor feedback
- **Signal Processing**: Two-stage IR amplifier circuit for robust signal detection

### Software Architecture
- Modular state-machine architecture controlling modes (wall-following, beacon tracking, manual debug)
- PID motor control with encoder feedback
- Web-based control interface for debugging (HTML510 server)
- Real-time sensor data processing and fusion

## Technologies Used

- **Hardware**: ESP32-WROOM, Arduino IDE, SolidWorks
- **Programming**: C/C++, Embedded Systems
- **Control Systems**: PID Control, Motor Control
- **Sensors**: Infrared Sensors, Time-of-Flight Sensors (VL53L0X), Encoders
- **Localization**: Vive Tracking System
- **Communication**: ESP-NOW, UDP, WiFi, I2C, SPI, UART
- **Circuit Design**: Op-Amps, Filters, ADC

## Performance Metrics

- **Success Rate**: ~80% completion of all game objectives during trials
- **Trophy Detection Range**: Up to 2.5 m with < 0.5 s response time
- **Wall-following Accuracy**: < 5 cm lateral deviation
- **IR Frequency Detection**: < 0.5 s response time for 550 Hz vs 23 Hz classification

## Key Achievements

- Integrated mechanical, electrical, and software systems into a fully autonomous robot
- Achieved ~80% success rate in completing all game objectives during trials
- Implemented IR frequency detection (550 Hz vs 23 Hz) with < 0.5 s response time
- Developed PID-controlled differential drive with < 5 cm wall-following accuracy
- Integrated Vive tracking for real-time global localization and path correction
- Designed modular state-machine architecture supporting multiple operational modes
- Created dual-voltage power regulation (14.8 V LiPo → 12 V & 5 V)
- Built two-layer chassis for optimal weight distribution and stability

## Project Structure

```
autonomous-competition-robot/
├── code/
│   ├── Autonomous_Robot/
│   │   ├── Autonomous_Robot.ino    # Main robot control code
│   │   ├── html510.cpp             # Web server implementation
│   │   ├── html510.h
│   │   ├── vive510.cpp             # Vive tracking library
│   │   └── vive510.h
│   ├── Modular Codes/              # Modular code components
│   │   ├── ESPNow_VIve/
│   │   ├── ESPnow-receiver/
│   │   ├── ESPnow-sender/
│   │   ├── game-UDPreceiver/
│   │   └── tof_dual/
│   ├── esp_now.ino                 # ESP-NOW communication
│   ├── ir_detection.ino            # IR sensor code
│   ├── pid_controller.ino          # PID control implementation
│   ├── time_of_flight.ino          # TOF sensor code
│   ├── udp.ino                     # UDP communication
│   ├── vive.ino                    # Vive tracking code
│   └── web_controller.ino          # Web interface
├── cad-models/                     # SolidWorks CAD files
│   ├── Rover V2.SLDASM
│   ├── Chassis V2 - IR Layer.SLDPRT
│   ├── Chassis V2 - Motor Layer.SLDPRT
│   └── [other component files]
├── 2d-drawings/                    # 2D technical drawings
│   ├── 2D Drawing - Base Layer.pdf
│   ├── 2D Drawing - Caster Mount.pdf
│   ├── 2D Drawing - Power Bank Holder.pdf
│   └── 2D Drawing - Upper Layer.pdf
└── README.md
```

## Installation & Setup

### Prerequisites
- Arduino IDE (with ESP32 board support)
- ESP32-WROOM development board
- Required libraries:
  - `Adafruit_VL53L0X` (for TOF sensors)
  - `WiFi`, `WiFiUdp`, `esp_now` (ESP32 built-in)
  - Custom libraries: `html510`, `vive510`

### Hardware Setup
1. Assemble the robot chassis according to CAD models
2. Install sensors and electronics following the electrical design
3. Connect power system (14.8 V LiPo battery)
4. Configure WiFi network settings in code

### Software Setup
1. Install Arduino IDE and ESP32 board support
2. Install required libraries via Arduino Library Manager
3. Upload `Autonomous_Robot.ino` to the ESP32
4. Configure network settings (SSID, password, IP addresses)
5. Access web interface for debugging and manual control

## Usage

### Autonomous Mode
The robot operates autonomously, following these behaviors:
1. **Wall-following**: Uses dual TOF sensors to maintain distance from walls
2. **Beacon tracking**: Detects and approaches IR beacons (trophies)
3. **Frequency classification**: Distinguishes real (550 Hz) vs fake (23 Hz) trophies
4. **Object manipulation**: Grips and transports trophies to scoring zones
5. **Police car pushing**: Pushes police car based on Vive position updates

### Manual Control
Access the web interface at the robot's IP address to:
- Manually control motors and servos
- View sensor readings in real-time
- Debug and test individual components
- Monitor Vive tracking data

## License

This project is licensed under the Apache License 2.0 - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- MEAM 5100 course instructors and TAs
- Team members who contributed to the project
- University of Pennsylvania for providing resources and facilities
