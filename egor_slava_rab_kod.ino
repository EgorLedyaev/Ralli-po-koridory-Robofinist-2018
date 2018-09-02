#include <Servo.h> 
Servo servo1;
Servo motor;

const int SENSOR_led=2;
volatile int state = LOW;
int a[6];
int i=0;
int h;

int moto_pin = 9;    //Подключаем левый мотор к пину 7
int js_position = 1500;  //Начальная позиция, всегда 1.5 мс для регуляторов бесколлекторных двигателей
int max_position = 2300; //Максимальное значение ШИМ 2.3 мс
int min_position = 800;  //Минимальное значени ШИМ 0.8 мс
int start = 1;  //Флаг задержки запуска

const int trigPin_F = 7;
const int echoPin_F = 8;
const int trigPin_L = 5;
const int echoPin_L = 6;
const int trigPin_R = 3;
const int echoPin_R = 4;

long duration, duration_F, duration_L, duration_R,distance_F;
int distance_L, distance_R;

void sonic_L()
{
  digitalWrite(trigPin_L, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_L, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_L, LOW);
  duration_L = pulseIn(echoPin_L, HIGH);
}
void sonic_R()
{
  digitalWrite(trigPin_R, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_R, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_R, LOW);
  duration_R = pulseIn(echoPin_R, HIGH);
  
}

void sonic_F()
{
  digitalWrite(trigPin_F, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_F, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_F, LOW);
  duration_F = pulseIn(echoPin_F, HIGH);
  
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
void dist()
  {
    sonic_F();
    sonic_L();
    sonic_R();
    distance_F = duration_F/58;
    distance_L = duration_L/58;
    distance_R = duration_R/58;
    
  }
void setup() 
{
  Serial.begin (9600);
 /* pinMode(trigPin_F, OUTPUT);
  pinMode(echoPin_F, INPUT);
  */
  pinMode(trigPin_L, OUTPUT);
  pinMode(echoPin_L, INPUT);
  pinMode(trigPin_R, OUTPUT);
  pinMode(echoPin_R, INPUT);
  pinMode(trigPin_F, OUTPUT);
  pinMode(echoPin_F, INPUT);
  servo1.attach(10);
  
  motor.attach(moto_pin, js_position, max_position);
  attachInterrupt(0, motor_stop, CHANGE);
  } 

void loop()
{

    if(start == 1) {
    motor.write(js_position);
    delay(700);
    start = 0;
  }

if (h==3){
  js_position = 0;
  motor.write(js_position);
}
else {
  dist();
  switch(distance_F)
  {
    case 0 ... 9:
      js_position = 0;
      motor.write(js_position);
      break;
    case 10 ... 4000:
  js_position = 2000;//1550 - нестабильно , 1580- нормально 
  motor.write(js_position);
  if (distance_L==distance_R)//distance=0, если машина ровно по середине
  {
   servo1.write(115); //Положение колёс прямо
  }
  else if (distance_L<distance_R)//Отрицательное значение distance будет, если
  //машина ближе к левой стене.
  //поворачиваем вправо
  {
    servo1.write(129);
  }
  else
  {
    //оставшийся, неучтённый вариант,
    //когда distance является
    //положительным числом.
    //Значит приблизились к правой стене.
    //Поворачиваем влево
    servo1.write(70);

  } 
  break;
  }
  
  delay(10);
}
}
 
