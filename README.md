# 🔐 Smart Door-Access Control System



An Arduino-based smart access control system that combines human presence detection, RFID authentication, password verification, and servo-based door unlocking.



---



## 📌 Overview



The **Smart Door Access Control System\*\* is an embedded security system developed and simulated using **Arduino Uno and Wokwi**.



The system detects the presence of a person using a PIR sensor and an ultrasonic sensor. Once a person is detected, the OLED prompts the user to scan an RFID card.



After successful RFID authentication, the user can enter a password using a 4×4 matrix keypad.



The keypad is connected to a custom **PCF8574 I/O expander**, which communicates with the Arduino through **I²C**.



If authentication is successful, a servo motor simulates the unlocking of the door.



---



## 🔄 System Flow



&#x20;   Person Detected

&#x20;         ↓

&#x20;   PIR + Ultrasonic

&#x20;         ↓

&#x20;   OLED: "SCAN CARD"

&#x20;         ↓

&#x20;   RFID Card Scanned

&#x20;         ↓

&#x20;   RFID Authentication

&#x20;         ↓

&#x20;      Authorized?

&#x20;       ↙       ↘

&#x20;      No        Yes

&#x20;      ↓          ↓

&#x20;    Denied    Enter Password

&#x20;                 ↓

&#x20;             4×4 Keypad

&#x20;                 ↓

&#x20;              PCF8574

&#x20;                 ↓

&#x20;             I²C → Arduino

&#x20;                 ↓

&#x20;            Password Check

&#x20;               ↙      ↘

&#x20;          Incorrect   Correct

&#x20;              ↓          ↓

&#x20;           Denied    Access Granted

&#x20;                         ↓

&#x20;                   Servo Activated

&#x20;                         ↓

&#x20;                    Door Unlocked



---



## ✨ Features



- PIR-based human presence detection

- Ultrasonic distance detection

- RFID-based authentication

- 4×4 matrix keypad for password entry

- Custom PCF8574 I/O expander

- I²C communication

- OLED user interface

- Password verification

- Servo-based door lock simulation

- Multiple authorized RFID cards

- Non-blocking timing using `millis()`

- Custom Wokwi chip implementation

- Complete Wokwi simulation



---



## 🧰 Hardware Components



| Component | Purpose |

|---|---|

| Arduino Uno | Main microcontroller |

| MFRC522 RFID Reader | RFID authentication |

| 4×4 Matrix Keypad | Password input |

| PCF8574 | GPIO expansion |

| SSD1306 OLED | User interface |

| HC-SR04 | Distance detection |

| PIR Sensor | Presence detection |

| Servo Motor | Door lock simulation |

| Buzzer | Audio feedback |

| LED | Status indication |



---



## 🔌 Communication



### I²C Communication



The Arduino communicates with the OLED and custom PCF8574 using the I²C communication protocol.



The custom PCF8574 uses the I²C address:



&#x20;   0x20



The PCF8574 pins are connected to the keypad as follows:



&#x20;   P0 → Row 1

&#x20;   P1 → Row 2

&#x20;   P2 → Row 3

&#x20;   P3 → Row 4



&#x20;   P4 → Column 1

&#x20;   P5 → Column 2

&#x20;   P6 → Column 3

&#x20;   P7 → Column 4



The Arduino activates one row at a time and checks the column states to determine which key is pressed.



### SPI Communication



The MFRC522 RFID reader communicates with the Arduino using the SPI protocol.



---



## 🔢 Keypad Scanning



The project uses a 4×4 matrix keypad connected through the custom PCF8574 I/O expander.



The Arduino communicates with the PCF8574 through I²C.



The keypad scanning process is:



&#x20;   Activate Row

&#x20;        ↓

&#x20;   Send State to PCF8574

&#x20;        ↓

&#x20;   Read PCF8574

&#x20;        ↓

&#x20;   Check Columns

&#x20;        ↓

&#x20;   Detect Pressed Key

&#x20;        ↓

&#x20;   Return Key



The keypad scanning logic is implemented in:



&#x20;   getKeyFromPCF8574()



### Keypad Controls



&#x20;   * → Backspace

&#x20;   # → Enter



The `*` key is used to remove the previously entered character, while the `#` key is used to submit the entered password.



---



## 🔐 RFID Authentication



The MFRC522 RFID reader reads the UID of an RFID card.



The detected UID is compared with the authorized UIDs stored in the Arduino program.



The system supports multiple authorized RFID cards.



The authentication process is:



&#x20;   RFID Card

&#x20;       ↓

&#x20;   Read UID

&#x20;       ↓

&#x20;   Compare UID

&#x20;       ↓

&#x20;    Authorized?

&#x20;      ↙     ↘

&#x20;     No      Yes

&#x20;     ↓        ↓

&#x20;   Denied   Continue

&#x20;            Authentication



---



## 🔢 Password Authentication



