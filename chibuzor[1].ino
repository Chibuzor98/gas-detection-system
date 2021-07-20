
#include <LiquidCrystal.h>
SoftwareSerial gsm(11,10);
LiquidCrystal lcd(7,A1,A2,A3,A4,A5);

#define co2zero 273
#define rgbred 11
#define rgbgreen 9
#define gnd 10
#define gnd_buzzer 7
#define rgbblue 8
#define anInput A0
#define led 4
#define led_gnd 5
int co2ppm_low = 400;
int co2ppm_medium = 1000;
int co2ppm_high = 2000;
int buzzer = 6;

void setup() {
 Serial.begin(9600);
 lcd.begin(16,2);
 pinMode(anInput, INPUT);
 pinMode(rgbred, OUTPUT); 
 pinMode(rgbgreen, OUTPUT); 
 pinMode(rgbblue, OUTPUT); 
 pinMode(gnd, OUTPUT); 
 pinMode(gnd_buzzer, OUTPUT); 
 pinMode(buzzer, OUTPUT); 
 pinMode(led, OUTPUT); 
 pinMode(led_gnd, OUTPUT);
 digitalWrite(led_gnd, LOW);
 digitalWrite(led, HIGH);
 gsm.begin(9600);
 delay(100);
 lcd.setCursor(1,0);
 lcd.print("GAS DETECTION");
 lcd.setCursor(5,1);
 lcd.print("SYSTEM");
 delay(2000);
 lcd.clear();
 digitalWrite(buzzer,LOW);  
}

void SendMessage(String message) {
  
gsm.println("AT+CMGF=1\r");
delay(1000);
gsm.println("AT+CMGS=\"+2348067057524\"\r");
delay(1000);
gsm.println(message);
delay(1000);
gsm.print((char)26);
delay(500);
gsm.println();
Serial.println("Text Sent");
delay(500);
}
void call(){
  gsm.println("ATD+ +2348067057524;");
  delay(20000);
  gsm.println("ATH");
}

void loop() {
  digitalWrite(led_gnd,LOW);
  digitalWrite(led, HIGH);
  digitalWrite(buzzer, LOW);
  digitalWrite(gnd,LOW);
digitalWrite(gnd_buzzer, LOW);
  int co2now[10];
  int co2raw =0;
  int co2ppm =0;
  int co2_sum = 0;
  int co2_comp = 0;
  for (int j = 0;j < 10; j++) {
    co2now[j] = analogRead(A0);
    delay(100);

  }
  for (int j =0; j < 10; j++) {
    co2_sum +=co2now[j];

  }
  co2raw = co2_sum / 10;
  co2_comp =co2raw - co2zero;
  co2ppm = map(co2_comp,0,1023,400,5000);
  lcd.setCursor(3,0);
  lcd.print("PPM Value");
  lcd.setCursor(5,1);
  lcd.print(co2ppm);
  //lcd.print(String(co2ppm)); 
  lcd.setCursor(10,1); 
  lcd.print("PPM");
  Serial.println(co2ppm);
  if(co2ppm>co2ppm_low && co2ppm<co2ppm_medium) {
    analogWrite(rgbgreen, 255);
    analogWrite(rgbred,0);
    analogWrite(rgbblue,0);
    //call();
    SendMessage("Warning : co2 concentration is increasing");

  }
  else if (co2ppm>co2ppm_medium && co2ppm<co2ppm_high) {
    analogWrite(rgbblue, 255);
    analogWrite(rgbgreen, 0);
    analogWrite(rgbred, 0);
        SendMessage("Warning : co2 concentration is increasing");



  }
  else if(co2ppm>co2ppm_high && co2ppm<=5000) {
    analogWrite(rgbred, 255);
    analogWrite(rgbgreen, 0);
    analogWrite(rgbblue, 0);
    digitalWrite(buzzer, HIGH);
    SendMessage("Warning: High CO2 Concentration!!!");
    delay(1000);
     call(); 
  }
  else if(co2ppm>5000){
    analogWrite(rgbred, 0);
    analogWrite(rgbgreen, 0);
    analogWrite(rgbblue, 0);
    digitalWrite(buzzer, LOW);
  }
  
}
