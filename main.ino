#include<LiquidCrystal.h>
#include<DHT.h>
#define DHTTYPE DHT11
int MQ135pin=13,DHTpin=4,Buzzer=2,LEDg1=23,LEDg2=22,LEDy1=18,LEDy2=3,LEDr1=21,LEDr2=19,LEDcon=15,rs=12,en=14,d4=26,d5=25,d6=33,d7=32;
DHT dht(DHTpin,DHTTYPE);
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
unsigned long totaltime;
long initial_delay=180000;


void led_off(int pin1, int pin2, int pin3, int pin4)
{
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
  digitalWrite(pin3, LOW);
  digitalWrite(pin4, LOW); 
}


void setup()
{
  Serial.begin(9600);
  pinMode(MQ135pin,INPUT);
  pinMode(LEDg1,OUTPUT);
  pinMode(LEDg2, OUTPUT);
  pinMode(LEDy1, OUTPUT);
  pinMode(LEDy2, OUTPUT);
  pinMode(LEDr1, OUTPUT);
  pinMode(LEDr2, OUTPUT);
  pinMode(LEDcon, OUTPUT);  
  pinMode(Buzzer, OUTPUT);
  lcd.begin(16,2);
  dht.begin();  
}


void loop()
{
  totaltime=millis();
  if(totaltime>initial_delay)
  {
    digitalWrite(Buzzer, LOW);
    digitalWrite(LEDcon, LOW);
    int humidity=0;
    lcd.clear();
    lcd.setCursor(0,0);
    float MQ135r=analogRead(MQ135pin);
    MQ135r=map(MQ135r,0,4095,0,1023);
    Serial.print("  Analog value: ");
    Serial.println(MQ135r);
    humidity=dht.readHumidity();
    float celciusTemp=dht.readTemperature();
    int hiC=dht.computeHeatIndex(celciusTemp,humidity,false);
    if(isnan(MQ135r) || isnan(humidity) || isnan(celciusTemp) || isnan(hiC))
    {
      Serial.println("Failed to read sensor data!");
      digitalWrite(LEDcon, HIGH);
    }
    if(MQ135r<=400)
    {
      digitalWrite(LEDg1, HIGH);
      digitalWrite(LEDg2, HIGH);
      led_off(LEDy1, LEDy2, LEDr1, LEDr2);
      lcd.print("VeryGood Quality");
    }
    else if(MQ135r>400 && MQ135r<=600)
    {
      digitalWrite(LEDg1, HIGH);    
      digitalWrite(LEDg2, LOW);
      led_off(LEDy1, LEDy2, LEDr1, LEDr2);
      lcd.print("Good Quality");
    }
    else if(MQ135r>600 && MQ135r<=700)
    {
      digitalWrite(LEDy1, HIGH);
      digitalWrite(LEDy2, LOW);
      led_off(LEDg1, LEDg2, LEDr1, LEDr2);
      lcd.print("Average Quality");
    }
    else if(MQ135r>700 && MQ135r<=800)
    {
      digitalWrite(LEDy1, HIGH);
      digitalWrite(LEDy2, HIGH);
      led_off(LEDg1, LEDg2, LEDr1, LEDr2);
      lcd.print("Poor Quality");
    }
    else if(MQ135r>800 && MQ135r<=950)
    {
      digitalWrite(LEDr1, HIGH);
      digitalWrite(LEDr2, LOW);
      led_off(LEDy1, LEDy2, LEDg1, LEDg2);
      lcd.print("Bad Quality");
    }
    else
    {
      digitalWrite(LEDr2, HIGH);
      digitalWrite(LEDr1, HIGH);
      led_off(LEDy1, LEDy2, LEDg1, LEDg2);
      lcd.print("EVACUATE!!!!!");    
      digitalWrite(Buzzer, HIGH);
    }  
    lcd.setCursor(0,1);
    lcd.print("RH:");
    lcd.print(humidity);
    lcd.print("% Temp:");
    lcd.print(hiC);
    lcd.print(" C");
    delay(1000);
  }
  else
  {
    lcd.setCursor(0,0);
    lcd.print(".....MQ135......");
    lcd.setCursor(0,1);
    lcd.print("...warming up...");
  }
}