//Если дальномер будет работать нестабильно даже на средних расстояниях,
//то рекомендуется подключить электролитический конеденсатор емкостью
//не менее 100 мкф между красным и черным проводом дальномера соблюдая полярность.

#include <Servo.h>

Servo servo;
Servo motor;

#define SENSOR_L 3 // подключение левого датчика расстояния к 3 порту
#define SENSOR_R 4 // подключение правого датчика расстояния к 4 порту

const int SENSOR_led=2; // подключение датчика обнаружения линии к 2 порту
volatile int state = LOW;
int a[6];
int i=0;
int h;

int       P ;      
float      kp = 3;
unsigned int Analog = 600;

#define RANGE_MAX 100

int moto_pin = 9;    //Подключаем левый мотор к пину 7
int js_position = 1500;  //Начальная позиция, всегда 1.5 мс для регуляторов бесколлекторных двигателей
int max_position = 2300; //Максимальное значение ШИМ 2.3 мс
int min_position = 800;  //Минимальное значени ШИМ 0.8 мс
int start = 1;  //Флаг задержки запуска

long duration, duration_F, duration_L, duration_R,distance_F;
int distance_L, distance_R

void sonic_L();
{
float volts_L = analogRead(SENSOR_L)*0.0048828125; // считываем напряжение с дальномера
int distance_L = 13*pow(volts_L, -1); // переводим значение напряжение в расстояние 
delay(500);
}

void sonic_R();
{
float volts_R = analogRead(SENSOR_R)*0.0048828125; // считываем напряжение с дальномера
int distance_R = 13*pow(volts_R, -1); // переводим значение напряжение в расстояние
}

void motor_stop()
{
  state = !state;
  int sign_int = digitalRead(SENSOR_led);
  Serial.println(!sign_int,'\n');
  if(!sign_int==1)
  {
  a[i]=!sign_int;
  i++;
  }
  for (int i = 0; i < 6; i++)
  {
 Serial.print(a[i]); // печатаем каждый из 5 элементов массива
}
Serial.print('\n');
h = a[0]+a[1]+a[2]+a[3]+a[4]+a[5];
Serial.print(h);
Serial.print('\n');
if(i == 6)
{
  memset(a, 0, 12); // обнуляем массив
  i = 0;
  }
}

void setup()
{
Serial.begin(9600);

servo.attach(10);

motor.attach(moto_pin, js_position, max_position);
attachInterrupt(0, motor_stop, CHANGE);
}

void loop()
{
P = (distance_L - distance_R) * kp;

Serial.print(distance_L);
Serial.print(distance_R);

if(start == 1)
{
motor.write(js.position);
delay(700);
start = 0;
}

if (h == 3)
{
js_position = 0;
motor.write(js_position);
}
else
{
if (P > 100)
{
s_angle = HIGH_ANGLE_M;
v_mot = v_start;
}
else if (P <= 100&& P>= 20)
{
s_angle = LOW_ANGLE_M;
v_mot = v_start;
}
else if (P < 20 && P>=0 )
{
s_angle = ZERO_ANGLE;
v_mot = v_start;
}  
else if (P > -20&& P <=0)
{
s_angle = ZERO_ANGLE;
v_mot = v_start;
}
else if (P >= -100 && P <=-20)
{
s_angle = LOW_ANGLE_P;
v_mot = v_start;
}
else if (P < -100) 
{
myservo.write (0);
m1 = 150;
}
else
{
s_angle = HIGH_ANGLE_P; 
v_mot = v_start;
}
}
