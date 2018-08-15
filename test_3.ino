#include <Servo.h> // подключаем библиотеку для работы с сервоприводом 
const int TRIG_P_F = 4; 
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

int motor = 5; 
const int SENSOR_MOTOR = 2; 
volatile int state = LOW; 
int a[6]; 
int i = 0; 
int h; 

void motor_f() 
{ 
state = !state; 
int sign_int = digitalRead(SENSOR_MOTOR); 
Serial.println(!sign_int, '\n'); 
if (!sign_int == 1) 
{ 
a[i] = !sign_int; 
i++; 
} 
for (int i = 0; i < 6; i++) 
{ 
Serial.print(a[i]); // печатаем каждый из 5 элементов массива 
} 
Serial.print('\n'); 
h = a[0] + a[1] + a[2] + a[3] + a[4] + a[5]; 
Serial.print(h); 
Serial.print('\n'); 
if (i == 6) 
{ 
memset(a, 0, 12); // обнуляем массив 
i = 0; 
} 
} 

void setup() // процедура setup 
{ 
servo.attach(servo_pin, min_position , max_position ); 

pinMode(TRIG_P_F, OUTPUT); 
pinMode(ECHO_P_F, INPUT); 
pinMode(TRIG_P_R, OUTPUT); 
pinMode(ECHO_P_R, INPUT); 
pinMode(TRIG_P_L, OUTPUT); 
pinMode(ECHO_P_L, INPUT); 

//выставляем руль в изначальную позицию 
servo.write(js_position); 
delay(700); 

Serial.begin(9600); 
pinMode(motor, OUTPUT); 
attachInterrupt(0, motor_f, CHANGE); 
} 

unsigned int distantion_1(byte trig, byte echo) 
{ 
int duration, distance; 
digitalWrite(trig, LOW); 
delayMicroseconds(2); 
digitalWrite(trig, HIGH); 
delayMicroseconds(10); 
digitalWrite(trig, LOW); 
duration = pulseIn(echo, HIGH); 
distance = duration / 58; 
return (int)distance; 
} 

void loop() // процедура loop 
{ 
unsigned int L, R; 
int D, D_C; 
int D_A; 
//считываем расстояние с правого и левого сенсора 
L = distantion_1(TRIG_P_L, ECHO_P_L); 
R = distantion_1(TRIG_P_R, ECHO_P_R); 
//рассчитываем разницу 
D = L - R; 
D_C = constrain(D, -15, 15); 
D_A = map(D_C, -15, 15, 60, 120); 
servo.write(D_A); 




/*if (D > W_TUNEL / 2) 
{ 
servo.write(0); 

} else if (D < (-W_TUNEL / 2)) { 
servo.write(180); 
} 
else { 
servo.write(90); 
}*/ 


if (h == 3) { 
digitalWrite(motor, state); 
} 
else { 
digitalWrite(motor, LOW); 
} 

}
