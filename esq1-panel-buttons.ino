/*
Read Ensoniq ESQ-1 panel buttons
Scott Metoyer, 2023
*/

typedef struct
{
    char* name;
    int downByte;
    int upByte;
    bool pressed;
}  Button;

Button buttons[8][7] = {
  { { "Cart B",   130,  2, false }, { "Data Down",  138,  10, false },  { "Soft 8",   146,  18, false }, { "Select",       154,  26, false },  { "Locate",   162,  34, false },  { "DCA 3",  170,  42, false },  { "Split/Layer",  178,  50, false } }, 
  { { "Cart A",   129,  1, false }, { "Data Up",    137,  9,  false },  { "Soft 7",   145,  17, false }, { "Create/Erase", 153,  25, false },  { "Control",  161,  33, false },  { "DCA 2",  169,  41, false },  { "Modes",        177,  49, false } }, 
  { { "Internal", 131,  3, false }, { "Write",      139,  11, false },  { "Soft 9",   147,  19, false }, { "Record",       155,  27, false },  { "Play",     163,  35, false },  { "LFO 2",  171,  43, false },  { "",             0,    0,  false } }, 
  { { "SEQ",      128,  0, false }, { "Compare",    136,  8,  false },  { "Soft 6",   144,  16, false }, { "Master",       152,  24, false },  { "MIDI",     160,  32, false },  { "DCA 1",  168,  40, false },  { "DCA 4",        176,  48, false } }, 
  { { "Bank 1",   132,  4, false }, { "Soft 1",     140, 12,  false },  { "Soft 10",  148,  20, false }, { "Storage",      156,  28, false },  { "OSC 1",    164,  36, false },  { "LFO 3",  172,  44, false },  { "",             0,    0,  false } }, 
  { { "Bank 2",   133,  5, false }, { "Soft 2",     141, 13,  false },  { "Soft 5",   149,  21, false }, { "Edit",         157,  29, false },  { "OSC 2",    165,  37, false },  { "Filter", 173,  45, false },  { "",             0,    0,  false } }, 
  { { "Bank 3",   134,  6, false }, { "Soft 3",     142, 14,  false },  { "LFO 1",    150,  22, false }, { "Mix/MIDI",     158,  30, false },  { "OSC 3",    166,  38, false },  { "ENV 4",  174,  46, false },  { "",             0,    0,  false } }, 
  { { "Bank 4",   135,  7, false }, { "Soft 4",     143, 15,  false },  { "ENV 2",    151,  23, false }, { "Stop/Cont",    159,  31, false },  { "ENV 1",    167,  39, false },  { "ENV 3",  175,  47, false },  { "",             0,    0,  false } } 
};

int rowPinState[8];
int colPinState[7];

void setup() {
  // Serial.begin(9600);
  Serial.begin(62500);
}

void loop() {
  readRowPins();
  readColumnPins();
  
  for(int i = 0; i < 8; i++) {
    for (int j = 0; j < 7; j++) {
      if (rowPinState[i] == LOW && colPinState[j] == LOW) {
        buttonPress(i, j);
      } else {
        buttonRelease(i, j);
      }
    }
  }

  delay(1);
}

void buttonPress(int row, int column) {
  if (buttons[row][column].pressed == false) {
    buttons[row][column].pressed = true;
    Serial.write(buttons[row][column].downByte);
    // Serial.println(buttons[row][column].downByte);  
  }
}

void buttonRelease(int row, int column) {
  if (buttons[row][column].pressed == true) {
    buttons[row][column].pressed = false;
    Serial.write(buttons[row][column].upByte);
    // Serial.println(buttons[row][column].upByte);  
  }
}

void readRowPins() {
  // Set the column pins to low
  for (int i = 10; i < 17; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }

  // Loop through the row pins and see if any are pressed (low)
  for (int j = 2; j < 10; j++) {
    pinMode(j, INPUT_PULLUP);
    int val = digitalRead(j);
    rowPinState[j - 2] = val;   
  }
}

int readColumnPins() {
  // Set the row pins to low
  for (int i = 2; i < 10; i++) {
    pinMode(i, OUTPUT);
    digitalWrite(i, LOW);
  }
   
  for (int j = 10; j < 17; j++) {
    pinMode(j, INPUT_PULLUP);
    int val = digitalRead(j);
    colPinState[j - 10] = val; 
  }
}

void printButton(int row, int column) {
    Serial.println(buttons[row][column].name);
}
