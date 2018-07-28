#include <Strela.h>
#include <Wire.h>
#include <Servo.h>

#define TRIGPIN   	P5	
#define ECHOPIN   	P6
#define TRIGPIN_2 	P3
#define ECHOPIN_2 	P4

#define SERVOPIN  	P1

#define KP_PIN			P11		 //
#define AN_PIN			P6		 //
#define V_PIN			P12		 //

//#define SENSOR_R        P6       //пин, к которому подключен первый датчик
#define SENSOR_L        P7        //пин, к которому подключен второй датчик

#define LINE_CONST 		1000

#define RANGE_MAX 100

//#define DBG_SERVO
//#define DBG_MOTOR
//#define DBG_ECHO
//#define DBG_LINE
#define DBG_RANGE

#define ZERO_ANGLE		90

#define LOW_ANGLE_M  	70
#define HIGH_ANGLE_M 	50

#define LOW_ANGLE_P		115
#define HIGH_ANGLE_P	150



#define BUTTON_ON   S3
#define BUTTON_CAL  S1

#define LED_POWER 	L3
#define LED_CAL 	L1

#define LED_LINE	L2

Servo myservo;  


byte Power = 0;					// Статус переменная для управления питанием робота
byte Cal = 0;
byte c=0;
byte Stop_line = 0

int cm, cm_old,cm_2_old,cm_2;

unsigned int v_start = 100;
unsigned int		 v_mot;
unsigned int s_angle = 90;

unsigned long currentMillis =0;
unsigned long previousMillis = 0;

int			 P ;			
float 		 kp = 3;			 // коэффициент разности
unsigned int Analog = 600;


String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete


void setup()
{
  Serial.begin (115200);
  inputString.reserve(30);
  
  uPinMode(TRIGPIN, OUTPUT);
  uPinMode(ECHOPIN, INPUT);
  uPinMode(TRIGPIN_2, OUTPUT);
  uPinMode(ECHOPIN_2, INPUT);
  
  myservo.attach(SERVOPIN);  	
  motorConnection(1,1);
  myservo.write (90);
  
  tone(BUZZER, 1000, 150);
  
} 
void loop()
{
	currentMillis = millis();	
	
if (uDigitalRead(BUTTON_ON) == HIGH)	
{
	delay(300); 
	if(uDigitalRead(BUTTON_ON) == HIGH)
		{
			if (Power == 1) {Power = 0;	   uDigitalWrite(LED_POWER,LOW);}
			else 			{Power = 1;	   uDigitalWrite(LED_POWER,HIGH);}		// Индикация режима работы робота
		}
}
	
if (uDigitalRead(BUTTON_CAL) == HIGH)	
{
	delay(300); 
	if(uDigitalRead(BUTTON_CAL) == HIGH)
		{
			if (Cal == 1) 	{Cal = 0; 	uDigitalWrite(LED_CAL,LOW);}
			else 			{Cal = 1; 	uDigitalWrite(LED_CAL,HIGH);}
		}
}	
	
if(Power>0)
{
// Считывание показаний УЗ 
	
	
	cm = ECHO1(4);
	if (cm > RANGE_MAX ) 	{cm = RANGE_MAX;} // ограничение диапазона
	
	delay(20);
	
	cm_2 =ECHO2(4);	
	if (cm_2 > RANGE_MAX ) 	{cm_2 = RANGE_MAX;}// ограничение диапазона
	
	//delay(10);
	
 // Вывод отладочной информации о расстоянии
  #ifdef DBG_RANGE
  Serial.print(cm); 
  Serial.print(" cm ");
  Serial.print(cm_2); 
  Serial.println(" cm ");
  #endif 
  
// delay(DELAY);
  
// Вычисление разности расстояний между дальномерами
  P=(cm-cm_2)*kp;
  
// Управление рулем а также скоростью тяговых моторов
// Выставляем угол поворота и скороость в переменные.

  if 	  (P > 100)				{s_angle = HIGH_ANGLE_M; v_mot = v_start;}
  else if (P <= 100&& P>= 20)	{s_angle = LOW_ANGLE_M;	 v_mot = v_start;}
  else if (P < 20 && P>=0 ) 	{s_angle = ZERO_ANGLE;   v_mot = v_start;}  
  else if (P > -20&& P <=0) 	{s_angle = ZERO_ANGLE;   v_mot = v_start;}
  else if (P >= -100 && P <=-20){s_angle = LOW_ANGLE_P;  v_mot = v_start;}
  //else if  P < -100 			{myservo.write (0);   m1 = 150;}
  else 							{s_angle = HIGH_ANGLE_P; v_mot = v_start;}

// Выводим отладку если надо
#ifdef DBG_SERVO
	Serial.print(" P= ");
	Serial.println(P);  
	Serial.print(" Angle = ");  
	Serial.println(s_angle); ;
#endif

	myservo.write	(s_angle);

	drive			(v_mot,v_mot);
#ifdef DBG_MOTOR	
	Serial.print    ("v_mot = ");	
	Serial.println  (v_mot);
	drive(0,0);
#endif
	STOP_LINE_DETECT();
if (Stop_line == 0)
{
	// отключить светоидод
	uDigitalWrite(LED_LINE,LOW);		
	Serial.println ("Line Detect!");
	// реверс двигателей, остановка
	#ifndef DBG_MOTOR
	drive(-200,-200);
	delay(300);
	#endif
	
	// отключение моторов. стоим 2 секунды
	drive(0,0);
	delay(2000);
	Stop_line = 0; 
	
	// движение вперед в течении 2 секунд
	#ifndef DBG_MOTOR
	drive(220,220);
	delay(2000);
	#endif
}
}
else 
{
	drive(0,0);
	myservo.write(90);

}

// раз в 10мс считывание датчиков линии 
/* if(currentMillis - previousMillis > 10)
{
	STOP_LINE_DETECT() ;			
	previousMillis = currentMillis ;
} */
	// delay(10);
	
// если обнаружена стоп линия

	Serial.println(currentMillis - previousMillis);
	previousMillis = currentMillis ;
	
if(Cal==1)
{
									//Значение от 0 до 2.5
	kp = analogRead(KP_PIN);
	Analog = analogRead(AN_PIN);
	v_start = analogRead(V_PIN);
	
	//Serial.print("SENSOR_R = ");
	//Serial.println(analogRead(SENSOR_R));
	Serial.print("SENSOR_L = ");
	Serial.println(analogRead(SENSOR_L));
	
	kp = kp/255;					//Значение от 0 до 4
	//Analog = Analog/400;
	v_start = v_start/4;			// 0-250

	Serial.print("kp = ");
	Serial.println(kp);
	Serial.print("An = ");
	Serial.println(Analog);	
	Serial.print("v_start = ");
	Serial.println(v_start);	
	
	delay(300);
/*********************************************************************************************/	
}
}

