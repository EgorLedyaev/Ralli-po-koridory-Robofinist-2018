#include <Servo.h> // подключаем библиотеку для работы с сервоприводом 
const int TRIG_P_F=4; 
const int ECHO_P_F = 5; 
const int ECHO_P_R = 12; 
const int TRIG_P_R = 11; 
const int ECHO_P_L = 6; 
const int TRIG_P_L = 9; 
const unsigned int W_TUNEL = 10; //ширина тунеля лабиринта 
Servo servo; // объявляем переменную servo типа "servo" 

int servo_pin = 8; 
int js_position = 90; 
int max_position = 180; 
int min_position = 0; 

void setup() // процедура setup 
{ 
servo.attach(servo_pin, min_position ,max_position ); 

pinMode(TRIG_P_F, OUTPUT); 
pinMode(ECHO_P_F, INPUT); 
pinMode(TRIG_P_R, OUTPUT); 
pinMode(ECHO_P_R,INPUT); 
pinMode(TRIG_P_L,OUTPUT); 
pinMode(ECHO_P_L,INPUT); 

//выставляем руль в изначальную позицию 
servo.write(js_position); 
delay(700); 



} 

unsigned int distantion_1(byte trig, byte echo) 
{ 
int duration, distance; 
digitalWrite(trig,LOW); 
delayMicroseconds(2); 
digitalWrite(trig, HIGH); 
delayMicroseconds(10); 
digitalWrite(trig, LOW); 
duration = pulseIn(echo,HIGH); 
distance = duration/58; 
return(int)distance; 
} 

void loop() // процедура loop 
{ 
unsigned int L, R, D; 
//считываем расстояние с правого и левого сенсора 
L = distantion_1(TRIG_P_L, ECHO_P_L); 
R = distantion_1(TRIG_P_R, ECHO_P_R); 
//рассчитываем разницу 
D = L - R; 

if (D> W_TUNEL / 2) 
{ 
servo.write(0); 

}else if (D<(-W_TUNEL/2)){ 
servo.write(180); 
} 
else{ 
servo.write(90); 
} 

}
