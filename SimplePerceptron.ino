// Dies ist ein Beispielprogramm eines Perceptrons für den Arduino.
// Es wurde auf der Basis der Coding Challenge "Simple Perceptron" von dem Coding Train aka Daniel Shiffman entworfen.
// Version vom 06.05.18 von JayPi4c


#include <LiquidCrystal_I2C.h>

// Einbinden der Perceptron und Point Klassen
#include "Perceptron.h"
#include "Point.h"
#include "Vars.h"


// Inititalisierung des LCD für visuellen output
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Das Perceptron, welches lernt
Perceptron brain;

// einige globale Variablen
int bestSoFar = 0;
int cycle = 0;

// globale Variablen für den Encoder
int count = 0;//Define the count
int lastCLK = 0;//CLK initial value

struct Values {
  float m;
  float b;
};

Values vals;


void setup() {
  Serial.begin(9600);
  // sorge für richtige Randomness bei jedem Start
  randomSeed(analogRead(A0));

  pinMode(SW, INPUT);
  digitalWrite(SW, HIGH);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  attachInterrupt(digitalPinToInterrupt(CLK), ClockChanged, CHANGE);

  //Initialisiere den LCD und gebe einen Boot output aus
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print("SimplePerceptron");
  lcd.setCursor(6, 2);
  lcd.print("JayPi4c");
  lcd.setCursor(13, 3);
  lcd.print("v.1.0.0");

  // erst nach 2.5 sekunden beginnt der richtige Sketch
  delay(2500);

  vals =  getValues(lcd);


  // Setze die Texte auf dem LCD
  lcd.clear();
  lcd.print("Performance:    %");
  lcd.setCursor(0, 1);
  lcd.print("Best so far:    %");
  lcd.setCursor(0, 3);
  lcd.print("target:" + (String)vals.m + "x" + (vals.b >= 0 ? "+" : "") + (String)vals.b + "=y");
}

void loop() {
  digitalWrite(13, HIGH);
  // test performance:
  // Es werden pro Durchlauf "total" mal gemessen, was das Perceptron schätzt (guess)
  // Anhand der Anzahl der richtig geschätzten Punkte wird die Performance ermittelt.
  int correctGuess = 0;
  for (int i = 0; i < total; i++) {
    Point pt(vals.m, vals.b);
    float inputs[] = {pt.x, pt.y, pt.bias};
    int target = pt.label;
    int guess_ = brain.guess(inputs);
    if (guess_ == target)
      correctGuess++;
  }
  float performance = (float)correctGuess / (float)total;
  performance *= 100;


  // Asugabe der Prerformance auf dem Display
  if (performance > bestSoFar) {
    bestSoFar = performance;
    lcd.setCursor(13, 1);
    int n = (int) (bestSoFar);
    String str = (String)n;
    lcd.print(getString(str));
  }

  lcd.setCursor(13, 0);
  int n = (int) (performance);
  String str = (String)n;
  lcd.print(getString(str));


  // Formula is weights[0]*x + weights[1]*y + weights[2] = 0
  float* weights = brain.getWeights();

  float m_ = (-weights[0]) / weights[1];
  float b_ = (-weights[2]) / weights[1];
  //Serial.println(weights[2]);

  lcd.setCursor(0, 2);
  String msg = "guess: " + (String)m_ + "x" + ( b_ >= 0 ? "+" : "") + (String)b_ + "=y";
  lcd.print(fillEmpty(msg));


  digitalWrite(13, LOW);
  // training:
  // Trainiere pro Durchlauf einen neuen Punkt und verbessere so die Schätzung
  for (int i = 0; i < total; i++) {
    Point pt(vals.m, vals.b);
    float inputs[] = {pt.x, pt.y, pt.bias};
    int target = pt.label;
    brain.train(inputs, target);
  }
  // nach einer Zehntel Sekunde beginnt die Loop wieder von vorne.
  // (Damit man auch lesen kann, was auf den Display geschrieben wird)
  //delay(100);

  cycle++;
  if (cycle > 500) {
    lcd.clear();
    lcd.print("Performance:    %");
    lcd.setCursor(0, 1);
    lcd.print("Best so far:    %");
    lcd.setCursor(0, 3);
    lcd.print("target: " + (String)vals.m + "x" + ( vals.b >= 0 ? "+" : "") + (String)vals.b + "=y");
    lcd.setCursor(13, 1);
    int n = (int) (bestSoFar);
    String str = (String)n;
    lcd.print(getString(str));
    cycle = 0;
  }
}


