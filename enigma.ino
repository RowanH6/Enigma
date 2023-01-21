
// Built by Rowan Honeywell
// Replica Enigma Machine
// January 15, 2021


// Library attachments
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD declaration
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Pin Declarations
int buttonPin = 7;
int clearPin = 6;
int potentiometerPin = A0;

// Integer declarations
int lcdLocation = 0;
int messageLocation = 0;

// Boolean declarations
boolean lcdActivation = false;

// Array declarations
char alphabet[33] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '.', '!', '?', 'S', 'D', 'R', ' '};
int takeAlphabet[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};
int randomAlphabet[26];
int recordedMessage[16];

void setup() {
  // Pin activation
  pinMode(buttonPin, INPUT);
  pinMode(clearPin, INPUT);
  pinMode(potentiometerPin, INPUT);

  // LCD activation
  lcd.begin(16, 2);
  lcd.init();
  lcd.backlight();

  // Prints the starting message
  StartMessage();

  // Serial monitor activation
  Serial.begin(9600);
}

void loop() {

  // Integer to read the value that the potentiometer returns
  int potRead = analogRead(potentiometerPin);

  // Integers to read the value that the buttons return
  int buttonState = digitalRead(buttonPin);
  int clearState = digitalRead(clearPin);

  // Checks if the lcd and randomization sequence hasn't been activated yet
  if (lcdActivation == false)
  {
    // Prints instructions
    lcd.setCursor(0, 0);
    lcd.print("SELECT BUTTON 1");
    lcd.setCursor(0, 1);
    lcd.print("TO START");

    // If the user selects button 1, the program starts
    if (clearState == 1)
    {

      // Clears the screen and randomizes the alphabet
      ClearScreen();

      Randomize();

      // Confirms that the program has been activated
      lcdActivation = true;

      delay(1000);
    }
  }

  // If the program has been activated
  else
  {
    // Maps the potentiometer values to match that of the length of the alphabet
    potRead = map(potRead, 0, 1023, 0, 32);

    // Indicates what letter the user can select
    lcd.setCursor(0, 1);
    lcd.print(alphabet[potRead]);

    // If the user selects 'SPACE'
    if (potRead == 29)
    {
      lcd.setCursor(5, 1);
      lcd.print("       ");
      lcd.setCursor(1, 1);
      lcd.print("PACE");

      // Checks if the user has confirmed to select 'SPACE'
      if (buttonState == 1)
      {
        // Moves the cursor by one
        lcdLocation++;

        // Records that a space was selected and updates the length of the message
        recordedMessage[messageLocation] = 32;
        messageLocation++;

        delay(1000);
      }
    }

    // If the user is selecting the 'decode' command
    else if (potRead == 30)
    {
      lcd.setCursor(1, 1);
      lcd.print("ECODE");

      if (buttonState == 1)
      {
        lcd.setCursor(0, 0);
        lcd.print("                ");

        for (int i = 0; i <= messageLocation - 1; i++)
        {
          lcd.setCursor(i, 0);
          lcd.print(alphabet[recordedMessage[i]]);
        }
      }
    }

    // If the user is selecting the 'reset' command
    else if (potRead == 31)
    {

      // Shows what the user is selecting
      lcd.setCursor(5, 1);
      lcd.print("       ");
      lcd.setCursor(1, 1);
      lcd.print("ESET");

      // If they select the command
      if (buttonState == 1)
      {

        // Clears the top row of the LCD
        lcd.setCursor(0, 0);
        lcd.print("                ");

        // Resets the takeAlphabet array
        for (int i = 0; i < 26; i++)
        {
          takeAlphabet[i] = i;
          Serial.println(takeAlphabet[i]);
        }

        // Sets the letters back to the start of the first row
        messageLocation = 0;
        lcdLocation = 0;

        // Randomizes the alphabet
        Randomize();
      }
    }

    // If the user is not selecting a function, this confirms that the selected letter is the only character visible on the bottom row
    else
    {
      lcd.setCursor(1, 1);
      lcd.print("     ");
    }

    // If the user selects a letter (not a special character or 'SPACE')
    if (buttonState == 1 && potRead <= 25)
    {

      // Records the letter that was selected and updates the length of the message
      recordedMessage[messageLocation] = potRead;
      messageLocation++;

      // Sets the cursor to the location following the last letter
      lcd.setCursor(lcdLocation, 0);

      // Prints based on the letter value at its randomly matched value
      lcd.print(alphabet[randomAlphabet[potRead]]);

      // Updates the location
      lcdLocation++;

      delay(1000);
    }

    // If the user selects a special character
    else if (buttonState == 1 && potRead > 25 && potRead < 29)
    {

      // Records the special character selected and updates the length of the message
      recordedMessage[messageLocation] = potRead;
      messageLocation++;

      // Sets the cursor to the location following the last letter
      lcd.setCursor(lcdLocation, 0);

      // Prints based on the non-random character value
      lcd.print(alphabet[potRead]);

      // Updates the location
      lcdLocation++;

      delay(1000);
    }

    // If the user selects the clear button
    if (clearState == 1)
    {
      // Clears the top row of letters
      lcd.setCursor(0, 0);
      lcd.print("                ");

      // Resets the recorded message
      messageLocation = 0;

      // Resets the location to zero
      lcdLocation = 0;
    }
  }
}


// Method to randomize the alphabet
void Randomize()
{
  // Integer which controls the location of the new values in the randomAlphabet array
  int pickCounter = 0;

  // Random value that picks where it will pull a value from in the array
  int randValue;

  // Runs the function for each letter of the alphabet
  for (int i = 26; i > 0; i--)
  {

    // Random declarations

    // Chooses one of three random generator
    int randomGen = random(3);

    // Chooses one of twenty six random letters
    int randomGen1 = random(26);
    int randomGen2 = random(26);
    int randomGen3 = random(26);

    // Controls which random generator gets picked
    switch (randomGen)
    {
      case 0:
        randValue = randomGen1;
        break;
      case 1:
        randValue = randomGen2;
        break;
      case 2:
        randValue = randomGen3;
        break;
    }

    // Checks that the value picked has not been picked yet
    if (takeAlphabet[randValue] != 30)
    {

      // Assigns the next place in the randomAlphabet array to the new random value
      randomAlphabet[pickCounter] = takeAlphabet[randValue];

      // Turns the picked value into a number that the generator will not pick again
      takeAlphabet[randValue] = 30;

      // Updates the location of the randomAlphabet
      pickCounter++;
    }

    // Checks if that value that is chosen has already been picked
    else
    {

      // Adds one back to the for loop to make up for the wasted loop
      i++;
    }
  }
}


// Method to print the starting message
void StartMessage()
{

  // Prints the 'WELCOME TO ENIGMA' message
  lcd.setCursor(0, 0);
  lcd.print("WELCOME TO");
  lcd.setCursor(0, 1);
  lcd.print("ENIGMA");

  //After 3 seconds, the screen is cleared
  delay(3000);

  // Runs the clear screen method
  ClearScreen();
}

// Method to clear the LCD screen
void ClearScreen()
{
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print("                ");
}
