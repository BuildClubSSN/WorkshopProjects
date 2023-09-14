int trig=6;
int echo=5;
int duration;
int distcm;
int led1=13;
int led2=12;
int led3=11;
int led4=10;
int led5=9;

void setup()
{
  pinMode(trig,OUTPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3,OUTPUT);
  pinMode(led4,OUTPUT);
  pinMode(led5,OUTPUT);
  pinMode(echo,INPUT);
}

void turnoff()
{
    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    digitalWrite(led3,LOW);
    digitalWrite(led4,LOW);
    digitalWrite(led5,LOW);
}

void loop()
{
}
