  //Передний датчик ECHO = 2 TRIG = 3 , Правый датчик ECHO = 12 TRIG = 11
  //левый echo 4, trig 5
#include <Servo.h> // подключаем библиотеку для работы с сервоприводом

const int TRIG_P_F=3;
const int ECHO_P_F = 2;
const int ECHO_P_R = 12;
const int TRIG_P_R = 11
const int ECHO_P_L = 4
const int TRIG_P_L = 5


Servo servo; // объявляем переменную servo типа "servo"
int servo_pin = 8;
int js_position = 0;
int max_position = 180;
int min_position = 0;

void setup() // процедура setup
{
servo.attach(servo_pin, js_position ,max_position ); 
}
{
  Serial.begin(9600)
  pinMode(TRIG_P_F, OUTPUT);
  pinMode(ECHO_P_F, INPUT);
  pinMode(TRIG_P_R, OUTPUT);
  pinMode(ECHO_P_R,INPUT);
  pinMode(TRIG_P_L,OUTPUT);
  pinMode(ECHO_P_L,INPUT);
}
void distantion_1()
{
  int duration, distance;
digitalWrite(TRIG_P_F,LOW);
delayMicroseconds(2);
digitalWrite(TRIG_P_F, HIGH);
delayMicroseconds(TRIG_P_F, LOW);
duration = pulseIn(ECHO_P_F,HIGH);
distance = duration/58;
}

  void distation_2()
  {
  int duration_2, distance_2;
digitalWrite(TRIG_P_R,LOW);
delayMicroseconds(2);
digitalWrite(TRIG_P_R, HIGH);
delayMicroseconds(TRIG_P_R, LOW);
duration_2 = pulseIn(ECHO_P_R,HIGH);
distance_2 = duration_2/58;
}
  void distation_3()
  {
  int duration_3, distance_3;
digitalWrite(TRIG_P_l,LOW);
delayMicroseconds(2);
digitalWrite(TRIG_P_l, HIGH);
delayMicroseconds(TRIG_P_l, LOW);
duration_3 = pulseIn(ECHO_P_l,HIGH);
distance_3 = duration_3/58; 
}
  void js_position()
  {
  if(start == 1);
  {
servo.write(js_position);

delay(700);
start = 0;

}
js_position=distation_1(A0);

js_position=map(js_position,0,300,0,250);
constration 
delay(20);
}
void loop() // процедура loop
 



