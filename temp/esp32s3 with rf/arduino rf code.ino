#define PIN1 4
#define PIN2 5
#define PIN3 6
#define PIN4 7
int globPin1Value = 0;
int globPin2Value = 0;
int globPin3Value = 0;
int globPin4Value = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PIN1, INPUT);
  pinMode(PIN2, INPUT);
  pinMode(PIN3, INPUT);
  pinMode(PIN4, INPUT);
  // pinMode(VT_pin, OUTPUT);
  // pinMode(indicator, OUTPUT);
  // resetReciver(1);
}

void RFmsgDecoder(int msgCode) {
  switch (msgCode) {
  case 1:
    // Serial.println("1");
    Serial.println("Motion detected on gyro sensor");
    break;
  case 2:
    // Serial.println("2");
    Serial.println("Motion detected on ultrasonic sensor");
    break;
  case 3:
    // Serial.println("3");
    Serial.println("Motion detected on both ultrasonic and gyro sensor");
    break;
  case 4:
    Serial.println("4");
    break;
  case 5:
    Serial.println("5");
    break;
  case 6:
    Serial.println("6");
    break;
  case 7:
    Serial.println("7");
    break;
  case 8:
    Serial.println("8");
    break;
  case 9:
    Serial.println("9");
    break;
  case 10:
    Serial.println("10");
    break;
  case 11:
    Serial.println("11");
    break;
  case 12:
    Serial.println("12");
    break;
  case 13:
    Serial.println("13");
    break;
  case 14:
    Serial.println("14");
    break;
  case 15:
    Serial.println("15");
    break;
  default:
    Serial.println("Msg out of range");
    break;
  }
}

void binaryToDecimal(int PIN_1, int PIN_2, int PIN_3, int PIN_4) {
  int decimal = 0;
  if (PIN_1 == 1) {
    decimal += 8;
  }
  if (PIN_2 == 1) {
    decimal += 4;
  }
  if (PIN_3 == 1) {
    decimal += 2;
  }
  if (PIN_4 == 1) {
    decimal += 1;
  }
  Serial.println(decimal);
  RFmsgDecoder(decimal);
}

void loop() {
  int Pin1ValueTemp = digitalRead(PIN1);
  int Pin2ValueTemp = digitalRead(PIN2);
  int Pin3ValueTemp = digitalRead(PIN3);
  int Pin4ValueTemp = digitalRead(PIN4);
  if (Pin1ValueTemp != globPin1Value || Pin2ValueTemp != globPin2Value ||
      Pin3ValueTemp != globPin3Value || Pin4ValueTemp != globPin4Value) {
    globPin1Value = Pin1ValueTemp;
    globPin2Value = Pin2ValueTemp;
    globPin3Value = Pin3ValueTemp;
    globPin4Value = Pin4ValueTemp;
    // digitalWrite(indicator, HIGH);
    // Serial.println("->" + String(globPin1ValueTemp) + "," +
    // String(globPin2ValueTemp) + "," + String(globPin3ValueTemp) + "," +
    // String(globPin4ValueTemp) + ".");
    if (globPin1Value == 1 || globPin2Value == 1 || globPin3Value == 1 ||
        globPin4Value == 1) {
      binaryToDecimal(globPin1Value, globPin2Value, globPin3Value,
                      globPin4Value);
    }

    // delay(500);
    // digitalWrite(indicator, LOW);
  }
  // delay(50);
}