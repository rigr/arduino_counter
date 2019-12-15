/* Counter
 *  
 *  on arduino uno
 *  I use this for a pickup winder
 * 
 * Connect a I2C display to the arduino to read the counter 
 * 
 * Use one pin to count - can be a reed switch or whatever pulls to ground
 * Use another pin connected to a momentary switch (to ground) to reset counter
 * 
 * 
 */

#include <LiquidCrystal_I2C.h>

int counter_pin = 2;
int reset_pin = 3;

int counter_pin_State;                 // variable to store counter_pin state
int last_counter_pin_State;            // variable to store last counter_pin state
int reset_pin_State;                   // variable to store reset_pin state
int last_reset_pin_State;              // variable to store last reset_pin state

long counter;

bool bPress = false;
bool reset_state = false;



LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address


void setup() {
  // Serial port setup, some info-output
  Serial.begin(9600);
  Serial.println("Arduino_LCD_out on A4 sda, A5 scl, "); //
  Serial.println("counter in to 2, reset_pin to 3"); //
  

  lcd.begin(16, 2);  // initialize the lcd for 16 chars 2 lines, turn on backlight

  for (int i = 0; i < 2; i++)  //two blinks
  {
    lcd.backlight();
    delay(50);
    lcd.noBacklight();
    delay(50);
  }
  
  lcd.backlight(); // finish with backlight on
  

  //-------- Write characters on the display ------------------
  // NOTE: Cursor Position: (CHAR, LINE) start at 0
  lcd.setCursor(0, 0); //Start at character 0 on line 0
  lcd.print("Arduino  Counter");
  lcd.setCursor(0, 1);
  lcd.print("                ");
     

  delay(1000);
  

  pinMode (counter_pin, INPUT);
  pinMode (reset_pin, INPUT);
  digitalWrite(counter_pin, HIGH);
  digitalWrite(reset_pin, HIGH);

}

void loop() {

     checkUp();
     checkReset();
     

     if( bPress){
      bPress = false;
      lcd.setCursor(0, 0); //Start at character 0 on line 0
      lcd.print("Arduino  Counter");
      lcd.setCursor(0, 1);
      lcd.print(counter);            
     }

     if( reset_state){
      reset_state = false;
      lcd.clear();
      lcd.setCursor(0, 0); //Start at character 0 on line 0
      lcd.print("Arduino  Counter");
      lcd.setCursor(0, 1);
      lcd.print(counter);            
     }
}


void checkUp()
{
  counter_pin_State = digitalRead(counter_pin);

  if (counter_pin_State != last_counter_pin_State) {
    // if the state has changed, increment the counter
    if (counter_pin_State == LOW) {
        bPress = true;
      // if the current state is HIGH then the button went from off to on:
      counter++;
      Serial.println("on");
      Serial.print("number of button pushes: ");
      Serial.println(counter);
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("off");
    }
    // Delay to avoid bouncing
    delay(50);
  }
  // save current state as the last state
  last_counter_pin_State = counter_pin_State;
}



void checkReset()
{
  reset_pin_State = digitalRead(reset_pin);

  // compare the buttonState to its previous state
  if (reset_pin_State != last_reset_pin_State) {
    // if the state has changed, increment the counter
    if (reset_pin_State == LOW) {
        reset_state = true;
      // if the current state is HIGH then the button went from off to on:
      counter = 0;
      Serial.println("reset was pressed");
      Serial.print("number of button pushes: ");
      Serial.println(counter);
    } else {
      // if the current state is LOW then the button went from on to off:
      Serial.println("reset not pressed");
    }
    // Delay to avoid bouncing
    delay(50);
  }
  // save current state as the last state
  last_reset_pin_State = reset_pin_State;
}

