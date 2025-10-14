#include <Wire.h>
#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>

// RFID pins
#define SS_PIN 10
#define RST_PIN 9

// GSM pins
#define GSM_RX 7
#define GSM_TX 8

MFRC522 mfrc522(SS_PIN, RST_PIN);
SoftwareSerial gsm(GSM_RX, GSM_TX); // RX, TX for GSM module

// Students and parent numbers
String studentUIDs[5] = {"E622410", "BEABF5", "UID3", "UID4", "UID5"};
String studentNames[5] = {"Jothis", "Jibin", "Shaheem", "Jomson", "Rahul"};
String parentNumbers[5] = {
  "+919446705532", 
  "+918056360688", 
  "+919333333333", 
  "+919444444444", 
  "+919555555555"
};

// Attendance array (real-time tracking)
int attendance[5][8]; // 5 students, 8 periods

int currentPeriod = 0;
const unsigned long periodDuration = 10000; // simulate period duration
unsigned long periodStartTime = 0;

void setup() {
  Wire.begin();          
  Serial.begin(9600);    
  SPI.begin();
  mfrc522.PCD_Init();

  // Initialize GSM
  gsm.begin(9600);
  Serial.println("Initializing GSM...");
  delay(1000);
  gsm.println("AT");
  delay(1000);
  gsm.println("AT+CMGF=1");  // Text mode
  delay(1000);

  Serial.println("RFID + GSM Master Ready...");

  periodStartTime = millis();

  // Initialize attendance array
  for(int i=0;i<5;i++){
    for(int j=0;j<8;j++){
      attendance[i][j] = 0; // 0 = absent
    }
  }
}

void loop() {
  // Auto increment period
  if (millis() - periodStartTime >= periodDuration) {
    if (currentPeriod < 7) { // 8 periods total
      currentPeriod++;
      periodStartTime = millis();
      Serial.print("Period changed to: ");
      Serial.println(currentPeriod + 1);
    } else if (currentPeriod == 7) {
      // End of day, send daily summary
      sendDailySummary();
      currentPeriod++; // to avoid sending again
    }
  }

  // Check for new RFID card
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial())
    return;

  // Read UID
  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }
  uidString.toUpperCase();
  Serial.print("Card UID: ");
  Serial.println(uidString);

  // Send UID to Slave via I2C
  Wire.beginTransmission(8); // Slave address
  Wire.write(uidString.c_str());
  Wire.endTransmission();

  // Find student index
  int studentIndex = -1;
  for (int i = 0; i < 5; i++) {
    if (uidString == studentUIDs[i]) {
      studentIndex = i;
      break;
    }
  }

  // Mark attendance & send real-time SMS
  if (studentIndex != -1) {
    attendance[studentIndex][currentPeriod] = 1; // mark present
    String msg = "Attendance marked for " + studentNames[studentIndex] +
                 " (Period " + String(currentPeriod + 1) + ")";
    sendSMS(parentNumbers[studentIndex], msg);
  } else {
    sendSMS("+919446705532", "Unknown Card UID: " + uidString);
  }

  delay(1500); // small delay before next scan
}

// Function to send SMS
void sendSMS(String number, String text) {
  gsm.println("AT+CMGF=1");  // Text mode
  delay(500);
  gsm.print("AT+CMGS=\"");
  gsm.print(number);
  gsm.println("\"");
  delay(500);
  gsm.print(text);
  delay(500);
  gsm.write(26); // CTRL+Z
  delay(3000);
  Serial.println("SMS sent to " + number + ": " + text);
}

// Function to send Daily Summary SMS to all parents
void sendDailySummary() {
  Serial.println("Sending daily summary SMS to all parents...");

  for (int i = 0; i < 5; i++) {
    String msg = "Attendance Summary for " + studentNames[i] + ":\n";
    for (int p = 0; p < 8; p++) {
      msg += "P" + String(p + 1) + ": ";
      if (attendance[i][p] == 1) msg += "Present";
      else msg += "Absent";
      msg += "\n";
    }
    sendSMS(parentNumbers[i], msg);
    delay(3000); // avoid GSM overload
  }

  Serial.println("Daily summary sent to all parents.");
}
