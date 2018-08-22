#include <Servo.h>
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
  js_position = 2300;
  motor.write(js_position);
}
}
 
