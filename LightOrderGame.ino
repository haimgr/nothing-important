/////////////////////////////////////////////
// LIGHT ORDER GAME
// By Haim Greenstein (C)
// Create in: 12.12.2015
/////////////////////////////////////////////

///////////////////////////////////
// Debug methods
// [set of simple debug function]
///////////////////////////////////

const bool DEBUG = true;

inline void debugInfo(const char * str, unsigned long value) {
  if (DEBUG) {
    Serial.print(str);
    Serial.print(" (");
    Serial.print(value);
    signed long signed_value = value;
    if (signed_value < 0) {
      Serial.print(" == ");
      Serial.print(signed_value);
    }
    Serial.println(")");
  }
}

inline void debugInfo(const char * str) {
  if (DEBUG) {
    Serial.print(str);
  }
}

//////////////////////
// Leds and Data
/////////////////////

const byte LED_NUM = 3;
// Array of LEDs, indicate to pins.
const byte LED[LED_NUM] = {12,11,10};
// A constant indicates all lads
const byte ALL_LEDS = -1;

// A constant indicates end of ORDER array
const byte ORDER_END = -1;
const byte MAX_ORDERS_SIZE = 7;
// array of 'order's -- values indicates to led/button indexes.
byte orders[MAX_ORDERS_SIZE+1];

// set led to HIGH or LOW. led_index refer to LEDs array or ALL_LEDS constant.
inline void setLed(byte led_index, bool value) {
  if (led_index != ALL_LEDS)
    digitalWrite(LED[led_index], value);
  else for (byte i = 0; i < LED_NUM; i++)
    digitalWrite(LED[i], value);
}

// On and off led several times.
//   set led to HIGH or LOW. led_index refer to LEDs array or ALL_LEDS constant.
void showBlink(byte led_index, int timeOn, int timeOff = 0, int times = 1) {
  while(times-- > 0) {
    //debugInfo("show blink", led_index);
    setLed(led_index, HIGH);
    delay(timeOn);
    setLed(led_index, LOW);
    delay(timeOff);
  }
}

// Current size of ordered array. 
//   (number of values until the ORDER_END value).
byte ordersSize() {
  byte count = 0;
  byte *orders_ptr = orders;
  while(*(orders_ptr++) != ORDER_END) count++;
  //for(int i=0; i < 255; i++) Serial.print(*orders_ptr);
  return count;
}

// delete all values in 'orders' array so the orderSize will be 0.
inline void resetOrder() {
  debugInfo("before", *orders);
  orders[0] = ORDER_END;
  debugInfo("afterre", *orders);
}

// order randomize values as number of order_size in orders array.
//   you should ensure that ordersSize() is not equals to MAX_ORDER_SIZE. 
void randOrder(byte order_size) {
  orders[order_size] = ORDER_END;
  while(order_size-- > 0) {
    orders[order_size] = random(LED_NUM);
    debugInfo("rand order led", orders[order_size]);
  }
  debugInfo("current order size", order_size);
}

// add new randomize value to the orders array.
//   you should ensure that ordersSize() is not equals to MAX_ORDER_SIZE. 
void addRandOrder() {
  byte order_index = ordersSize();
  orders[order_index] = random(LED_NUM);
  orders[order_index+1] = ORDER_END;
  debugInfo("add rand order led", orders[order_index]);
  debugInfo("current order size", order_index+1);
}

// show the order by blinking the opropporiated leds.
void showOrder() {
  for (byte *order_ptr = orders; *order_ptr != ORDER_END; order_ptr++) {
    debugInfo("show led", *order_ptr);
    showBlink(*order_ptr, 1000, 200);
  }
}


////////////////////
// Game Methods
////////////////////

const byte BTN_NUM = 3;
const byte BTN[BTN_NUM] {7,6,5};

const unsigned long TIMER_STOP = -1;
const unsigned long BUTTON_DELAY = 1000/60;
const unsigned long RESET_TIME = 60000;

enum { BLINK_MODE, PLAY_MODE};

unsigned long buttonDelayTimer[BTN_NUM];
unsigned long resetTimer = TIMER_STOP;

byte gameMode;
bool gameContinueType;
byte input_count;

void buttonOperation(byte btn_index) {
  if (gameMode == BLINK_MODE) {
    gameContinueType = btn_index != 2;
    playStart();
  }
  else if (gameMode == PLAY_MODE)
    nextPlayInput(btn_index);
}

void blinkModeLoop() {
  static byte blinkLed;
  static unsigned long blinkTimer;
  const unsigned long blinkSpeed = 1000/3;
  
  if (millis() > blinkTimer) {
    digitalWrite(LED[blinkLed], LOW);
    ++blinkLed %= LED_NUM;
    digitalWrite(LED[blinkLed], HIGH);
    blinkTimer = millis() + blinkSpeed;
  }
}

void playModeLoop() {
  if (millis() > resetTimer) {
    resetGame();
    return;
  }
}

inline void playStart() {
  debugInfo("playStart");
  gameMode = PLAY_MODE;
  resetOrder();
  playNext();
}

void playNext() {
  debugInfo("playStart");
  
  input_count = 0;
  
  showBlink(ALL_LEDS, 100, 90, 5);
  delay(800);

  byte order_size = ordersSize() + 1;
  // if orders array is full then end game
  if (order_size > MAX_ORDERS_SIZE) {
    debugInfo("game end");
    for(int i = 0; i < 76; i++)
      showBlink(random(LED_NUM), 100, 30);
    resetGame();
    return;
  }
  // next order
  if (gameContinueType)
    addRandOrder();
  else
    randOrder(order_size);
  showOrder();

  resetTimer = millis() + RESET_TIME;
}

void nextPlayInput(byte btn_index) {
  if (btn_index == orders[input_count]) {
    input_count++;
    if (input_count == ordersSize())
      playNext();
  }
  else
    resetGame();
}

void resetGame() {
  debugInfo("reset game");
  gameMode = BLINK_MODE;
  showBlink(ALL_LEDS, 2000);
}

////////////////////
// Main Methods
////////////////////

void setup() {
  for(byte i = 0; i < BTN_NUM; i++) {
    pinMode(BTN[i], INPUT);
    digitalWrite(BTN[i], HIGH);
    buttonDelayTimer[i] = TIMER_STOP;
  }

  for(byte i = 0; i < LED_NUM; i++) {
    pinMode(LED[i], OUTPUT);
  }
  
  randomSeed(millis());
  
  if (DEBUG) Serial.begin(9600);
}

void loop() {
  if (gameMode == BLINK_MODE)
    blinkModeLoop();
  else if (gameMode == PLAY_MODE)
    playModeLoop();
    
  for (byte i = 0; i < BTN_NUM; i++) {
    if (millis() > buttonDelayTimer[i]) {
      debugInfo("button: ", i);
      buttonOperation(i);
      buttonDelayTimer[i] = TIMER_STOP;
    }
    else if (!digitalRead(BTN[i]) /*button pressed*/) {
        buttonDelayTimer[i] = millis() + BUTTON_DELAY;
    }
  }
}


