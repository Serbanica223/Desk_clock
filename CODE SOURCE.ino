#include <Wire.h>
#include <Adafruit_GFX.h>
#include <RTClib.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>



float floatMap(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

LiquidCrystal_I2C lcd_i2c(0x26, 20, 4); // I2C address 0x27, 20 column and 4 rows
LiquidCrystal_I2C nume(0x27, 16, 2); 

RTC_DS1307 rtc;


char daysOfTheWeek[7][12] = {
  "Duminica",
  "Luni",
  "Marti",
  "Miercuri",
  "Joi",
  "Vineri",
  "Sambata"
};

#define SCREEN_WIDTH 128 // OLED width,  in pixels
#define SCREEN_HEIGHT 64 // OLED height, in pixels
#define DHTPIN 4
#define DHTTYPE DHT22

int lastState1 = HIGH;
int currentState;


int oka = 1;
int okb = 1;
int p = 5;
int STOP = HIGH;

  int Ora_alarma = 0;
  int Minut_alarma = 0;



DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);

  

  lcd_i2c.init(); 
  lcd_i2c.backlight();
  nume.init(); 
  nume.backlight();

  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1);
  }



  rtc.adjust(DateTime(2023, 5, 29, 12, 20, 0));
  dht.begin();

  pinMode(32, INPUT_PULLUP);
  pinMode(33, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
  pinMode(34, INPUT_PULLUP);
  pinMode(18, OUTPUT);

  delay(2000);         
            
  
  nume.setCursor(0,1);
  nume.print("SERBAN xx GORCEA");
}

void loop() {
  
  
  

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false);

  DateTime now = rtc.now();
  if(oka==1 && okb==1)
  {
  
  lcd_i2c.setCursor(0, 0); 
    lcd_i2c.print(now.hour(), DEC);
    lcd_i2c.print(':');
    lcd_i2c.print(now.minute(), DEC);
    lcd_i2c.print(':');
    if(now.second()<10)
    lcd_i2c.print(0);
    lcd_i2c.print(now.second(), DEC);

  lcd_i2c.setCursor(0, 3); 

  lcd_i2c.print(now.day(), DEC);
  lcd_i2c.print('/');
  lcd_i2c.print(now.month(), DEC);
  lcd_i2c.print('/');
  lcd_i2c.print(now.year(), DEC);
  lcd_i2c.print("->");
  lcd_i2c.print(daysOfTheWeek[now.dayOfTheWeek()]);

  lcd_i2c.setCursor(10, 1);
  lcd_i2c.print("Temp:");
  lcd_i2c.print(t);

  lcd_i2c.setCursor(10, 2); 
  
  lcd_i2c.print("Umid:");
  lcd_i2c.print(h);
  
  }


  else if(oka==0 && okb==1)
  {
    
    
    lcd_i2c.setCursor(7, 0);
    lcd_i2c.print("ALARMA");
    lcd_i2c.setCursor(5, 1);

    lcd_i2c.print(Ora_alarma, DEC);
    lcd_i2c.print(':');
    if(Minut_alarma<10)
    lcd_i2c.print(0);
    lcd_i2c.print(Minut_alarma, DEC);

    lcd_i2c.setCursor(p, 3); 
    lcd_i2c.print("^");

    
    if(p==5)
    {
      int analogValue = analogRead(13);
      Ora_alarma = floatMap(analogValue, 0, 4095, 0, 23);
      
    }
    else
    {
      int analogValue = analogRead(13);
      Minut_alarma = floatMap(analogValue, 0, 4095, 0, 59);

    }

    

    currentState = digitalRead(14);
    
    if(currentState == LOW)
  { delay(200);
    if(p==5)
    p=8;
    else p=5;
    currentState = HIGH;
    lcd_i2c.clear();
  }


  }

  else if(oka==1 && okb==0)
  { 
    
    lcd_i2c.setCursor(7, 0);
    lcd_i2c.print("SETARE");
    lcd_i2c.setCursor(5, 1); 
    lcd_i2c.print(now.hour(), DEC);
    lcd_i2c.print(':');
    if(now.minute()<10)
    lcd_i2c.print(0);
    lcd_i2c.print(now.minute(), DEC);

    lcd_i2c.setCursor(p, 3); 
    lcd_i2c.print("^");

    float Ora;
    float Minut;

    if(p==5)
    {
      int analogValue = analogRead(13);
      Ora = floatMap(analogValue, 0, 4095, 0, 23);
      
    }
    else
    {
      int analogValue = analogRead(13);
      Minut = floatMap(analogValue, 0, 4095, 0, 59);

    }

    rtc.adjust(DateTime(2023, 5, 29, Ora, Minut, 9));

    currentState = digitalRead(14);
    
    if(currentState == LOW)
  { delay(200);
    if(p==5)
    p=8;
    else p=5;
    lcd_i2c.clear();
    currentState = HIGH;
  }

    

  }




  int a = digitalRead(32);
  int b = digitalRead(33);



//-----------------PENTRU SET
  if( b == LOW)
  { delay(1000);
    b = HIGH;
    
    if(okb==1)
    okb=0;
    else
    okb=1;
    lcd_i2c.clear();
  }
//---------------------------





//----------PENTRU ALARMA
  if( a == LOW)
  { delay(1000);
    a = HIGH;

    if(oka==1)
    oka=0;
    else
    oka=1;
    lcd_i2c.clear();
  }

  //-----------------------
  
  nume.setCursor(0,0);
  nume.print("Alarma: ");
  nume.print(Ora_alarma, DEC);
  nume.print(':');

  nume.print(Minut_alarma, DEC);

  if(Ora_alarma == now.hour() && Minut_alarma == now.minute())
  {
  ledcWriteTone(0,800);
  delay(1000);
  uint8_t octave = 1;
  ledcWriteNote(0,NOTE_C,octave);  
  delay(1000);
  }

  }
  

