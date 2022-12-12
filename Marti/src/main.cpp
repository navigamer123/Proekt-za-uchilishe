#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeMonoOblique9pt7b.h>
#include <DHT.h>
#include <MQ135.h>
#define SCREEN_WIDTH 128 //дефииниране на екраната ширина
#define SCREEN_HEIGHT 64 //дефиниране на екраната височина

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET); //Създаваме променлива от класа с параметри височина,ширина,вид на прехвъряне на дани и кой да е ресет пина

#define sensor    A0 // сензора за измерване на газ
#define DHTPIN 2          // Digital pin 2
#define DHTTYPE DHT11     // DHT 11

DHT dht(DHTPIN, DHTTYPE); //създаваме променлима от клас с която измерваме температурата и влажноста в въздоха
MQ135 gasSensor = MQ135(sensor); //създаваме променлива от класа за четене на гас сензора
String quality =""; //променлива от типа стринг с която пишем качеството на въздоха
float gasLevel = 0;         //int variable za nivoto na gas

float h =0;  //променлива от типа флоат за да може да показваме дробни числа
float t = 0; //променлива от типа флоат за да може да показваме дробни числа


/*!
 *  @brief  прочита дачиците и ги пише на екрана
 *  @param     
 *					 
 *	@return  none
 */
void sendSensor()
{
  h = dht.readHumidity(); //четем дачика за влажноста
  t = dht.readTemperature();//четем дачика за температура
  gasLevel = (int)gasSensor.getPPM(); //прочитаме даните oт сензора за гас и ги пишеме на променливата гасЛежел
  if (isnan(h)||isnan(t)|| isnan(gasLevel))//проверяваме дали някоя от променливите е равна на NaN или not a number ако да опита пак
  {
   return;
  }
  Serial.println(h);
  Serial.println(t);
  if (gasLevel < 1810)
  {
     // quality = "  Добро.";
     quality = "Good";
  }
  else
  { 
      if (gasLevel >= 1810 && gasLevel < 2250)
      {
         // quality = "  Лошо.";
         quality = "Poor";
      }
      else 
      {
         if (gasLevel >= 2250 && gasLevel <= 3000)
         {
            // quality = "Много лошо!";
            quality = "Very poor";
         }
         else if (gasLevel > 3000 && gasLevel <= 3500)
         {
            //quality = "Има пожар в стаята!";
            quality = "Smoke in the room";
         }
         else
         {
            quality = "To late for you";
         }
      }
  }
  display.clearDisplay();//Чистиме буфера на екрана
  display.setFont();//слагаме дефалтния фонт
  display.setTextSize(1); // правиме размера на екрана на 1
  display.setCursor(0, 43);//слагеме курсора да почне да пише от 0 по x и 43 по y
  display.print("Temp:");//принтираме температурата
  display.print(t);
  display.println(" C");
  display.print("RH: ");
  display.print((double)h,2);//принтираме влажноста
  display.print(" %");
  display.setTextSize(1);  
  display.setCursor(1,5);
  display.println(F("Air Quality Monitor v1.0:"));
  display.println(" ");
  display.setTextSize(1);
  display.print(quality); 
  display.display();
}


void setup() {
  Serial.begin(9600);
  Serial.println(F("Program is running"));
  pinMode(sensor,INPUT);
  dht.begin();
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed")); //Ако не засече заче екрана
}
  display.clearDisplay();//чисти екрана
  display.setTextColor(WHITE);//слага цвета на бял
  
  display.setTextSize(1);
  display.setCursor(50, 0);
  display.println("Air");
  display.setTextSize(1);
  display.setCursor(23, 20);
  display.println("Qulaity monitor");
  display.display();
  delay(1200);
  display.clearDisplay();
  
  display.setTextSize(1);
  display.setFont(&FreeMonoOblique9pt7b);
  display.setCursor(20, 10);
  display.println("By");
  display.println("Ivan");
  display.println("and Martin");
  display.display();
  delay(2000);
  display.clearDisplay();    
  display.setFont();
}

void loop() 
{
   
   sendSensor(); 
   delay(4000);
}