unsigned int ECHO1(byte Cycles)
{
	unsigned int duration=0,i=0,cm ;
	
	for (i=0;i<Cycles;i++)			// Накопление данных
	{
	// Считываем дальномер
	  uDigitalWrite(TRIGPIN, LOW); 
	  delayMicroseconds(2);
	  uDigitalWrite(TRIGPIN, HIGH); 
	  delayMicroseconds(10); 
	  uDigitalWrite(TRIGPIN, LOW);
	
	  cm = pulseIn(ECHOPIN, HIGH, 80000); // получаем длину
	  cm=cm/58;					   // переводим в см
	  
	  duration +=cm; 			   // накапливаем
	  
	  #ifdef DBG_ECHO
	   Serial.println(duration);
	  #endif 
	}

	duration = duration/Cycles;	   // усредняем

	return (int)duration; 			// возвращаем 
}

unsigned int ECHO2(byte Cycles)
{
	unsigned int duration_2=0,i=0,cm;
	
	for (i=0;i<Cycles;i++)
	{
	  uDigitalWrite(TRIGPIN_2, LOW); 
	  delayMicroseconds(2);
	  uDigitalWrite(TRIGPIN_2, HIGH); 
	  delayMicroseconds(10); 
	  uDigitalWrite(TRIGPIN_2, LOW);
		cm = pulseIn(ECHOPIN_2, HIGH, 80000);
		cm=cm/58;
		
	  duration_2 =duration_2+cm;  
	  #ifdef DBG_ECHO
	   Serial.println(duration_2);
	  #endif
	}

	duration_2 = duration_2/Cycles;

	return (int)duration_2; 
}  

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
		uDigitalWrite(LED_LINE,HIGH);		//светодиод вкл для индикации
		
		c++;							
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
		uDigitalWrite(LED_LINE,LOW);
	}
}
/*      Калибровка сенсора     */
