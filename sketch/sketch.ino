#include<Adafruit_SSD1306.h>
#include<Adafruit_GFX.h>
#include<Wire.h>
//#include<Keypad.h>
#include<EEPROM.h>
#include<Servo.h>
#include<MFRC522.h>
#include<SPI.h>
#define sspin 6
#define PCF8574_ADDR 0x20
#define rstpin 5
#define swidth 128
#define sheight 64
#define tpin 9
#define epin 8
int count;
Servo myservo;
bool enter = false;
int currentCard = -1;
char enteredPIN[5];
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'},
};
int  bpin = 2, lpin = 7;
int servopin = 3;
int pirpin = 10 , val;          
unsigned long cmillis;  
unsigned long ultramillis , pirmillis;   
int ultratime = 500, pirtime = 20 , buzztime = 2000 , ledtime = 2000;
long duration ; 
float distance ;
byte uid1[] = {0xAA, 0xBB, 0xCC, 0xDD};
byte uid2[] = {0x01, 0x02, 0x03, 0x04};
byte uid3[] = {0x11, 0x22, 0x33, 0x44};
byte uid4[] = {0x55, 0x66, 0x77, 0x88};
MFRC522 rfid(sspin, rstpin);
Adafruit_SSD1306 display(swidth , sheight , &Wire , -1);
byte pinIndex = 0;
void led()
{
    digitalWrite(lpin, HIGH);
    delay(ledtime);
    digitalWrite(lpin, LOW);

}
void buzzer()
{
    digitalWrite(bpin,HIGH);
    delay(buzztime);
    digitalWrite(bpin, LOW);
}
void writePCF8574(byte data)
{
  Wire.beginTransmission(PCF8574_ADDR);
  Wire.write(data);
  Wire.endTransmission();
}
byte readPCF8574()
{
  Wire.requestFrom(PCF8574_ADDR,1);
  if(Wire.available())
  {
    return Wire.read();
  }
  return 0xFF;
}
char getKeyFromPCF8574()
{
  for (int row = 0; row < 4; row++)
  {
    byte output = 0xFF;
    // Activate one row
    output &= ~(1 << row);
    // Send row state to PCF8574
    writePCF8574(output);
    // Read the PCF8574
    byte input = readPCF8574();
    // Check columns
    for (int col = 0; col < 4; col++)
    {
      if (!(input & (1 << (col + 4))))
      {
        return keys[row][col];
      }
    }
  }

  return '\0';
}
void showPIN()
{
  display.clearDisplay();

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("ENTER PIN:");

  display.setCursor(0, 30);

  for (byte i = 0; i < pinIndex; i++)
  {
    display.print(enteredPIN[i]);
    display.print(" ");
  }

  display.display();
}
void Activation()
{
  if(cmillis - pirmillis >= pirtime)
  {
    pirmillis = cmillis;
    val = digitalRead(pirpin);
  }
  if (cmillis - ultramillis >= ultratime)
  {
    ultramillis = cmillis ;
    digitalWrite(tpin , LOW);
    delayMicroseconds(2);
    digitalWrite(tpin , HIGH);
    delayMicroseconds(10);
    digitalWrite(tpin , LOW);
    duration = pulseIn(epin, HIGH);
    distance = (duration * 0.0343) / 2;
    if(distance <= 80 && val == HIGH && !enter)
    {
      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 20);
      display.println("SCAN CARD");
      display.display();
    }
  }
}
bool checkUID(byte *storedUID)
{
    for (byte i = 0; i < 4; i++)
    {
        if (rfid.uid.uidByte[i] != storedUID[i])
            return false;
    }

    return true;
}
bool rfidFunction()
{
  if (!rfid.PICC_IsNewCardPresent())
    return false;

  if (!rfid.PICC_ReadCardSerial())
    return false;

  currentCard = -1;

  if (checkUID(uid1))
  {
    currentCard = 0;
  }
  else if (checkUID(uid2))
  {
    currentCard = 1;
  }
  else if (checkUID(uid3))
  {
    currentCard = 2;
  }
  else if (checkUID(uid4))
  {
    currentCard = 3;
  }

  if (currentCard != -1)
  {
    // Authorized RFID found
    enter=true;

    pinIndex = 0;
    enteredPIN[0] = '\0';

    showPIN();

    return true;
  }
  else
  {
    // Unknown RFID
    enter= false;

    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(0, 20);
    display.println("DENIED");
    display.display();

    return false;
  }
}
void pwd()
{
  bool correct = false;

  if (currentCard == 0 && strcmp(enteredPIN, "1234") == 0)
    correct = true;

  else if (currentCard == 1 && strcmp(enteredPIN, "2345") == 0)
    correct = true;

  else if (currentCard == 2 && strcmp(enteredPIN, "3456") == 0)
    correct = true;

  else if (currentCard == 3 && strcmp(enteredPIN, "4567") == 0)
    correct = true;

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 20);

  if (correct)
  {
    display.println("AUTHORIZED");
    count=0;
    myservo.write(90);
    delay(2000)
    myservo.write(0);
    display.display();
    delay(2000);
    enter = false;
    currentCard = -1;
    pinIndex = 0;
    enteredPIN[0] = '\0';
    display.clearDisplay();
    display.display();
  }
  else
  {
    display.println("DENIED");
    count++;
    display.display();
    delay(1500);

    if (count >= 3)
    {
      //3 WRONG ATTEMPTS -> ALARM,LED

      buzzer();
      led();

      // Reset entire security session
      count = 0;
      enter = false;
      currentCard = -1;
      pinIndex = 0;
      enteredPIN[0] = '\0';

      display.clearDisplay();
      display.setTextSize(2);
      display.setCursor(0, 20);
      display.println("LOCKED");
      display.display();

      delay(2000);

      display.clearDisplay();
      display.display();
    }

    else
    {
      // Give the same person another chance
      pinIndex = 0;
      enteredPIN[0] = '\0';

      showPIN();
    }
  }
}
void setup() {
  Serial.begin(9600);
  Wire.begin();
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("OLED not found"));
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 20);
  display.display();
  Wire.beginTransmission(PCF8574_ADDR);

  byte error = Wire.endTransmission();

  if (error == 0)
  {
    Serial.println(F("PCF8574 found!"));
  }
  else
  {
    Serial.print(F("PCF8574 NOT found. I2C error: "));
    Serial.println(error);
  }
  myservo.attach(servopin);
  myservo.write(0);
  SPI.begin();
  rfid.PCD_Init();
  //pinMode(servopin , OUTPUT);
  pinMode(lpin , OUTPUT);
  pinMode(bpin , OUTPUT);
  pinMode(pirpin , INPUT);
  pinMode(tpin , OUTPUT);
  pinMode(epin , INPUT);
}

void loop()
{
  cmillis = millis();

  Activation();

  rfidFunction();

  char key = getKeyFromPCF8574();

  if (enter)
  {
    if (key != '\0')
    {
      Serial.print(F("Key Pressed: "));
      Serial.println(key);

      if (key >= '0' && key <= '9')
      {
        if (pinIndex < 4)
        {
          enteredPIN[pinIndex] = key;
          pinIndex++;
          enteredPIN[pinIndex] = '\0';

          showPIN();
        }
      }

      else if (key == '*')
      {
        if (pinIndex > 0)
        {
          pinIndex--;
          enteredPIN[pinIndex] = '\0';

          showPIN();
        }
      }

      else if (key == '#')
      {
        if (pinIndex == 4)
        {
          Serial.print(F("PIN Entered: "));
          Serial.println(enteredPIN);

          pwd();
        }
      }
      delay(100);
    }
  }
}