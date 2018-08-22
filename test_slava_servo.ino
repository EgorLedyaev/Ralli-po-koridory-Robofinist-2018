#include <Servo.h> // подключаем библиотеку для работы с сервоприводом 
const int TRIG_P_F = 4; 
const int ECHO_P_F = 5; 
const int ECHO_P_R = 12; 
const int TRIG_P_R = 11; 
const int ECHO_P_L = 6; 
const int TRIG_P_L = 9; 
const int MAX_R_G = 180; 
const int MIN_L_G = 0; 
const int MIN_MS = 200; 
const int MAX_MS = 20*58; 
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

//Kalman filter for one dimmension sensor 
class KalmanFilter_1 
{ 
public: 
KalmanFilter_1(double q, double r, double f = 1, double h = 1); 

void Init(double value, double covariance); 
double NewValue(double data); 
double GetValue(); 

private: 
double Q, R, F, H; 
double X0, P0; 
double Value; 
double Covariance; 
}; 

KalmanFilter_1::KalmanFilter_1(double q, double r, double f = 1, double h = 1) 
{ 
Q = q; 
R = r; 
F = f; 
H = h; 

Init(0, 0); 
} 

void KalmanFilter_1::Init(double value, double covariance) 
{ 
Value = value; 
Covariance = covariance; 
} 

double KalmanFilter_1::NewValue(double data) 
{ 
double K; 

//Calculate prediction 
X0 = F * Value; 
P0 = F * Covariance * F + Q; 

//value update and correction 
K = H * P0 / (H * P0 * H + R); 
Value = X0 + K * (data - H * X0); 
Covariance = (1 - K * H) * P0; 

return Value; 
} 

double KalmanFilter_1::GetValue() 
{ 
return Value; 
} 

KalmanFilter_1 kalmanfilter_L(0.1, 4); 
KalmanFilter_1 kalmanfilter_R(0.1, 4); 


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
//attachInterrupt(0, motor_f, CHANGE); 
} 

unsigned int duration( byte TRIG,byte ECHO) 
{ 
int duration; 
digitalWrite(TRIG, LOW); 
delayMicroseconds(2); 
digitalWrite(TRIG, HIGH); 
delayMicroseconds(10); 
digitalWrite(TRIG, LOW); 
duration = pulseIn(ECHO, HIGH, 58000); 
return (int)duration; 
} 

void loop() // процедура loop 
{ 
unsigned int L, R; 
unsigned int K_L; 
unsigned int K_R; 
int D, D_C; 
int D_A; 


//считываем расстояние с правого и левого сенсора 
L = duration(TRIG_P_L, ECHO_P_L); 
delayMicroseconds(150); 
R = duration(TRIG_P_R, ECHO_P_R); 
delayMicroseconds(150); 

K_L = kalmanfilter_L.NewValue(L); 
K_R = kalmanfilter_R.NewValue(R); 

K_L = constrain(K_L, MIN_MS, MAX_MS); 
K_R = constrain(K_R, MIN_MS, MAX_MS); 
//коректеровка к нулевому значению 
K_L = K_L - MIN_MS; 
K_R = K_R - MIN_MS; 

//рассчитываем разницу 
D = K_L - K_R; 
D_A = map(D, -MAX_MS + MIN_MS, MAX_MS - MIN_MS,MIN_L_G,MAX_R_G); 

servo.write(D_A); 
//Serial.println(D); 

//печатаем дистанцию 
Serial.print(K_L);Serial.print(' ');Serial.print(K_R);Serial.print(' ');Serial.print(D_A); 
Serial.println(); 




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
