
// TEST FOR 1 OR 2 STEPPERS AT THE SAME SPEED 

//#define stepPin 10
//float stepDelay = 1.5;

//void setup() {
//  Serial.begin(9600);
//  pinMode(stepPin, OUTPUT);
//  pinMode(2,OUTPUT);
//  digitalWrite(2,LOW);
//}

//void loop() {
//  int i=0;
//  for(i=0; stepDelay<5; i++) {
//    Serial.println(stepDelay);
//    int k=0;
//     while(k<400) {
//      digitalWrite(stepPin, HIGH);
//      delay(stepDelay);
//      digitalWrite(stepPin, LOW);
//      k++;
//     }
//     stepDelay +=.1;
//    }
//    
//  i=0;
//}

// TEST FOR STEPPERS RUNNING AT 2 DIFFERENT SPEEDS

#define stepperPin1_3 6
#define stepperPin2_4 4
#define stepDelay1_3 1500 //micro seconds
#define stepDelay2_4 5000 //micro seconds

void setup() {
  Serial.begin(115200);
  pinMode(stepperPin1_3, OUTPUT);
  pinMode(stepperPin2_4, OUTPUT);
  pinMode(2,OUTPUT);
  digitalWrite(2,LOW);
}

void loop() {
  
  unsigned long timerS1_3=micros();
  unsigned long timerS2_4=micros();
  
//  digitalWrite(stepperPin1_3,HIGH);
//  digitalWrite(stepperPin2_4, HIGH);
//  delay(1.2);
//  digitalWrite(stepperPin1_3,LOW);
//  digitalWrite(stepperPin2_4,LOW);
  
  while(1) {
  
    if((micros()-timerS1_3)>=stepDelay1_3) {
      digitalWrite(stepperPin1_3, LOW);
      timerS1_3=micros(); // reset timer
      S1_3High();
      //Serial.println("Stepper 1_3");
    }
    if((micros()-timerS2_4)>=stepDelay2_4) {
      digitalWrite(stepperPin2_4, LOW);
      timerS2_4=micros(); // reset timer
      S2_4High();
      //Serial.println("Stepper 2_4");
    }
  }  
}

void S1_3High() {
  digitalWrite(stepperPin1_3,HIGH);
}

void S2_4High() {
  digitalWrite(stepperPin2_4,HIGH);
}
