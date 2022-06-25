//This is my oldest code for arduino, just a simple temperature and humidity monitoring with DHT sendor and TM1637 display
//And it has start timer ( idk why and what purpose it serves ¯\_(ツ)_/¯ )
//pins: display(2 - DIO, 3-CLK), lights(4-RED, 5-GREEN), temp(6), button(9)

#include <Adafruit_Sensor.h>
#include "DHT.h" //DHT library
#include <GyverTM1637.h>//Display TM1637 library
#define CLK 3//Display clk pin
#define DIO 2//Display dio pin
int8_t DispMSG[4] = {};
unsigned long dispTimer;
GyverTM1637 tm1637(CLK, DIO);//init display

#define dhtPin 6//Sensor pin
unsigned long timer;
int cell_0, cell_1, cell_2, cell_3;//TM1637 units
DHT dht(dhtPin, DHT11);//init sensor

int tDisp, hDisp;
bool button, swichFlag = 0, buttonFlag, ledState = 1;
bool dataSwich;
unsigned long ledTimer;
int ledColor;

unsigned short seconds, minutes, hours;
unsigned int secondsCount;
unsigned long pointsBlinkTimer;
bool pointsBlinkState;

void dataPrint(float, float);

void setup() {
  Serial.begin(9600);//serial port open for data output(not nessesary)
  pinMode(4, OUTPUT);//Incorrect data indicator
  pinMode(5, OUTPUT);//Correct data indicator
  pinMode(9, INPUT_PULLUP);//Swich button
  dht.begin();
  
  tm1637.brightness(5); //display brightness 0-7
  
//  if (Serial.available() > 0) {
//    hours = Serial.read();
//    Serial.println(hours);
//  }

  
}

void loop() {

  //Button for switching between start timer and sensor data
  button = !digitalRead(9);
  if(button==1 && swichFlag == 0){
    swichFlag = 1;
    buttonFlag = !buttonFlag;
  }
  if(button==0 && swichFlag == 1){
    swichFlag = 0;
  }

  //Led and double dots blinking
  if(millis()-pointsBlinkTimer > 500){
    pointsBlinkState = !pointsBlinkState;
    ledState = !ledState;
    pointsBlinkTimer = millis();
  }
  if(pointsBlinkState == 0 && buttonFlag == 0){
    tm1637.point(POINT_ON);
  }else{
    tm1637.point(POINT_OFF);
  }
  
  //Switch between temperature and humidity every 10 seconds
  if(millis()-dispTimer > 10000){
    dataSwich = !dataSwich;
    dispTimer = millis();
  }
  
  float h = dht.readHumidity(); //humidity check
  float t = dht.readTemperature(); //temperature check
    
  if (isnan(h) || isnan(t)) {  // is data correct
    Serial.println("Read error");
    digitalWrite(4, ledState);//incorrect work indicator - on
    digitalWrite(5, LOW);
  }else{
    digitalWrite(5, ledState);
    digitalWrite(4, LOW);
  }
  
  if(millis()-timer > 1000){

    seconds = (millis()/1000) - secondsCount;
    if(seconds >= 60){
      minutes++;
      seconds = 0;
      secondsCount += 60;
    }
    if(minutes == 60){
      hours++;
      minutes = 0;
    }
    if(hours == 24){
      hours = 0;
    }

    tDisp = static_cast<int>(t);
    hDisp = static_cast<int>(h);
    
    timer = millis();
  }
  
  if(buttonFlag == 0){
      if(dataSwich == 1){
        cell_0 = hours / 10;
        cell_1 = hours % 10;
        cell_2 = minutes / 10;
        cell_3 = minutes % 10;
      }else{
        cell_0 = minutes / 10;
        cell_1 = minutes % 10;
        cell_2 = seconds / 10;
        cell_3 = seconds % 10;
      }
  }else{
      if(dataSwich == 0){
        cell_0 = 0;
        cell_1 = tDisp / 10;
        cell_2 = tDisp % 10;
        cell_3 = 12;
      }else{
        cell_0 = 0;
        cell_1 = hDisp / 10;
        cell_2 = hDisp % 10;
        cell_3 = 15;
      }
      
  }

  tm1637.display(cell_0, cell_1, cell_2, cell_3);

}

void dataPrint(float temp, float humidity){
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" *C "); //Serial port - output data
} 
