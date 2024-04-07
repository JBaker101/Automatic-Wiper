//variables that will change
int sensorValue = 0;
int digitalState = 0;
int buffer = 2;
bool rain = false;

//constants
const int lowLedState = LOW;
const int highLedState = HIGH;
const int relayPin = 12;
const int inputPin = 8;
const int ledPin1 = 2;
const int ledPin2 = 3;
const int ledPin3 = 4;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //   cli();
  // /*1. First we reset the control register to make sure we start with everything disabled.*/
  //   TCCR1A = 0;                 // Reset entire TCCR1A to 0
  //   TCCR1B = 0;                 // Reset entire TCCR1B to 0

  //   /*2. We set the prescalar to the desired value by changing the CS10 CS12 and CS12 bits. */
  //   TCCR1B |= B00000100;        //Set CS12 to 1 so we get prescalar 256

  //   /*3. We enable compare match mode on register A*/
  //   TIMSK1 |= B00000010;        //Set OCIE1A to 1 so we enable compare match A

  //   /*4. Set the value of register A to 31250*/
  //   OCR1A = 65535;             //Finally we set compare register A to this value
  //   sei();                     //Enable back the interrupts
  //setup LED status lights
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(inputPin, INPUT);

  //set intial LED state
  digitalWrite(ledPin1, lowLedState);
  digitalWrite(ledPin2, lowLedState);
  digitalWrite(ledPin3, lowLedState);

  //setup relay pin
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(A0);
  digitalState = digitalRead(inputPin);

  if (Serial.available() > 0) {
    int value = Serial.read();

    if (value == '1') {
      digitalWrite(relayPin, HIGH);
      buffer = 0;
    } else if (buffer >= 2) {
      digitalWrite(relayPin, LOW);
    }

    if (value == '3') {
      Serial.println(sensorValue);
    }
  }

  //No rain
  if (sensorValue > 550 || digitalState == 0) {
    digitalWrite(ledPin1, lowLedState);
    digitalWrite(ledPin2, lowLedState);
    digitalWrite(ledPin3, lowLedState);
    //Serial.println("No_Rain");
    //Wait 1 seconds
    delay(1000);
  }

  if (digitalState == 1) {
    //Light rain
    if (sensorValue <= 550 && sensorValue > 350) {
      digitalWrite(ledPin1, highLedState);
      digitalWrite(ledPin2, lowLedState);
      digitalWrite(ledPin3, lowLedState);
      Serial.println(sensorValue);
      rain = true;
      //Wait 2 seconds
      delay(2000);
    }
    //Medium rain
    if (sensorValue <= 350 && sensorValue > 275) {
      digitalWrite(ledPin1, lowLedState);
      digitalWrite(ledPin2, highLedState);
      digitalWrite(ledPin3, lowLedState);
      Serial.println(sensorValue);
      rain = true;
      //Wait 2 seconds
      delay(2000);
    }
    //Heavy Rain
    if (sensorValue <= 275) {
      digitalWrite(ledPin1, lowLedState);
      digitalWrite(ledPin2, lowLedState);
      digitalWrite(ledPin3, highLedState);
      Serial.println(sensorValue);
      rain = true;
      //Wait 2 seconds
      delay(2000);
    }
  }
  //digitalWrite(relayPin, LOW);
  buffer++;
}

//With the settings above, this IRS will trigger each 1000ms.
// ISR(TIMER1_COMPA_vect){
//   TCNT1  = 0;                  //First, set the timer back to 0 so it resets for next interrupt
//   Serial.println(sensorValue);
// }
