#include <Stepper.h>
#include <stdbool.h>

const int buttonPin1 = 2;
const int buttonPin2 = 53;
const int stepsPerRevol = 64;
bool sSet = false;
bool motorSet = false;
int speedArr[] = {30, 100, 200, 330};
int speedIndex = 0;


int currVelocity = 0;
int motorSpeed = 0;


// button states for input1
int buttonState = 0;
int prevButtonState = 0;

// button states for input2
int buttonState2 = 0;
int prevButtonState2 = 0;



Stepper myStepper(stepsPerRevol, 4, 5, 6, 7);


typedef enum fsaState {
  INIT,
  IN_RUN,
  IN_STOP,
} state;

state currState;


void setup() {
  // put your setup code here, to run once:
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  Serial.begin(9600);
}









void initExecute() {

  if (sSet) {
    myStepper.setSpeed(currVelocity);
    myStepper.step(1);

  } else {
    Serial.print("starting slow\n");
    myStepper.setSpeed(speedArr[0]);
    myStepper.step(1);
    speedIndex = 0;

  }




  motorSet = true;
  Serial.print("HERE\n");


  buttonState = digitalRead(buttonPin1);

  if (buttonState != prevButtonState) {
    Serial.print("button was hit");
    currState = IN_RUN;

    if (sSet) {
      int currVelocityIndex = getCurrVelocity(currVelocity);
      if (  currVelocityIndex < 3 ) {
        myStepper.setSpeed(speedArr[currVelocityIndex + 1]);
        myStepper.step(1);
      } else {
        // nothing max speed reached
      }
    } else {

      speedIndex = 1;
      Serial.print("speed set to 1");
    }
  }
  prevButtonState = buttonState;

  Serial.print("checking the second button\n");
  buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 != prevButtonState2) {
    // if speed isnt set
    if (buttonState2 == HIGH) {
      // save cutr velocity and go to next state
      currVelocity = speedArr[speedIndex];
      currState = IN_STOP;

    }
  } else {

  }
  prevButtonState2 = buttonState2;








}

void stateInRun() {

  buttonState = digitalRead(buttonPin1);
  if (buttonState != prevButtonState) {
    // if speed isnt set
    if (buttonState == HIGH) {
      Serial.print("Button pressed in IN_RUN !!!!!!!!!!!!!!!!!!!!!!\n");
      inrunExecute(currVelocity);
    }
  } else {
    myStepper.setSpeed(speedArr[speedIndex]);
    myStepper.step(1);
  }
  prevButtonState = buttonState;


  buttonState2 = digitalRead(buttonPin2);
  if (buttonState2 != prevButtonState2) {
    // if speed isnt set
    if (buttonState2 == HIGH) {
      // save cutr velocity and go to next state
      currVelocity = speedArr[speedIndex];
      motorSet = false;
      sSet = true;
      currState = IN_STOP;

    }
  } else {

  }
  prevButtonState2 = buttonState2;
}


void stateInStop() {


  Serial.print("you are now in STOP");

  if (motorSet == false) {

    buttonState2 = digitalRead(buttonPin2);
    if (buttonState2 != prevButtonState2) {
      // if speed isnt set
      if (buttonState2 == HIGH) {
        // save cutr velocity and go to next state
        currVelocity = 0;
        sSet = false;
        currState = INIT;

      }
    } else {

    }
    prevButtonState2 = buttonState2;

  }


  buttonState = digitalRead(buttonPin1);
  if (buttonState != prevButtonState) {
    // if speed isnt set
    if (buttonState == HIGH) {
      // save cutr velocity and go to next state
      currState = INIT;

    }
  } else {

  }
  prevButtonState = buttonState;
}



void inrunExecute(int cVelocity) {

  if (sSet == false) {
    if (speedIndex >= 3) {
      //nothing maxspeed reached
      Serial.print("MAX SPEED REACHED");
    } else {

      myStepper.setSpeed(speedArr[speedIndex]);
      myStepper.step(1);
      speedIndex++;
      Serial.print("\n");
      Serial.print(speedIndex);
    }
  } else {
    int currVelocityIndex = getCurrVelocity(cVelocity);
    if (  currVelocityIndex < 3 ) {
      myStepper.setSpeed(speedArr[currVelocityIndex + 1]);
      myStepper.step(1);
    } else {
      // nothing max speed reached
    }

  }
}




// return the index of the spped
int getCurrVelocity(int cVelocity) {
  int j;
  for (j = 0; j <= 3; j++) {
    if (cVelocity == speedArr[j]) {
      return j;
    }
  }
}







void loop() {

 

  currState = INIT;
  while (1) {

    switch (currState) {

      case INIT: Serial.print("in init\n");
        initExecute();
        break;

      case IN_RUN: //Serial.print("in IN_RUN\n");
        stateInRun();
        break;
      case IN_STOP: Serial.print("in IN_STOP\n");
        stateInStop();
        break;

    }
  }
}
