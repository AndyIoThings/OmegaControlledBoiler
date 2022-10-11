char serialByte = '\0';
String serialData = "";
const int RelayPin = 12;
const uint32_t secsInDay = 240;
//Time and timers in seconds
//clock
uint32_t currentTime = 0;
//timers
uint32_t repeatTimer = 60;
uint32_t onTimer = 60;
//settings - set in the web interface 
uint32_t turnOnTime = 0;
uint32_t turnOffTime = 60;
uint32_t repeatTimerSet = 60;
uint32_t onTimerSet = 60;
bool repeat = false;
//Protocol
//commands
const String manualMode = "man";
const String autoMode = "aut";
//manual mode/auto mode
//manual mode - direct instruction to turn on or off
//----------------------------------------------------
//auto mode - options On with following turn on time in seconds
//option Off with following turn off time in seconds
//max 5 digits
//if it's needed to be repeated more often than once a day you can add repeat parameter
//with time in seconds afterwards
// example: aut On  12345 Off 12445 repeat 43200
const String turnOn = "On ";
const String turnOff = "Off";
//for semimanual mode these values follows turn on/off time
//meaning the program will continue repeating after (timeoff - timeon) period
const String repeatYes = "rep";


void setup() {
  Serial.begin(9600);
  pinMode(RelayPin, OUTPUT);
  digitalWrite(RelayPin, LOW);
}

void loop() {
  if (currentTime == secsInDay) {  //86400
    currentTime = 0;
  }
  // Serial.println("Timers");
  // Serial.println(repeatTimer);
  // Serial.println(OnTimer);
  // Serial.println(currentTime);
  if (digitalRead(RelayPin) == LOW && ((currentTime == turnOnTime) || (repeat && repeatTimer == 0))) {
    digitalWrite(RelayPin, HIGH);
    Serial.println(currentTime);
    Serial.println("High");
  } else if (digitalRead(RelayPin) == HIGH && currentTime == turnOffTime && !repeat) {
    digitalWrite(RelayPin, LOW);
    Serial.println(currentTime);
    Serial.println("Low");
  } else if (digitalRead(RelayPin) == HIGH && onTimer == 0 && repeat) {
    digitalWrite(RelayPin, LOW);
    onTimer = onTimerSet;
    repeatTimer = repeatTimerSet;
    Serial.println(currentTime);
    Serial.println("Low");
  }
  while (Serial.available() > 0) {
    serialByte = (char)Serial.read();
    if (serialByte == '\0' || serialByte == '\n' || serialByte == '\r')
      continue;
    //Serial.println(serialByte);
    serialData += serialByte;
  }
  if (serialData != "") {
    Serial.println(serialData);
    auto cmd = serialData.substring(0, 3);
    Serial.println(cmd);
    if (manualMode == cmd) {
      auto option = serialData.substring(4, 7);
      Serial.println(option);
      if (turnOn == option) {
        digitalWrite(RelayPin, HIGH);
        Serial.println(serialData);
      } else if (turnOff == option) {
        digitalWrite(RelayPin, LOW);
        Serial.println(serialData);
      }
    } else if (autoMode == cmd) {
      auto indexOn = serialData.indexOf(turnOn);
      if (indexOn != -1) {
        auto value = serialData.substring(indexOn + 5, indexOn + 4 + 5);
        Serial.println(value);
        turnOnTime = value.toInt();
      }
      auto indexOff = serialData.indexOf(turnOn);
      if (indexOff != -1) {
        auto value = serialData.substring(indexOff + 5, indexOff + 4 + 5);
        Serial.println(value);
        turnOffTime = value.toInt();
      }
      if (turnOnTime < turnOffTime) {
        onTimer = onTimerSet = turnOffTime - turnOnTime;
      } else if (turnOnTime > turnOffTime) {
        auto res = secsInDay - turnOnTime;
        onTimer = onTimerSet = res + turnOffTime;
      }
      auto indexRepeat = serialData.indexOf(repeatYes);
      if (indexRepeat != -1) {
        auto value = serialData.substring(indexOff + 5, indexOff + 5 + 5);
        Serial.println(value);
        repeatTimer = value.toInt();
      }
    }
    serialData = "";
  }
  delay(1000);
  currentTime++;
  if (repeat && digitalRead(RelayPin) == HIGH)
    onTimer--;
  if (repeat && digitalRead(RelayPin) == LOW)
    repeatTimer--;
}