String getString(String msg) {
  int len = msg.length();
  String msgOld = msg;
  if (len < 3) {
    for (int i = 0; i < 3 - len; i++)
      msg = " ";
    msg += msgOld;
  }
  return msg;
}

String fillEmpty(String msg) {
  int len = msg.length();
  for (int i = 0; i < 20 - len; i++) {
    msg += " ";
  }
  return msg;
}


Values getValues(LiquidCrystal_I2C lcd) {
  Values v;

  lcd.clear();
  lcd.print("  m   *x+  b  =y");
  lcd.setCursor(0, 1);
  char mS[] = "-88.88";
  char bS[] = "-88.88";
  lcd.print((String)mS + "*x" + (String)bS + "=y");
  lcd.setCursor(0, 2);
  lcd.print("^");

  while (digitalRead(SW)) {
    lcd.setCursor(0, 1);
    char msg = (count % 2 == 0 ? '+' : '-');
    //Serial.println(msg);
    lcd.print(msg);
    int i = 0;
    mS[i] = msg;
  }
  lcd.setCursor(0, 2);
  lcd.print(" ");

  delay(200);
  for (int i = 1; i < (sizeof(mS) / sizeof(char)) - 1; i++) {
    if (i == 3)
      continue;

    lcd.setCursor(i, 2);
    lcd.print("^");
    while (digitalRead(SW)) {
      lcd.setCursor(i, 1);
      int num = abs(count);
      String c = (String)(num);
      mS[i] = c[0];
      //      Serial.print("m[" + (String)i + "]");
      //      Serial.print(mS[i]);
      //      Serial.print("|| c: ");
      //      Serial.println(c);
      lcd.print(abs(count));
    }

    delay(200);
    lcd.setCursor(i, 2);
    lcd.print(" ");
  }
  //  Serial.print("Jetzt kommt m: ");
  //  Serial.print(mS);

  String s = (String)mS;
  float m = atof(mS);

  v.m = m;


  lcd.setCursor(8, 2);
  lcd.print("^");
  while (digitalRead(SW)) {
    lcd.setCursor(8, 1);
    char msg = (count % 2 == 0 ? '+' : '-');
    //Serial.println(msg);
    lcd.print(msg);

    bS[0] = msg;
  }
  lcd.setCursor(8, 2);
  lcd.print(" ");

  delay(200);
  for (int i = 9; i < 8 + (sizeof(bS) / sizeof(char)) - 1; i++) {
    if (i == 11)
      continue;

    lcd.setCursor(i, 2);
    lcd.print("^");
    while (digitalRead(SW)) {
      lcd.setCursor(i, 1);
      int num = abs(count);
      String c = (String)(num);
      bS[i - 8] = c[0];
      //      Serial.print("b[" + (String)i + "]");
      //      Serial.print(bS[i - 8]);
      //      Serial.print("|| c: ");
      //      Serial.println(c);
      lcd.print(abs(count));
    }

    delay(200);
    lcd.setCursor(i, 2);
    lcd.print(" ");
  }

  s = (String)bS;
  float b = atof(bS);
  v.b = b;
  return v;

}



void ClockChanged()
{
  int clkValue = digitalRead(CLK);//Read the CLK pin level
  int dtValue = digitalRead(DT);//Read the DT pin level
  if (lastCLK != clkValue)
  {
    lastCLK = clkValue;
    count += (clkValue != dtValue ? 1 : -1);//CLK and inconsistent DT + 1, otherwise - 1
    count %= 10;
    Serial.print("count:");
    Serial.println(count);
  }
}

