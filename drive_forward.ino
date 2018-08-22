#include <Servo.h>
Servo motor;
const int trigPin_F = 7;
const int echoPin_F = 8;
const int mot_pin = 9;    //Подключаем левый мотор к пину 7
int js_position = 1500;  //Начальная позиция, всегда 1.5 мс для регуляторов бесколлекторных двигателей
//const int max_position = 2300; //Максимальное значение ШИМ 2.3 мс
//const int min_position = 800;  //Минимальное значени ШИМ 0.8 мс
int start = 1;  //Флаг задержки запуска
long duration, distance;

void sonic_F()
{
  digitalWrite(trigPin_F, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin_F, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin_F, LOW);
  duration = pulseIn(echoPin_F, HIGH);
  distance = duration/58;
}

void setup() 
{
  Serial.begin (9600);
  pinMode(trigPin_F, OUTPUT);
  pinMode(echoPin_F, INPUT);
  motor.attach(mot_pin);    //Инициальзация левого мотора (порт, начальная позиция, максимальная позиция)       !!!
} 

void loop() 
{
  
  //Начальная установка регуляторов в нулевое положение
  if(start == 1) {
    motor.write(js_position);
    
    delay(1500);
    start = 0;
  }
  sonic_F(); 
  js_position = distance;  //Считывание положения потенциометра
  js_position = map(js_position, 2, 4000, 1600, 1800);  //Преобразование расстояния в обороты
  
  if(distance > 10)//Если дистанция более 10см, то можно разгоняться как разрешает расстояние
  {

  motor.write(js_position);     //Записываем и отправляем на регулятор мотора
  }
  else //Совсем все плохо? 
  {
    js_position=0;
    motor.write(js_position);
  }
  Serial.print(distance);
  Serial.println(" см");
  Serial.println(js_position);
  delay(20);  //Задержка цикла 20 мс
}
