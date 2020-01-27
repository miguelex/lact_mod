#include <LiquidCrystal_I2C.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define UpperThreshold 550
#define LowerThreshold 500

BLECharacteristic *pCharacteristic;
bool deviceConnected = false;
int txValue = 0;

#define SERVICE_UUID            "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX  "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


bool BPMTiming    = false;
bool BeatComplete = false;
int LastTime      = 0;
int BPM           = 0;
int lcdColumns    = 16;
int lcdRows       = 2;

LiquidCrystal_I2C lcd(0x3F, lcdColumns, lcdRows); 
//LiquidCrystal_I2C lcd(0x27, lcdColumns, lcdRows); 

class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect (BLEServer* pServer) {
    deviceConnected = true ;
  };

  void onDisconnect (BLEServer* pServer) {
    deviceConnected = false;
  };
  
};

void setup() {
  lcd.init();                     
  lcd.backlight();

  // Creamos el Dispositivo BLE
  BLEDevice::init("ESP32");

  // Creamos el servidor BLE
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Creamos el Servicio BLE
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Creamos la Caracteristica BLE
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_TX,
    BLECharacteristic::PROPERTY_NOTIFY);

  // Envio de notificaciones
  pCharacteristic->addDescriptor(new BLE2902());

  // Iniciamos servicio
  pService->start();

  // Iniciamos avisos
  pServer->getAdvertising()->start();
  Serial.println("Esperando una conexion para poder notificar");
  
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
      //lcd.clear();
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
      
      if (deviceConnected) {
        txValue = BPM;
    
        //Conversion
        char txString[8];
        dtostrf(txValue, 1, 2, txString);
    
        // Seteando el valor en la caracterisitica
        pCharacteristic->setValue(txString);
    
        //Enviando notificacion
        pCharacteristic->notify();
        Serial.println("Se envia el valor: " + String(txString));
      }
      
      delay(1000);
      //lcd.clear();
  }
}
