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
const int stepsPerTurn = 10;
void setup() {
  pinMode (APin, INPUT);
  pinMode (BPin, INPUT);  
  pinMode (directionPin, OUTPUT);
  pinMode (pulsePin, OUTPUT);

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
  if (lastState == stateNoneTriggered && thisState == stateATriggered){
    stepsToGo += stepsPerTurn * directionFlip;
  }
  if (lastState == stateNoneTriggered && thisState == stateBTriggered){
    stepsToGo -= stepsPerTurn * directionFlip;
  }
  if (stepsToGo < 0){
    digitalWrite(directionPin, LOW);
  }
  else {
    digitalWrite(directionPin, HIGH);
  }
  uint8_t absSteps = abs(stepsToGo);
  if (absSteps > 0){
    if (absSteps > 100){
      delayTime = 150;
    }
    else if (absSteps > 50){
      delayTime = 225;
    }
    else {
      delayTime = 300;
    }
    digitalWrite(pulsePin, LOW);
    delayMicroseconds(delayTime);
    digitalWrite(pulsePin, HIGH);  
    delayMicroseconds(delayTime);
    stepsToGo += stepsToGo < 0 ? 1 : -1; 
  }


}
