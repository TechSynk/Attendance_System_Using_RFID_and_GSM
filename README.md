# Smart RFID Attendance System with GSM Alerts & SD Card Backup

📘 Project Title

Smart RFID Attendance System with GSM Alerts & SD Card Backup

🧠 Project Overview

This system uses two Arduino boards to automate student attendance marking using RFID cards.

 * UNO 1 (Master) handles RFID scanning and GSM communication to send SMS alerts to parents in real-time.
 * UNO 2 (Slave) manages LCD display, EEPROM, and SD card storage for daily attendance logging.
  
Both Arduinos communicate via I²C (Inter-Integrated Circuit) communication.

⚙️ System Architecture

🔹 Arduino UNO 1 (Master)

 * Reads RFID card UIDs via MFRC522 module (SPI)
 * Sends the UID to UNO 2 using I²C
 * Sends SMS notifications to parents using GSM module
 * Keeps track of period-wise attendance
 * Sends a daily summary SMS at the end of the day
   
🔹 Arduino UNO 2 (Slave)

 * Receives the UID from UNO 1 via I²C
 * Displays current period and student name on I²C LCD
 * Logs attendance data to an SD card
 * Stores attendance data in EEPROM for backup in case of power failure

🧩 Hardware Required


| Component                      | Quantity | Description                                           |
| ------------------------------ | -------- | ----------------------------------------------------- |
| Arduino UNO                    | 2        | One for Master (RFID + GSM), one for Slave (LCD + SD) |
| MFRC522 RFID Module            | 1        | To scan RFID cards                                    |
| GSM Module (SIM800L / SIM900A) | 1        | For sending SMS                                       |
| I²C LCD 16x2                   | 1        | Displays attendance info                              |
| SD Card Module                 | 1        | Stores attendance logs                                |
| EEPROM (built-in)              | -        | Used for data backup                                  |
| Jumper Wires                   | -        | For connections                                       |
| Breadboard                     | 1        | For circuit setup                                     |
| RFID Tags/Cards                | 5        | For student IDs                                       |

🔌 Pin Diagram and Connections

🧭 UNO 1 (Master) – RFID + GSM + I2C

| Module                      | Pin  | Arduino UNO Pin |
| --------------------------- | ---- | --------------- |
| **MFRC522 RFID**            | SDA  | 10              |
|                             | SCK  | 13              |
|                             | MOSI | 11              |
|                             | MISO | 12              |
|                             | RST  | 9               |
|                             | VCC  | 3.3V            |
|                             | GND  | GND             |
| **GSM Module (SIM800L)**    | TX   | 8               |
|                             | RX   | 7               |
|                             | VCC  | 5V              |
|                             | GND  | GND             |
| **I²C Connection to Slave** | SDA  | A4              |
|                             | SCL  | A5              |
|                             | GND  | Common Ground   |

🧭 UNO 2 (Slave) – LCD + SD Card + EEPROM + I2C

| Module                           | Pin  | Arduino UNO Pin |
| -------------------------------- | ---- | --------------- |
| **SD Card Module**               | CS   | 10              |
|                                  | MOSI | 11              |
|                                  | MISO | 12              |
|                                  | SCK  | 13              |
|                                  | VCC  | 5V              |
|                                  | GND  | GND             |
| **I²C LCD Display**              | SDA  | A4              |
|                                  | SCL  | A5              |
|                                  | VCC  | 5V              |
|                                  | GND  | GND             |
| **I²C Connection (from Master)** | SDA  | A4              |
|                                  | SCL  | A5              |
|                                  | GND  | Common Ground   |

🧠 How the System Works

🔸 Step 1: Initialization

 * Both Arduinos boot up.
 * UNO 1 initializes RFID and GSM modules.
 * UNO 2 initializes the LCD, SD card, and EEPROM memory.

🔸 Step 2: Period Timing

 * Each class period is simulated as 10 seconds (periodDuration = 10000).
 * UNO 1 automatically increments the currentPeriod after every 10 seconds.
 * UNO 2 also keeps track of the same.

🔸 Step 3: RFID Scan

 * A student scans their RFID card.
 * UNO 1 reads the card’s UID and identifies the student.
 * It marks the student as present for the current period.
 * It sends an SMS notification to the student’s parent using the GSM module.
 * It also sends the UID to UNO 2 via I²C.

🔸 Step 4: LCD Display and Logging

 * UNO 2 displays the current period and student’s name on the LCD.
 * It marks attendance in its EEPROM and logs data to the SD card.

🔸 Step 5: Daily Summary

 * After the final period (8th), UNO 1 automatically sends a daily summary SMS to all parents.
 * UNO 2 stores the complete attendance data in a file named DAY1.TXT on the SD card.

💾 Data Storage Example (DAY1.TXT)

Student   P1 P2 P3 P4 P5 P6 P7 P8
Jothis    1  1  0  1  0  1  1  1
Jibin     1  0  1  1  1  0  1  1
Shaheem   0  1  1  1  1  1  0  1
Jomson    1  1  1  1  1  1  1  1
Rahul     1  1  0  0  1  1  0  0

📱 SMS Format Examples

✅ Real-time attendance:

Attendance marked for Jothis (Period 3)

📊 Daily summary:

Attendance Summary for Jibin:
P1: Present
P2: Absent
P3: Present
...
P8: Present



![Screenshot_2025-10-12-23-51-55-890_com miui gallery](https://github.com/user-attachments/assets/130fa90a-5616-45e2-af0e-3de88c5cbe6b)
