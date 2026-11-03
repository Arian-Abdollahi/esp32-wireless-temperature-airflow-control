#include "BluetoothSerial.h"
#include <Arduino.h>


String device_name = "ESP32-WROOM-32";

// Check if Bluetooth is available
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// Check Serial Port Profile
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Port Profile for Bluetooth is not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

const int lm35Pin = 34 ;
const int heaterPin = 17; // پین دیجیتال برای کنترل رله
const int fanPin = 26;    // پین متصل به رله فن
const int relay1Pin = 27;
const int relay2Pin = 14;
const int limitSwitch1Pin = 25;
const int limitSwitch2Pin = 16;
float deadband = 5.0; // محدوده مرده باند
float setpoint = 0.0;
// تابع برای دریافت داده‌های بلوتوث
int H = 0;
int finish = 1;

void setup() {
  Serial.begin(115200);
  SerialBT.begin(device_name);  //Bluetooth device name
  //SerialBT.deleteAllBondedDevices(); // Uncomment this to delete paired devices; Must be called after begin
  Serial.printf("\nThe device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  pinMode(lm35Pin,INPUT);
  pinMode(heaterPin, OUTPUT);
  pinMode(fanPin, OUTPUT);
  pinMode(limitSwitch1Pin, INPUT);
  pinMode(limitSwitch2Pin, INPUT);
  pinMode(relay1Pin, OUTPUT);
  pinMode(relay2Pin, OUTPUT);
  digitalWrite(heaterPin, LOW);
  digitalWrite(fanPin, LOW);
  Serial.write("please write a set point: \n "); 
  // پین‌های رله‌ها را خاموش می‌کنیم
  digitalWrite(relay1Pin, LOW);
  digitalWrite(relay2Pin, LOW);
  digitalWrite(limitSwitch1Pin,LOW);
  digitalWrite(limitSwitch2Pin,LOW);
}

void turn_right(){
    if (H == 0){
    digitalWrite(relay1Pin, HIGH);
    digitalWrite(relay2Pin, LOW);
    delay(50);
    H = -1;
    }
  else if (H == 1) {
    digitalWrite(relay1Pin, HIGH);
    digitalWrite(relay2Pin, LOW);
    delay(70);
    H = -1;
    }
  else {
    digitalWrite(relay1Pin, LOW);
    digitalWrite(relay2Pin, LOW);
    H = -1;
    }    
}
void turn_left(){
  if (H == 0){
    digitalWrite(relay1Pin, LOW);
    digitalWrite(relay2Pin, HIGH);
    delay(50);
    H = 1;
  }
  else if (H == -1) {
    digitalWrite(relay1Pin, LOW);
    digitalWrite(relay2Pin, HIGH);
    delay(70);
    H = 1;
  }
  else {
    H = 1;
  }  
}

void loop() {
  if (finish != 0){
    if (Serial.available()) {
    SerialBT.write(Serial.read());
    }
    if (SerialBT.available()) {
    Serial.write(SerialBT.read());
    }
    delay(20);

    bool limitSwitch1State = digitalRead(limitSwitch1Pin);
    bool limitSwitch2State = digitalRead(limitSwitch2Pin);

    if (SerialBT.available()) {
      char receivedChar = SerialBT.read();

      // Setpoint update
      if (isdigit(receivedChar)) {
        String input = String(receivedChar) + SerialBT.readStringUntil('\n');
        setpoint = input.toFloat();
        SerialBT.print("Set Point Updated: ");
        SerialBT.println(setpoint);
      }
      
      // Motor control
      if (receivedChar == 'R') {
        digitalWrite(heaterPin, HIGH); // روشن کردن هیتر
        digitalWrite(fanPin, LOW);     // خاموش کردن فن
        turn_right();
        SerialBT.print("\nturn Right Updated\n");
      } 
      else if (receivedChar == 'F') {
        digitalWrite(heaterPin, LOW); // روشن کردن هیتر
        digitalWrite(fanPin, LOW);     // خاموش کردن فن
        SerialBT.print("\nTurn off\n");
        finish = 0;

      }     
      else if (receivedChar == 'L') {
        digitalWrite(heaterPin, LOW);  // خاموش کردن هیتر
        digitalWrite(fanPin, HIGH);    // روشن کردن فن
        turn_left();
        SerialBT.print("\nturn left Updated\n");
      }
    }  
      // خواندن ولتاژ انالوگ از سنسور
    int sensorValue = analogRead(lm35Pin);
    
    // محاسبه دما بر اساس ولتاژ خوانده شده
    float temperatureC = sensorValue*0.489 - 80;
    if(setpoint != 0){
      SerialBT.print("Temperature: ");
      SerialBT.print(temperatureC);
      SerialBT.println(" C");    
    }


    if (limitSwitch1State == HIGH && limitSwitch2State == LOW){
      digitalWrite(heaterPin, HIGH); // روشن کردن هیتر
      digitalWrite(fanPin, LOW);     // خاموش کردن فن
      turn_right();
      SerialBT.print("\nlimitswitch 1 Right Updated\n");
    }
    else if (limitSwitch2State == HIGH && limitSwitch1State == LOW){
      digitalWrite(heaterPin, LOW);  // خاموش کردن هیتر
      digitalWrite(fanPin, HIGH);    // روشن کردن فن
      turn_left();
      SerialBT.print("\nlimitswitch 2 left Updated\n");
    }
    
    else if (temperatureC  < (setpoint - deadband)) {
        // کنترل هیتر و فن با توجه به دمای فعلی
      if (setpoint > 0) {
      // کنترل هیتر و فن با توجه به دمای فعلی
        digitalWrite(heaterPin, HIGH); // روشن کردن هیتر
        digitalWrite(fanPin, LOW);     // خاموش کردن فن
        Serial.println("Heater ON, Fan OFF");
        //  اگر دما کمتر از دمای مرجع منهای باند مرده باشد، رله 1 روشن و رله 2 خاموش می‌شود
        turn_right();
      } 
    }
    else if (temperatureC > (setpoint + deadband) ) {
      if (setpoint > 0) {
        digitalWrite(heaterPin, LOW);  // خاموش کردن هیتر
        digitalWrite(fanPin, HIGH);    // روشن کردن فن
        Serial.println("Heater OFF, Fan ON");
        // اگر دما بیشتر از دمای مرجع به علاوه باند مرده باشد، رله 1 خاموش و رله 2 روشن می‌شود
        turn_left();
      }
    }
      
    else {
      digitalWrite(heaterPin, LOW);  // خاموش کردن هیتر
      digitalWrite(fanPin, LOW);     // خاموش کردن فن
      Serial.println("Heater OFF, Fan OFF");

      // اگر دما در محدوده باند مرده باشد، هر دو رله خاموش می‌شوند
      if (H == 1){
        digitalWrite(relay1Pin, HIGH);
        digitalWrite(relay2Pin, LOW);
        delay(50);
        H = 0;
      }
      else if (H == -1){
        digitalWrite(relay1Pin, LOW);
        digitalWrite(relay2Pin, HIGH);
        delay(70);
        H = 0;          
      }
      else {
        H = 0;
      }
    }
    digitalWrite(relay1Pin, LOW);
    digitalWrite(relay2Pin, LOW);
    delay(1000); // تاخیر2 ثانیه قبل از خواندن مقدار دما
  }
}

