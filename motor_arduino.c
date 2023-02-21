#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <LiquidCrystal_I2C.h>      
#include <SoftwareSerial.h>
#include <ThreeWire.h>  
#include <RtcDS1302.h>
#include <DFPlayer_Mini_Mp3.h>

int trig_1 = 2;
int echo_1 = 3;
int trig_2 = 4;
int echo_2 = 5;
int trig_3 = 6;
int echo_3 = 7;
int trig_4 = 8;
int echo_4 = 9;

int ENA = 10;
int EN1 = 11;
int EN2 = 12;

int O_step = 3;
int N_step;

#define SWITCH1 A0 
#define SWITCH2 A1 
#define SWITCH3 A2 
#define SWITCH4 A3

int Motor_speed=255; // 모터 속도 PWM 100으로 설정 0~255
long duration1, distance1;
long duration2, distance2;
long duration3, distance3;
long duration4, distance4;

long new_distance_1;
long new_distance_2;
long new_distance_3;
long new_distance_4;
int C=0;
long distance_return(int trig, int echo);
void motor_go(int EN1, int EN2, int ENA);
void motor_back(int EN1, int EN2, int ENA);
void motor_stop(int EN1, int EN2);
void sensor_Print(long distance);

void setup() {
  Serial.begin(9600);
  pinMode(13,OUTPUT);
   
  pinMode(SWITCH1, INPUT_PULLUP); 
  pinMode(SWITCH2, INPUT_PULLUP); 
  pinMode(SWITCH3, INPUT_PULLUP); 
  pinMode(SWITCH4, INPUT_PULLUP); 

  pinMode(EN1,OUTPUT);
  pinMode(EN2,OUTPUT);
  pinMode(trig_1, OUTPUT); // trigPin을 출력으로 
  pinMode(echo_1, INPUT); // echoPin을 입력이다.
  pinMode(trig_2, OUTPUT); // trigPin을 출력으로 
  pinMode(echo_2, INPUT); // echoPin을 입력이다.
  pinMode(trig_3, OUTPUT); // trigPin을 출력으로 
  pinMode(echo_3, INPUT); // echoPin을 입력이다.
  pinMode(trig_4, OUTPUT);
  pinMode(echo_4, INPUT);
  pinMode(ENA,OUTPUT);  
}

void loop() {
    digitalWrite(13, LOW);
    while(C==0){
        new_distance_1 = distance_return(trig_1, echo_1);
        new_distance_2 = distance_return(trig_2, echo_2);
        new_distance_3 = distance_return(trig_3, echo_3);
        new_distance_4 = distance_return(trig_4, echo_4);  //sensor_Print(new_diance_1);

        Serial.print(" 1 = "); Serial.println(new_distance_1);
        Serial.print(" 2 = "); Serial.println(new_distance_2);
        Serial.print(" 3 = "); Serial.println(new_distance_3);
        Serial.print(" 4 = "); Serial.println(new_distance_4); 

        if(new_distance_4<20){ delay(500);
            if(new_distance_4<20){ N_step = 4;} }     
        else if(new_distance_3<20) { delay(500);
            if(new_distance_3<20){ N_step = 3; } }   
        else if(new_distance_2<20){ delay(500);
            if(new_distance_2<20){ N_step = 2; } }   
        else if(new_distance_1<20&&new_distance_1<0) { delay(500);
            if(new_distance_1<20&&new_distance_1<0){ N_step = 1;  }}
        else {
            if(new_distance_4<20){N_step = 4;}
            else if(new_distance_3<20){N_step = 3;}
            else if(new_distance_2<20){N_step = 2;}
            else if(new_distance_1<20&&new_distance_1<0){N_step = 1;} 
            }  
             
        Serial.print("현재스탭 = "); Serial.println (O_step);                                                                     
        Serial.print ("NEW step = "); Serial.println (N_step);            // 1 < 2 < 3 < 4
        if(N_step == 0){ motor_stop(EN1, EN2); C=1;}
        else if(O_step < N_step){
            if(N_step == 4){Serial.println("up"); do{motor_go(EN1, EN2, ENA);}while(digitalRead(A3)==LOW);}
            else if(N_step == 3){Serial.println("up"); do{motor_go(EN1, EN2, ENA);}while(digitalRead(A2)==LOW);}
            else if(N_step == 2) {Serial.println("up"); do{motor_go(EN1, EN2, ENA);}while(digitalRead(A1)==LOW);}
            motor_stop(EN1, EN2); C=1; O_step = N_step; digitalWrite(13, HIGH); delay(15000);}
        else if(O_step > N_step) { 
            if(N_step == 1){Serial.println("down"); do{motor_back(EN1, EN2, ENA);}while(digitalRead(A0)==LOW);}
            else if(N_step == 2){Serial.println("down"); do{motor_back(EN1, EN2, ENA);}while(digitalRead(A1)==LOW);}
            else if(N_step == 3) {Serial.println("down"); do{motor_back(EN1, EN2, ENA);}while(digitalRead(A2)==LOW);}
            motor_stop(EN1, EN2); C=1; O_step = N_step;}
        else { motor_stop(EN1, EN2);digitalWrite(13, HIGH); delay(15000); }
    
        Serial.print ("O_step = "); Serial.println (O_step); 
        motor_stop(EN1, EN2);
        Serial.println ("                                     END"); 
        }C=0;
        digitalWrite(13, LOW);
    }   //  loop END
 
 long distance_return(int trig, int echo)   // 거리 측정
{
  long duration, distance;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = duration / 29.1 / 2 ;
  return distance; 
} 

void motor_go(int EN1, int EN2, int ENA)    // 정방향 
{ digitalWrite(EN1, HIGH);
  digitalWrite(EN2, LOW); 
  analogWrite(ENA, 200); }

void motor_back(int EN1, int EN2, int ENA)  // 정지
{ digitalWrite(EN1, LOW);
  digitalWrite(EN2, HIGH); 
  analogWrite(ENA, 200);} 

void motor_stop(int EN1, int EN2)           // 역방향
{ digitalWrite(EN1, LOW);
  digitalWrite(EN2, LOW);
  analogWrite(ENA, 0); 
  Serial.println("stop"); }
