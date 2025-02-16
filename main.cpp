/*smart home heating system
Kushal Shrestha
1st semester, 2024(Autumn)
*/
#include <IRremote.hpp>
#include<LiquidCrystal.h>
#include <Servo.h>
const int enA= 10;
const int motorpin1 = 9;
const int motorpin2 = 8;
const int pir = 12;
const int ir = 11;
long lastDetectionTime = 0; 
const unsigned long ONE_HOUR = 3600000;
const int humid = A1;
const int led = A2;
Servo servo_3;
int check = 0;
int speed = 0;
int temporary = 0;
LiquidCrystal lcd(7, 6, 5, 4, 13, 2);
IRrecv irrecv(ir);
decode_results results;



void setup()
{
  Serial.begin(9600);
  pinMode(motorpin1,OUTPUT);
  pinMode(motorpin2,OUTPUT);
  pinMode(enA,OUTPUT);
  pinMode(pir,INPUT);
  pinMode(A0,INPUT);
  pinMode(led,OUTPUT);
  lcd.begin(16, 2); 
  irrecv.enableIRIn();
  Serial.print("Enabled IRin\n"); 
  //Lcd printing
  lcd.setCursor(0,0);        
  lcd.print("   Made by:"); 
  lcd.setCursor(2,1);
  lcd.print("  Kushal");
  servo_3.attach(3, 500, 2500);
  pinMode(humid, INPUT);
  delay(2000);
}




void loop(){
 
    
  if (digitalRead(pir) == HIGH){
    
  	lastDetectionTime = millis();
  }
  if (millis() - lastDetectionTime > ONE_HOUR) {
    digitalWrite(motorpin1,LOW);
    digitalWrite(motorpin2,LOW);
    delay(200);
  }
   
  else{
    
    if (check % 2 == 0){
      auto_temp();
      check += 1; 
    }
    else{
       
      int val = analogRead(humid);
  if (val >= 500 && val < 800){
    digitalWrite(led,LOW);
     display_temp();
  }
    else{
      digitalWrite(led,HIGH);
       display_temp();
    }
    if(IrReceiver.decode()) {
  	auto value= IrReceiver.decodedIRData.decodedRawData;
  	Serial.print(value,HEX);
    Serial.print("\n");
    if (value == 0xFF00BF00){
    auto_temp();
    check += 1; 
    }
    else if (value == 0xF50ABF00){
    speed += 50;
    digitalWrite(motorpin1,HIGH);
    digitalWrite(motorpin2,LOW);
    analogWrite(enA,speed);
    Serial.print(speed);
    delay(200);
    }
    else if (value == 0xF708BF00){
    speed -= 50;
    digitalWrite(motorpin1,HIGH);
    digitalWrite(motorpin2,LOW);
    analogWrite(enA,speed);
      Serial.print(speed);
    delay(200);
    }
  }
  }

  }
  irrecv.resume();
  delay(100);
}



void auto_temp(){
  float temp = 0;
  do {
  if(IrReceiver.decode()) {
  auto value= IrReceiver.decodedIRData.decodedRawData;
  Serial.print(value,HEX);
  Serial.print("\n");
    if (value == 0xFF00BF00){
    break;
  }
  }
  float reading = analogRead(A0);
  float voltage = reading * (5.0 / 1024.0);
  temp =(voltage - 0.5) * 100;
  temp = (int)temp;
  int val = analogRead(humid);
  Serial.print(val);
  Serial.print("\n");
  if (val >= 500 && val < 800){
    digitalWrite(led,LOW);
  }
    else{
      digitalWrite(led,HIGH);
    }
    if (temp <= 10){
      digitalWrite(motorpin1,HIGH);
      digitalWrite(motorpin2,LOW);
      analogWrite(enA,255);
      delay(200);
    servo_3.write(90);
    delay(15);

    }
    else if (temp <= 15){
      digitalWrite(motorpin1,HIGH);
      digitalWrite(motorpin2,LOW);
      analogWrite(enA,155);
      delay(200);
      servo_3.write(90);
      
    }
    else if (temp <= 20){
      digitalWrite(motorpin1,HIGH);
      digitalWrite(motorpin2,LOW);
      analogWrite(enA,55);
      delay(200);
      servo_3.write(90);
    }
    else{
      digitalWrite(motorpin1,LOW);
      digitalWrite(motorpin2,LOW);
      delay(200);
      servo_3.write(180);
    }
  display_temp();
  }while (temp != 20);
  irrecv.resume();
}

void display_temp(){
  float reading = analogRead(A0);
  float voltage = reading * (5.0 / 1024.0);
  float temp =(voltage - 0.5) * 100;
  temp = (int)temp;
  //Lcd printing
  lcd.setCursor(0,0);        
  lcd.print(" Temperature is "); 
  lcd.setCursor(2,1);
  lcd.print("    ");
  lcd.print(temp);
  lcd.print("C   ");
}

