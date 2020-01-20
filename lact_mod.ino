#include <LiquidCrystal_I2C.h>



#define UpperThreshold 550
#define LowerThreshold 500

bool BPMTiming    = false;
bool BeatComplete = false;
int LastTime      = 0;
int BPM           = 0;
int lcdColumns    = 16;
int lcdRows       = 2;

LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows); 

void setup() {
  lcd.init();                     
  lcd.backlight();
  Serial.begin(9600);
}
 
 
void loop() {
 
  int value = analogRead(36);

  // calc bpm
  if (value>UpperThreshold)   {
      if (BeatComplete) {
          BPM = millis()-LastTime;
          BPM = int(60/(float(BPM)/1000));
          BPMTiming =false;
          BeatComplete =false;
      }
      if (BPMTiming == false) {
          BPMTiming = true;
          LastTime  = millis();
      }
  }

  if (value < LowerThreshold & BPMTiming) {
      BeatComplete = true;
      // output bpm to serial monitor
      Serial.print(BPM);
      Serial.println(" BPM");
      lcd.setCursor(0, 0);
      lcd.print("B.P.M.");
      lcd.setCursor(0,1);
      lcd.print(BPM);
      
      
      delay(1000);
      lcd.clear();
  }
}
