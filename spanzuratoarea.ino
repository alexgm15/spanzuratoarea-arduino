#include <LiquidCrystal_I2C.h>
#define MAX_ROWS 3
#define MAX_COLS 3
#define MAX_CHRS 16
#define MAX_WORDS 6
#define MAX_CHANCES 6

LiquidCrystal_I2C lcd(0x27,16,2);

int indxWordToGuess = random(0, MAX_WORDS);
int guessedLetters = 0;
int chancesLeft = MAX_CHANCES;
int row;
int col;
int i,j,k;
int foundColumn = 0;
int debounce = 300;
int cntPressedBtns[9];
int rowsPins[MAX_ROWS];
int colsPins[MAX_COLS];

char initChars[8];
char chrToPrint;
int pressedButton;
int prevPressedBtn;
int start_game;
String words[MAX_WORDS];
int discoveredLetters[MAX_CHRS];

String substrToDisplay(String msg, int start) {
  String substrToDisplay;
  int cntCh = 0;
  for (int i = start; i < msg.length(); i++) {
    if (msg.charAt(i) == ' ') {
      cntCh = i - start;
    }
    if (i - start + 1 > MAX_CHRS) {
      break;
    }
    if (i == msg.length() - 1) {
      int j = i;
      while (j > start && msg.charAt(j) != ' ') {
        cntCh++;
        j--;
      }
      cntCh++;
    }
  }
  substrToDisplay = msg.substring(start, start + cntCh);
  
  return substrToDisplay;
}

void displayMessage(String msg, int delayMsg){
  int strLen = msg.length();
  int chsDisplayed = 0;
  while(chsDisplayed < strLen){
    String returnedSubstr = substrToDisplay(msg, chsDisplayed);
    chsDisplayed += returnedSubstr.length();
    chsDisplayed++;

    lcd.setCursor(0,0);
    lcd.print(returnedSubstr);

    if (chsDisplayed < strLen) {
      String returnedSubstr = substrToDisplay(msg, chsDisplayed);
      chsDisplayed += returnedSubstr.length();
      chsDisplayed++;
      
      lcd.setCursor(0,1);
      lcd.print(returnedSubstr);
      if (delayMsg == 1) {
        delay(2500);
        lcd.clear();
      }
    } else {
      if (delayMsg == 1) {
        delay(2500);
        lcd.clear();
      }
    }
  }
}

void setup() {
  int aRead = analogRead(0);
  indxWordToGuess = aRead % MAX_WORDS;
  
  rowsPins[0] = 2;
  rowsPins[1] = 3;
  rowsPins[2] = 4;
  
  colsPins[0] = 8;
  colsPins[1] = 9;
  colsPins[2] = 10;

  pressedButton = 0;
  start_game = 0;
  for (i = 0; i < words[indxWordToGuess].length(); i++) {
    discoveredLetters[i] = 0;
  }
  
  for (row = 0; row < MAX_ROWS; row++) {
    pinMode(rowsPins[row], OUTPUT);
  }
  for (col = 0; col < MAX_COLS; col++) {
    pinMode(colsPins[col], INPUT_PULLUP);
  }
  
  for(row = 0; row < MAX_ROWS; row++){
    digitalWrite(rowsPins[row], HIGH);
  }

  prevPressedBtn = 10;

  lcd.init();
  lcd.clear();         
  lcd.backlight();
  
  displayMessage("Bine ati venit!", 1);
  displayMessage("Acest proiect simuleaza jocul de SPANZURATOAREA", 1);
  displayMessage("Poti gresi o litera de maxim 6 ori", 1);
  displayMessage("Mult succes!", 1);
  
  initChars[0] = 'A';
  initChars[1] = 'D';
  initChars[2] = 'G';
  initChars[3] = 'J';
  initChars[4] = 'M';
  initChars[5] = 'P';
  initChars[6] = 'S';
  initChars[7] = 'V';

  words[0] = "TRIAJ";
  words[1] = "ARAHNIDA";
  words[2] = "OBSCUR";
  words[3] = "MICROPROCESOR";
  words[4] = "SCULA";
  words[5] = "FANTASTIK";
}

