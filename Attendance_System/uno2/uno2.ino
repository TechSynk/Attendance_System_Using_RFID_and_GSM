#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <LiquidCrystal_I2C.h>
#include <EEPROM.h>

#define SD_CS 10
LiquidCrystal_I2C lcd(0x27,16,2);

char lastUID[32];
bool newUID = false;
bool daySaved = false;   

// Define students
String studentUIDs[5] = {"BEABF5","E622410","UID3","UID4","UID5"}; 
String studentNames[5] = {"Jothis","Jibin","Shaheem","Jomson","Rahul"};
int attendance[5][8];   

int currentPeriod = 0;
unsigned long periodStartTime = 0;
const unsigned long periodDuration = 10000;

void setup() {
  Wire.begin(8); 
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);

  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Attendance Ready");

  // Initialize SD card
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    lcd.setCursor(0,1);
    lcd.print("SD Fail");
    Serial.println("SD card initialization failed!");
    while(1);
  }
  Serial.println("SD card initialized successfully!");
  lcd.setCursor(0,1);
  lcd.print("SD OK");

  // Load attendance from EEPROM and reset uninitialized values
  for(int i=0;i<5;i++){
    for(int j=0;j<8;j++){
      byte val = EEPROM.read(i*8 + j);
      if(val == 255) val = 0; // Treat uninitialized EEPROM as 0
      attendance[i][j] = val;
    }
  }

  // Optional: Reset all attendance at the start of the day
  for(int i=0;i<5;i++){
    for(int j=0;j<8;j++){
      attendance[i][j] = 0;
      EEPROM.write(i*8 + j, 0);
    }
  }

  periodStartTime = millis();
}

void loop() {
  // Auto increment period
  if(millis() - periodStartTime >= periodDuration){
    if(currentPeriod < 7){  // Stop incrementing after 8th period
      currentPeriod++;
      periodStartTime = millis();
    }

    // End of day
    if(currentPeriod >= 7 && !daySaved){  // Save at last period
      saveAttendance();  
      daySaved = true;
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Period: ");
    lcd.print(currentPeriod + 1);
    lcd.setCursor(0,1);
    lcd.print("Scan UID...");
    delay(500);
  }

  // If new UID received
  if(newUID){
    int studentIndex = -1;
    for(int i=0;i<5;i++){
      if(String(lastUID) == studentUIDs[i]){
        studentIndex = i;
        break;
      }
    }

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Period: ");
    lcd.print(currentPeriod + 1);

    if(studentIndex != -1){
      // Mark attendance for current period only
      attendance[studentIndex][currentPeriod] = 1;
      EEPROM.write(studentIndex*8 + currentPeriod, 1); // backup to EEPROM

      lcd.setCursor(0,1);
      lcd.print(studentNames[studentIndex]);
      lcd.print(": Present");
      Serial.print("Period ");
      Serial.print(currentPeriod + 1);
      Serial.print(" - ");
      Serial.println(studentNames[studentIndex]);
    } else {
      lcd.setCursor(0,1);
      lcd.print("Unknown UID");
      Serial.print("Unknown UID: ");
      Serial.println(lastUID);
    }

    newUID = false;
  }

}

// I2C Receive Event
void receiveEvent(int bytes){
  int i=0;
  while(Wire.available() && i<31){
    lastUID[i] = Wire.read();
    i++;
  }
  lastUID[i] = '\0';
  newUID = true;
}

void saveAttendance(){
  Serial.println("Writing full-day attendance to SD...");
  delay(100); // allow Serial/I2C to settle

  const char* filename = "DAY1.TXT"; // change per day if needed
  File dataFile = SD.open(filename, FILE_WRITE);

  if(dataFile){
    // Print header
    dataFile.print("Student   "); // adjust spaces for alignment
    for(int p=1;p<=8;p++){
      dataFile.print("P");
      dataFile.print(p);
      dataFile.print(" ");
    }
    dataFile.println();

    // Print each student row
    for(int i=0;i<5;i++){
      // Fixed-width name column
      dataFile.print(studentNames[i]);
      int spaces = 10 - studentNames[i].length(); // pad to 10 chars
      for(int s=0;s<spaces;s++) dataFile.print(" ");

      // Attendance for 8 periods
      for(int j=0;j<8;j++){
        dataFile.print(attendance[i][j]);
        dataFile.print("  "); // double space for clarity
      }
      dataFile.println();
    }

    dataFile.close();
    Serial.println("Day summary saved to SD card.");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Day saved to SD");
  } else {
    Serial.println("Error opening file for writing.");
    lcd.setCursor(0,1);
    lcd.print("SD Write Error");
  }
} 