After successful RFID authentication, the user enters a password using the 4×4 keypad.



The keypad uses special keys for password control:



&#x20;   * → Backspace

&#x20;   # → Enter



The `*` key removes the last entered character, while the `#` key confirms and submits the password.



The entered password is then compared with the authorized password.



&#x20;   Enter Password

&#x20;         ↓

&#x20;      Press #

&#x20;         ↓

&#x20;   Password Check

&#x20;      ↙       ↘

&#x20;   Incorrect   Correct

&#x20;       ↓          ↓

&#x20;    Denied     Granted



---



## 🖥️ OLED Display



The SSD1306 OLED provides visual feedback to the user.



Example messages include:



&#x20;   SCAN CARD

&#x20;   ENTER PASSWORD

&#x20;   ACCESS GRANTED

&#x20;   ACCESS DENIED



The OLED communicates with the Arduino using I²C.



---



## 🚪 Door Control



A servo motor is used to simulate the door locking mechanism.



When authentication is successful, the servo moves to simulate unlocking the door.



&#x20;   Authentication Successful

&#x20;             ↓

&#x20;       Access Granted

&#x20;             ↓

&#x20;       Servo Activated

&#x20;             ↓

&#x20;        Door Unlocked



The servo can later be replaced with an appropriate physical locking mechanism for a real-world implementation.



---



## ⏱️ Non-Blocking Timing



The project uses `millis()`-based timing for periodic operations instead of relying on long `delay()` calls.



This allows different parts of the system to operate without unnecessarily blocking the main loop.



Timing is used for operations such as:



- PIR checking

- Ultrasonic measurement

- Keypad processing

- OLED updates

- RFID checking

- Buzzer control

- Servo control



---



## 🧪 Wokwi Simulation



The complete system is simulated using **Wokwi**.



A custom PCF8574 chip was created for the simulation to provide the required I/O expansion and I²C communication for the keypad.



The custom chip implementation consists of:



&#x20;   pcf8574.chip.c

&#x20;   pcf8574.chip.json



The circuit configuration is defined in:



&#x20;   diagram.json



---



## 🔗 Wokwi Simulation



The complete project can be tested through the Wokwi simulation.



**Wokwi Project:**  


https://wokwi.com/projects/473753324101395457


---


## 📂 Project Structure



&#x20;   Smart-Door-Access-Control-System/

&#x20;   │

&#x20;   ├── sketch.ino

&#x20;   ├── diagram.json

&#x20;   ├── libraries.txt

&#x20;   ├── pcf8574.chip.c

&#x20;   ├── pcf8574.chip.json

&#x20;   │

&#x20;   ├── Images/

&#x20;   │   └── system-overview.png

&#x20;   │

&#x20;   └── README.md



### Important Files



| File | Description |

|---|---|

| `sketch.ino` | Main Arduino program |

| `diagram.json` | Wokwi circuit configuration |

| `libraries.txt` | Required libraries |

| `pcf8574.chip.c` | Custom PCF8574 implementation |

| `pcf8574.chip.json` | Custom chip configuration |

| `Images/` | Project images |



---



## 💻 Technologies Used



- Arduino C/C++

- Arduino Uno

- Wokwi

- I²C

- SPI

- RFID

- Matrix keypad scanning

- PCF8574 I/O expansion

- OLED display

- PIR sensing

- Ultrasonic sensing

- Servo control

- `millis()`-based timing

- Embedded systems programming



---



## 🎯 Project Objective



The main objective of this project is to develop a multi-stage access control system using embedded hardware.



The system combines multiple stages of authentication:



&#x20;   Presence Detection

&#x20;          ↓

&#x20;   RFID Authentication

&#x20;          ↓

&#x20;   Password Authentication

&#x20;          ↓

&#x20;   Access Decision

&#x20;          ↓

&#x20;   Door Control



The project also provides practical experience with hardware-software integration and embedded programming.



---



## 📚 Learning Outcomes



This project provided practical experience with:



- Arduino programming

- Embedded C/C++

- I²C communication

- SPI communication

- Matrix keypad scanning

- GPIO expansion

- RFID interfacing

- OLED interfacing

- Sensor integration

- Servo control

- Non-blocking programming

- `millis()` timing

- Custom Wokwi chip development

- Hardware-software integration



---



## 🚀 Future Improvements



Possible future improvements include:



- EEPROM-based password storage

- Individual passwords for RFID cards

- Password management through keypad

- Access logging

- Real-time clock integration

- Automatic door locking

- Failed-attempt lockout

- Alarm functionality

- ESP32-based wireless monitoring

- Cloud/database integration

- Real hardware implementation



---



## 👩‍💻 Team


**Haripriya**
**Gauri Banka**  



---



## ⭐ Project Status


**Simulation prototype completed in Wokwi.**


The project currently demonstrates the core access-control logic, sensor integration, RFID authentication, keypad input, I²C communication, and servo-based door control in a simulated environment.

