
#include <LedControl.h>

LedControl lc = LedControl(12, 11, 10, 1);

const int up = 2;
const int down = 3;
const int left = 4;
const int right = 5;

const int max_len = 64;
int snakeX[max_len];
int snakeY[max_len];

int length = 3; // initial

int x = 1, y = 0;  // Default direction: RIGHT

int foodX, foodY;
bool eat;

// Timing for movement
unsigned long lastMoveTime = 0;
unsigned long moveInterval = 300; // ms

int prev_len = 3;


// y = row , x = column

void setup() {

  Serial.begin(9600);
  Serial.println("Game Start");
  Serial.println("Score:- 0");

  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);

  pinMode(up, INPUT_PULLUP);
  pinMode(down, INPUT_PULLUP);
  pinMode(left, INPUT_PULLUP);
  pinMode(right, INPUT_PULLUP);

  // Initial snake body
  snakeX[0] = 2;  snakeY[0] = 0;
  snakeX[1] = 1;  snakeY[1] = 0;
  snakeX[2] = 0;  snakeY[2] = 0;

  foodgen();
  Rungame();
}

void loop() {
  readIn();  // Always check input

  

  unsigned long currentTime = millis();

  moveInterval = min(700, 300 + (length - 3)*10);

  if (currentTime - lastMoveTime >= moveInterval) {
    lastMoveTime = currentTime;

    // Check if snake will eat the food
    eat = (snakeX[0] + x + 8) % 8 == foodX && (snakeY[0] + y + 8) % 8 == foodY;

    if(movesnake()) // true = collision
    {
      gameover();
    }

    if (eat && length < max_len) {
      length++;
    }


  if(length != prev_len){
    Serial.print("Score:- ");
    Serial.println(length - 3);
    prev_len = length;
  }

    if (eat) {
      foodgen();
    }


    Rungame();

  }

  
}

void gameover()
{
  for(int i = 0;i<3;i++)
  {
    lc.clearDisplay(0);
    delay(300);
    Rungame();
    delay(300);

  }

  resetgame();
}


void resetgame()
{
  length = 3;
  x = 1;
  y = 0;

 

  snakeX[0] = 2;  snakeY[0] = 0;
  snakeX[1] = 1;  snakeY[1] = 0;
  snakeX[2] = 0;  snakeY[2] = 0;

  foodgen();
  Rungame();
}

void foodgen() {
  bool flag;

  do {
    flag = false;

    foodX = random(0, 8);
    foodY = random(0, 8);

    for (int i = 0; i < length; i++) {
      if (snakeX[i] == foodX && snakeY[i] == foodY) {
        flag = true;
        break;
      }
    }
  } while (flag);
}

void Rungame() {
  lc.clearDisplay(0);

  for (int i = 0; i < length-1; i++) {
    lc.setLed(0, snakeY[i], snakeX[i], true);
  }


  lc.setLed(0, foodY, foodX, true);

  delay(100);
  bool tail = false;
  tail = !tail;
  if(tail)
  lc.setLed(0,snakeY[length-1],snakeX[length-1],true);

  
}

// up = -1 , down = 1, left = -1, right = 1
void readIn() {
  if (digitalRead(up) == LOW && y != 1) {
    y = -1; x = 0;
  }
  if (digitalRead(down) == LOW && y != -1) {
    y = 1; x = 0;
  }
  if (digitalRead(left) == LOW && x != 1) {
    x = -1; y = 0;
  }
  if (digitalRead(right) == LOW && x != -1) {
    x = 1; y = 0;
  }
}

bool movesnake() {
  for (int i = length - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  snakeX[0] = (snakeX[0] + x + 8) % 8;
  snakeY[0] = (snakeY[0] + y + 8) % 8;

  for(int i = 1;i<length;i++)
  {
    if(snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i])
    return true;

  }

  return false;
}


