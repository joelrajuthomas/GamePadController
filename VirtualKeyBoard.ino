/*Start */
const byte key_down = 9; 
const byte key_up = 17;
const byte key_left = 15;
const byte key_right = 14;
const byte click_press = 11;

volatile byte state = LOW;

/*define variable use in the program*/
byte data = B0000;

/*Sending data to PC */
String data_Sending;
String Analog_sending;

char receiveVal;

/*define analog input */
int analogY = 26;

int analogX = 2;

String x_analog;
String y_analog;

int x_val = 0;

int y_val = 0;
int buttonstate = 0;
void setup() 
{

  /*Initially the program */
  
  /*initialize rgb led output pin */
 
  /*initialize input/output sensor */
  pinMode(key_down,INPUT_PULLUP);
  digitalWrite(key_down, LOW);
  pinMode(key_up,INPUT_PULLUP);
  digitalWrite(key_up, LOW);
  pinMode(key_left,INPUT_PULLUP);
  digitalWrite(key_left, LOW);
  pinMode(key_right,INPUT_PULLUP);
  digitalWrite(key_right, LOW);
  pinMode(click_press,INPUT);
  digitalWrite(click_press, LOW);
  
  attachInterrupt(digitalPinToInterrupt(key_up), up, FALLING );
  attachInterrupt(digitalPinToInterrupt(key_down), down, FALLING );
  attachInterrupt(digitalPinToInterrupt(key_left), left, FALLING );
  attachInterrupt(digitalPinToInterrupt(key_right), right, FALLING );
  attachInterrupt(digitalPinToInterrupt(click_press), mouseClick, FALLING );

  /*initialize analog input */
  
  /*initialize the UART */
  Serial.begin(115200);
//  Serial.print("PLEASE WORK");

  
}

void loop()
{
  x_val = analogRead(analogX); 
  x_analog = String(x_val); 
  delay(100);
  
  y_val = analogRead(analogY);
  y_analog = String(y_val); 
  Analog_sending =  x_analog + " " + y_analog; 
//  Serial.print(Analog_sending);
  Serial.print(Analog_sending);
  
  delay(500);

//  buttonstate = digitalRead(key_up);
//  if(buttonstate == HIGH)
//  {
//    Serial.print("UP");
//  }
}

/** Interupt event of key#1 */
void up()
{
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 if (interrupt_time - last_interrupt_time > 200) 
 {
    digitalWrite(key_up, LOW);

     state = !state; 
     data_Sending = "UP";
     Serial.print(data_Sending);
 }
 last_interrupt_time = interrupt_time;
}

/** Interupt event of key#2 */
void down()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200) 
  {
      digitalWrite(key_down, LOW);
     state = !state; 
     data_Sending = "DOWN";
     Serial.print(data_Sending);
  }
  last_interrupt_time = interrupt_time;
}

void left()
{
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 if (interrupt_time - last_interrupt_time > 200) 
 {
    digitalWrite(key_left, LOW);

     state = !state; 
     data_Sending = "LEFT";
     Serial.print(data_Sending);
 }
 last_interrupt_time = interrupt_time;
}

void right()
{
 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 if (interrupt_time - last_interrupt_time > 200) 
 {
    digitalWrite(key_right, LOW);

     state = !state; 
     data_Sending = "RIGHT";
     Serial.print(data_Sending);
 }
 last_interrupt_time = interrupt_time;
}

void mouseClick()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  if (interrupt_time - last_interrupt_time > 200) 
  {
     digitalWrite(click_press, LOW);
     state = !state; 
     data_Sending = "CLICK";
     Serial.print(data_Sending);
  }
  last_interrupt_time = interrupt_time;
}


/*END */
