# Diffraction Decoder

## Purpose & Educational Goals

The Diffraction Decoder Game is an interactive educational tool designed to teach students fundamental concepts in optics, specifically **diffraction phenomena**. By combining hands-on puzzle-solving with real optical physics, this game makes abstract scientific concepts tangible and engaging.

Students learn about:
- Light diffraction through various apertures and gratings
- The relationship between wavelength, aperture size, and diffraction patterns
- Spatial reasoning and problem-solving skills
- Pattern recognition and scientific observation

## Introduction

This project transforms traditional optics education into an immersive, game-based learning experience. Players must solve a physical sliding puzzle, then use their understanding of light diffraction to decode a secret password. The game bridges the gap between theoretical physics knowledge and practical application, making learning both fun and memorable.

## How to Play

### Step 1: Solve the Sliding Puzzle
- The game board features **9 box slots with only 8 slides**
- Slide the pieces around to reveal numbers corresponding to different colors
- Each color-number combination represents a diffraction configuration

### Step 2: Laser Diffraction Analysis
- Rotate the game machine to access different numbered positions
- Shine a laser pointer through the diffraction lens at each numbered position
- Observe where the light is diffracted and the resulting pattern
- The diffraction patterns reveal the digits of the secret password

### Step 3: Enter the Password
- Use the keypad to input the 6-digit code discovered through diffraction analysis
- The LCD display provides feedback on your progress
- Correct entry triggers a victory melody and logs your success

### Step 4: Continue Learning
- Upon successful completion, the system displays the next game/challenge
- Progress is automatically tracked via WiFi connectivity and Google Sheet logging

## Technical Skills & Components

### Hardware Components
- **ESP32 Microcontroller**: Main processing unit with built-in WiFi
- **16x2 LCD Display**: Real-time feedback and instructions
- **4x3 Matrix Keypad**: Password input interface
- **Buzzer**: Audio feedback for correct/incorrect attempts
- **Laser Pointer & Diffraction Lens**: Core optical components
- **3D Printed Main Container**: Holds the keypad and stores necessary hardwares
- **Physical Sliding Puzzle Board**: Mechanical game interface

### Software Features
- **WiFi Connectivity**: Automatic connection to UCSD-GUEST network
- **Google Apps Script Integration**: Cloud-based progress tracking
- **Real-time Data Logging**: Performance analytics and game completion tracking
- **Robust Error Handling**: Automatic WiFi reconnection and connection monitoring
- **Audio Feedback System**: Success and failure melody indication

### Technical Skills Demonstrated
- **Embedded Systems Programming**: Arduino/ESP32 development
- **Network Programming**: HTTP client implementation and WiFi management
- **Cloud Integration**: Google Sheets API integration via Apps Script
- **Educational Game Design**: Pedagogically-informed interaction design

## System Architecture

```
Physical Game Board → ESP32 Microcontroller → LCD/Keypad Interface
                                ↓
                        WiFi Network Connection
                                ↓
                        Google Apps Script
                                ↓
                        Google Sheets Database
```

## Setup & Installation

### Prerequisites
- Arduino IDE with ESP32 board package
- Required libraries:
  - `LiquidCrystal.h`
  - `Keypad.h`
  - `WiFi.h`
  - `HTTPClient.h`

### Hardware Setup
1. Connect LCD display to ESP32 pins (ex. 22, 23, 21, 19, 18, 5)
2. Wire keypad matrix to pins (ex. 27, 14, 12 for columns; 32, 33, 25, 26 for rows)
3. Connect buzzer to pin 13 (or any GPIO pin)
4. Assemble physical sliding puzzle board
5. Install diffraction lens and laser pointer mechanism

### Software Configuration
1. Upload the provided Arduino sketch to ESP32
2. Ensure `pitches.h` file is in the same directory
3. Configure Google Apps Script URL in the code (if using Google Sheet)
4. Verify WiFi credentials

## Educational Impact

This game successfully combines:
- **Kinesthetic Learning**: Physical manipulation of puzzle pieces
- **Visual Learning**: Observation of light diffraction patterns  
- **Logical Reasoning**: Connecting patterns to numerical codes
- **Technology Integration**: Modern IoT and cloud connectivity

Students develop both scientific understanding and technical skills while engaging with cutting-edge educational technology.

## Project Contributors

### [Jordon Warf ↗](https://www.linkedin.com/in/jordonwarf)
- **Original Game Design**: Conceptualized educational diffraction-based puzzle game combining optics and interactive learning
- **Hardware Fabrication**: Designed and constructed physical game board with sliding puzzle mechanism and optical components
- **Core Programming**: Developed foundational embedded systems code for keypad input, LCD display, and game logic
- **Optical Integration**: Integrated and calibrated laser pointer, diffraction lens, and optical measurement systems

### [Nick Ji ↗](nickjiee.github.io)
- **WiFi Connectivity Implementation**: Modified code to add robust WiFi connection management with automatic reconnection capabilities
- **Google Sheets Integration**: Developed HTTP client functionality to send game completion status to Google Sheets via Apps Script for real-time progress tracking
- **Dynamic Game Management**: Implemented system to retrieve and display next game information from Google Sheets database on LCD screen
- **Testing & Maintenance**: Verified and maintained game functionalities and presented game at outreach events.
- **Documentation**: Authored this comprehensive README and technical specification

## License

This educational project is designed for academic use and learning purposes. Everyone is more than welcome to replicate and learn! For more details, please check the [MIT License](/LICENSE).
