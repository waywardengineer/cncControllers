const uint8_t APin = 2;
const uint8_t BPin = 0;
const uint8_t directionPin = 4;
const uint8_t pulsePin = 1;

const uint8_t stateNoneTriggered = 0;
const uint8_t stateATriggered = 1;
const uint8_t stateBTriggered = 2;
const uint8_t stateBothTriggered = 3;

uint8_t thisState = 0;
uint8_t lastState;
uint8_t delayTime;

bool aPinState;
bool bPinState;
const int8_t directionFlip = 1;
int stepsToGo = 0;
int stepsPerNotch = 1;
long timeOfLastStop;
long timeSinceLastStop = 1000;
long nextStepMicrosecondsTime = 0;

const int minStepTime = 150;
const int maxStepTime = 500;

const int stepsToGoRampStart = 10;
const int stepsToGoRampEnd = 800;

void setup() {
  pinMode (APin, INPUT);
  pinMode (BPin, INPUT);  
  pinMode (directionPin, OUTPUT);
  pinMode (pulsePin, OUTPUT);
  timeOfLastStop = millis();
  

}

void loop() {
  aPinState = digitalRead(APin);
  bPinState = digitalRead(BPin);
  lastState = thisState;
  if (aPinState && bPinState){
    thisState = stateBothTriggered;
  }
  else if (aPinState){
    thisState = stateATriggered;
  }
  else if (bPinState){
    thisState = stateBTriggered;
  }
  else{
    thisState = stateNoneTriggered;
  }

  if (lastState != stateNoneTriggered && thisState == stateNoneTriggered){
    long timeOfThisStop = millis();
    timeSinceLastStop = timeOfThisStop - timeOfLastStop;
    timeOfLastStop = timeOfThisStop;
    if (timeSinceLastStop < 100){
      stepsPerNotch = 15;
    }
    else if (timeSinceLastStop > 200){
      stepsPerNotch = 7;
    }
    else {
      stepsPerNotch = 1;
    }
  }
  
  if (lastState == stateNoneTriggered && thisState != stateNoneTriggered){
    if (thisState == stateATriggered){
      stepsToGo += stepsPerNotch * directionFlip;
    }
    if (thisState == stateBTriggered){
      stepsToGo -= stepsPerNotch * directionFlip;
    }
    if (stepsToGo < 0){
      digitalWrite(directionPin, LOW);
    }
    else {
      digitalWrite(directionPin, HIGH);
    }
    int absSteps = abs(stepsToGo);
    delayTime = map(constrain(absSteps, stepsToGoRampStart, stepsToGoRampEnd), stepsToGoRampStart, stepsToGoRampEnd, maxStepTime, minStepTime);
  }
  long nowMicroSeconds = micros();
  if (stepsToGo != 0 && nowMicroSeconds > nextStepMicrosecondsTime){
    digitalWrite(pulsePin, LOW);
    delayMicroseconds(100);
    digitalWrite(pulsePin, HIGH);  
    nextStepMicrosecondsTime = nowMicroSeconds + delayTime;
    stepsToGo += stepsToGo < 0 ? 1 : -1; 
  }
}
