#include<LiquidCrystal.h>
#include<DHT.h>
#define DHTTYPE DHT11
int MQ135pin=13,DHTpin=4,Buzzer=2,LEDg1=23,LEDg2=22,LEDy1=18,LEDy2=3,LEDr1=21,LEDr2=19,LEDcon=15,rs=12,en=14,d4=26,d5=25,d6=33,d7=32;
DHT dht(DHTpin,DHTTYPE);
LiquidCrystal lcd(rs,en,d4,d5,d6,d7);
unsigned long totaltime;
long initial_delay=180000;


void change_LED_state(uint8_t vg1, uint8_t vg2, uint8_t vy1, uint8_t vy2, uint8_t vr1, uint8_t vr2)
{
  digitalWrite(LEDg1, vg1);
  digitalWrite(LEDg2, vg2);
  digitalWrite(LEDy1, vy1);
  digitalWrite(LEDy2, vy2);
  digitalWrite(LEDr1, vr1);
  digitalWrite(LEDr2, vr2); 
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
      change_LED_state(HIGH,HIGH,LOW,LOW,LOW,LOW);
      lcd.print("VeryGood Quality");
    }
    else if(MQ135r>400 && MQ135r<=600)
    {
      change_LED_state(HIGH,LOW,LOW,LOW,LOW,LOW);
      lcd.print("Good Quality");
    }
    else if(MQ135r>600 && MQ135r<=700)
    {
      change_LED_state(LOW,LOW,HIGH,LOW,LOW,LOW);
      lcd.print("Average Quality");
    }
    else if(MQ135r>700 && MQ135r<=800)
    {
      change_LED_state(LOW,LOW,HIGH,HIGH,LOW,LOW);
      lcd.print("Poor Quality");
    }
    else if(MQ135r>800 && MQ135r<=950)
    {
      change_LED_state(LOW,LOW,LOW,LOW,HIGH,LOW);
      lcd.print("Bad Quality");
    }
    else
    {
      change_LED_state(LOW,LOW,LOW,LOW,HIGH,HIGH);
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