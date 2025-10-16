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


![Screenshot_2025-10-12-23-51-55-890_com miui gallery](https://github.com/user-attachments/assets/130fa90a-5616-45e2-af0e-3de88c5cbe6b)