void printChrToTheLCD() {
    lcd.clear();
    for (i = 0; i < words[indxWordToGuess].length(); i++) {
      lcd.setCursor(i,0);
      if (discoveredLetters[i] == 1) {
        lcd.print(words[indxWordToGuess][i]);
      }
      else{
        lcd.print('_');
      }
    }

    lcd.setCursor(0,1);
    lcd.print("=== ");
    
    lcd.setCursor(4,1);
    lcd.print(chrToPrint);
    
    lcd.setCursor(5,1);
    lcd.print(" ===");
}

void selectLetter(){
  if (pressedButton != prevPressedBtn) {
    cntPressedBtns[prevPressedBtn] = 0;
  }
  if (prevPressedBtn != 8) {
    prevPressedBtn = pressedButton;
  
    chrToPrint = initChars[pressedButton];
    chrToPrint += cntPressedBtns[pressedButton];
    printChrToTheLCD();
  } else {
    prevPressedBtn = -1;
    chrToPrint = '_';
    printChrToTheLCD();
  }
  
  cntPressedBtns[pressedButton]++;
  // Penultima tasta permite selectarea literelor VWXYZ
  if ((cntPressedBtns[pressedButton] == 3 && pressedButton != 7) || 
      (cntPressedBtns[pressedButton] == 5 && pressedButton == 7)) {
    cntPressedBtns[pressedButton] = 0;
  }
  delay(debounce);  
  // turn to millis
}

void loop(){
   if (start_game == 0) {
     displayMessage("Apasa ENTER pentru a juca", 0);
   } else if (start_game == 2) {
     displayMessage("Apasa ENTER pt a juca din nou!", 0);
   }
   
   for (row = 0; row < MAX_ROWS; row++) {
      digitalWrite(rowsPins[row], LOW);
      for (col = 0; col < MAX_COLS; col++) {
         foundColumn = digitalRead(colsPins[col]);
         pressedButton = 3 * row + col;
         if (foundColumn == LOW && pressedButton != 8 && start_game > 0){
            selectLetter();
         } else if (foundColumn == LOW && pressedButton == 8 && (start_game == 0 || start_game == 2)) {
            if (start_game == 0) {
              start_game = 1;
            } else if (start_game == 2) {
              start_game = 3;
            }
            lcd.clear();
            for (i = 0; i < words[indxWordToGuess].length(); i++) {
              lcd.setCursor(i,0);
              lcd.print('_');
            }
            
            lcd.setCursor(0,1);
            lcd.print("=== ");
            
            lcd.setCursor(4,1);
            lcd.print('_');
            
            lcd.setCursor(5,1);
            lcd.print(" ===");
            
            delay(debounce);
            // turn to millis
         } else if (foundColumn == LOW && pressedButton == 8 && (start_game == 1 || start_game == 3)) {
            if (prevPressedBtn != 8) {
              lcd.clear();
              displayMessage("Se verifica...", 1);

              bool isLetterGood = false;
              for (i = 0; i < words[indxWordToGuess].length(); i++) {
                if (chrToPrint == words[indxWordToGuess][i]) {
                  discoveredLetters[i] = 1;
                  guessedLetters++;
                  isLetterGood = true;
                }
              }

              if (guessedLetters == words[indxWordToGuess].length()) {
                start_game = 2;
                for (int i = 0; i < MAX_CHRS; i++) {
                  discoveredLetters[i] = 0;
                }
                guessedLetters = 0;
                indxWordToGuess = random(0, MAX_WORDS);
                prevPressedBtn = 10;
                displayMessage("Felicitari! Ai supravietuit jocului!", 1);
                chancesLeft = MAX_CHANCES;
                break;
              }
              
              if (isLetterGood) {
                displayMessage("Ai ghicit o litera", 1);
                
              } else {
                // se va pierde o parte a corpului
                chancesLeft--;
                String displayChances = String("Mai ai ");
                displayChances.concat(chancesLeft);
                displayChances.concat(" vieti");
                displayMessage("Ai grija! Ti s-a taiat o parte a corpului", 1);
                displayMessage(displayChances, 1);
                if (chancesLeft == 0) {
                  start_game = 2;
                  for (int i = 0; i < MAX_CHRS; i++) {
                    discoveredLetters[i] = 0;
                  }
                  guessedLetters = 0;
                  indxWordToGuess = random(0, MAX_WORDS);
                  prevPressedBtn = 10;
                  displayMessage("Esti oale si ulcele!", 1);
                  break;
                }
              }
              
              prevPressedBtn = 8;
              selectLetter();
            }
         }
      }
      digitalWrite(rowsPins[row], HIGH);
   }
}
