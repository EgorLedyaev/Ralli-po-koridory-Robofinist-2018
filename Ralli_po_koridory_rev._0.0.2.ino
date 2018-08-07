#include <Wire.h>
#include <Servo.h>

const int TRIGPIN 11; //DDRB = B00100000
const int ECHOPIN 12; //DDRB = B01000000
const int TRIGPIN_2 10; //DDRB = B00010000
const int ECHOPIN_2 8; //DDRB = B00000100

const int SERVOPIN 13; //DDRB = B10000000

const int SENSOR 7; //DDRD = B1000000

#define RANGE_MAX 100

#define LINE_CONST 1000

byte Power = 0;
byte Cal = 0;
byte c=0;
byte Stop_line = 0x00

volatile unsigned int CYCLE = 0; 

int cm, cm_old,cm_2_old,cm_2;

unsigned int v_start = 100;
unsigned int     v_mot;
unsigned int s_angle = 90;

unsigned long currentMillis =0;
unsigned long previousMillis = 0;

int       P ;      
float      kp = 3;
unsigned int Analog = 600;

String inputString = "";
boolean stringComplete = false;
 
void setup() 
{
   //Serial.begin (115200);
  //inputString.reserve(30);

  DDRB = B00100000 //uPinMode(TRIGPIN, OUTPUT);
  DDRB = B01000000 //uPinMode(ECHOPIN, INPUT);
  DDRB = B00010000 //uPinMode(TRIGPIN_2, OUTPUT);
  DDRB = B00000100 //uPinMode(ECHOPIN_2, INPUT);

  myservo.attach(SERVOPIN);
  motorConnection(1,1);
  myservo.write (90);
}

void BUTTON_ON()
{
 if (uDigitalRead(BUTTON_ON) == HIGH)
{
  delay(300);
  if(uDigitalRead(BUTTON_ON) == HIGH)
  {
    if (Power == 1)
    {
      Power = 0;
    }
    else
    {
      Power = 1;
    }
    }
  } 
}

void loop() 
{
  currentMillis = millis();

  BUTTON_ON();

if (Power > 0)
{
  cm = ECHO1(4);
  if (cm > RANGE_MAX)
  {
    cm = RANGE_MAX;
  }

  delay(20);

  cm_2 = ECHO2(4)
  if (cm_2 > RANGE_MAX)
  {
    cm_2 = RANGE_MAX;
  }
  
#ifdef DBG_RANGE
  Serial.print(cm);
  Serial.print(" cm ");
  Serial.print(cm_2);
  Serial.println(" cm ");
#endif

  P = (cm - cm_2) * kp;
  
  
 // if     (P > 100)       {s_angle = HIGH_ANGLE_M; v_mot = v_start;}
  //else if (P <= 100&& P>= 20) {s_angle = LOW_ANGLE_M;  v_mot = v_start;}
 // else if (P < 20 && P>=0 )   {s_angle = ZERO_ANGLE;   v_mot = v_start;}  
  //else if (P > -20&& P <=0)   {s_angle = ZERO_ANGLE;   v_mot = v_start;}
  //else if (P >= -100 && P <=-20){s_angle = LOW_ANGLE_P;  v_mot = v_start;}
  //else if  P < -100       {myservo.write (0);   m1 = 150;}
  //else              {s_angle = HIGH_ANGLE_P; v_mot = v_start;}
  
#ifdef DBG_SERVO
  Serial.print(" P = ");
  Serial.println(P);
  Serial.print(" Angle = ");
  Serial.print(s_angle);
#endif

myservo.write(s_angle);

drive (v_mot,v_mot);

#ifdef DBG_MOTOR
  Serial.print("v_mot = ");
  Serial.println(v_mot);
  drive(0,0);
#endif

  STOP_LINE_DETECT();
  
if (Stop_line == 0xff)
{
  Serial.println(" Line Detect ! ");

#ifdef DBG_MOTOR
  drive(-200,-200);
  delay(300);
#endif

  drive(0,0);
  delay(2000);
  Stop_line = 0x00;

#ifdef DBG_MOTOR
  drive(220,220);
  delay(2000);
#endif 
  }
else
{
  drive(0,0);
  myservo.write(90);
}
  Serial.println(currentMillis - previousMillis);
  previousMillis = currentMillis;

if(Cal == 1)
{
  kp = analogRead(KP_PIN);
  Analog = analogRead(AN_PIN);
  v_start = analogRead(V_PIN);
  
  Serial.print("SENSOR_L = ");
  Serial.println(analogRead(SENSOR_L));
  
  kp = kp/255;          //Значение от 0 до 4
  //Analog = Analog/400;
  v_start = v_start/4;      // 0-250

  Serial.print("kp = ");
  Serial.println(kp);
  Serial.print("An = ");
  Serial.println(Analog); 
  Serial.print("v_start = ");
  Serial.println(v_start);  
  
  delay(300);
  
}
}

unsigned int ECHO1(byte Cycles)
{
  unsigned int duration=0,i=0,cm ;
  
  for (i=0;i<Cycles;i++)      // Накопление данных
  {
  // Считываем дальномер
    PORTB = B00000000 //uDigitalWrite(TRIGPIN, LOW); //DDRB = B00100000 
    delayMicroseconds(2);
    PORTB = B00100000 //uDigitalWrite(TRIGPIN, HIGH); //DDRB = B00100000
    delayMicroseconds(10); 
    PORTB = B00000000 //uDigitalWrite(TRIGPIN, LOW); //DDRB = B00100000
  
    cm = pulseIn(ECHOPIN, HIGH, 80000); // получаем длину
    cm=cm/58;            // переводим в см
    
    duration +=cm;         // накапливаем
    
    #ifdef DBG_ECHO
     Serial.println(duration);
    #endif 
  }

  duration = duration/Cycles;    // усредняем

  return (int)duration;       // возвращаем 
}

unsigned int ECHO2(byte Cycles)
{
  unsigned int duration_2=0,i=0,cm;
  
  for (i=0;i<Cycles;i++)
  {
    PORTB = B00000000 //uDigitalWrite(TRIGPIN_2, LOW); //DDRB = B00010000
    delayMicroseconds(2);
    PORTB = B00010000 //uDigitalWrite(TRIGPIN_2, HIGH); //DDRB = B00010000
    delayMicroseconds(10); 
    PORTB = B00000000 //uDigitalWrite(TRIGPIN_2, LOW); //DDRB = B00010000
    cm = pulseIn(ECHOPIN_2, HIGH, 80000);
    cm=cm/58;
    
    duration_2 = duration_2+cm;  
    #ifdef DBG_ECHO
     Serial.println(duration_2);
    #endif
  }

  duration_2 = duration_2/Cycles;

  return (int)duration_2; 
}  
/***** Детектирование стоп линии. Отключение робота при наезде на черную линию
    duration задает время отслеживания линии (помехоустойчивость)
    Используются цифровые датчики.
****/
void STOP_LINE_DETECT (void)
{
  
  
  unsigned int var;
  var = analogRead(SENSOR_L); 
  
  #ifdef DBG_LINE
  Serial.print    ("SENSOR_L = ");  
  Serial.println  (var);
  #endif
  
  
  // если обнаружен черный цвет, то
  if(var>Analog)
  {
   
    
    c++;              // инкремент переменной
    // если долго черная линия, то значит это стоп линия.
    if (c>LINE_CONST)
    {
      Stop_line = 255;
      c=0;
    } 
  }
  else 
  {
    c=0;
  }
}
}
