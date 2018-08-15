int motor = 5;
const int SENSOR_MOTOR=2;
volatile int state = LOW;
int a[6];
int i=0;
int h;
void motor()
{
  state = !state;
  int sign_int = digitalRead(SENSOR_MOTOR);
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
  pinMode(motor, OUTPUT);
  attachInterrupt(0, motor, CHANGE);
}
 
void loop()
{
if (h==3){
  digitalWrite(motor, state); 
}
else {
  digitalWrite(motor, LOW);
}
}
 
