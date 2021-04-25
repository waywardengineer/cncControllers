const uint8_t APin = 2;
const uint8_t BPin = 0;
//const uint8_t LEDPin = 1;
const uint8_t directionPin = 4;
const uint8_t pulsePin = 1;

const uint8_t stateNoneTriggered = 0;
const uint8_t stateATriggered = 1;
const uint8_t stateBTriggered = 2;
const uint8_t stateBothTriggered = 3;

uint8_t thisState = 0;
uint8_t lastState;

bool aPinState;
bool bPinState;
const int8_t directionFlip = 1;
int stepsToGo = 0;
const int stepsPerTurn = 10;
void setup() {
  pinMode (APin, INPUT);
  pinMode (BPin, INPUT);  
  //pinMode (LEDPin, OUTPUT);
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
  if (abs(stepsToGo) > 0){
    digitalWrite(pulsePin, LOW);
    delayMicroseconds(100);
    digitalWrite(pulsePin, HIGH);  
    delayMicroseconds(300);
    stepsToGo += stepsToGo < 0 ? 1 : -1; 
    //digitalWrite(LEDPin, HIGH);
  }
  //else{
    //digitalWrite(LEDPin, LOW);  
  //}*/

}
