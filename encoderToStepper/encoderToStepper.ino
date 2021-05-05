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
unsigned long delayTime;

bool aPinState;
bool bPinState;
const int8_t directionFlip = 1;
int stepsToGo = 0;
int stepsPerNotch = 1;
long timeOfLastStop;
long timeSinceLastStop = 1000;
unsigned long nextStepMicrosecondsTime = 0;
unsigned long nowMicroSeconds;
const long minStepTime = 300;
const long maxStepTime = 12000;

const long stepsToGoRampStart = 15;
const long stepsToGoRampEnd = 800;

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
    if (timeSinceLastStop < 300){
      stepsPerNotch = 15;
    }
    else if (timeSinceLastStop < 500){
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
    long absSteps =  constrain(abs(stepsToGo), stepsToGoRampStart, stepsToGoRampEnd);
    delayTime = map(absSteps, stepsToGoRampStart, stepsToGoRampEnd, maxStepTime, minStepTime);
  }
  nowMicroSeconds = micros();
  if (stepsToGo != 0 && nowMicroSeconds > nextStepMicrosecondsTime){
    digitalWrite(pulsePin, LOW);
    delayMicroseconds(150);
    digitalWrite(pulsePin, HIGH);  
    nextStepMicrosecondsTime = nowMicroSeconds + delayTime;
    stepsToGo += stepsToGo < 0 ? 1 : -1; 
  }
}
