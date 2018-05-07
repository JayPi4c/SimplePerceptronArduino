// Dies ist ein Beispielprogramm eines Perceptrons für den Arduino.
// Es wurde auf der Basis der Coding Challenge "Simple Perceptron" von dem Coding Train aka Daniel Shiffman entworfen.
// Version vom 06.05.18 von JayPi4c


#include <LiquidCrystal_I2C.h>

// Einbinden der Perceptron und Point Klassen
#include "Perceptron.h"
#include "Point.h"
#include "Vars.h"


// Inititalisierung des LCD für visuellen output
LiquidCrystal_I2C lcd(0x20, 20, 4);

// Das Perceptron, welches lernt
Perceptron brain;

// einige globale Variablen
int bestSoFar = 0;
int cycle = 0;


void setup() {

  // sorge für richtige Randomness bei jedem Start
  randomSeed(analogRead(A0));

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




  // Setze die Texte auf dem LCD
  lcd.clear();
  lcd.print("Performance:    %");
  lcd.setCursor(0, 1);
  lcd.print("Best so far:    %");
  lcd.setCursor(0, 3);
  lcd.print("target:" + (String)m + "x" + (b > 0 ? "+" : "") + (String)b + "=y");
}

void loop() {
  digitalWrite(13, HIGH);
  // test performance:
  // Es werden pro Durchlauf "total" mal gemessen, was das Perceptron schätzt (guess)
  // Anhand der Anzahl der richtig geschätzten Punkte wird die Performance ermittelt.
  int correctGuess = 0;
  for (int i = 0; i < total; i++) {
    Point pt;
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
  String msg = "guess: " + (String)m_ + "x" + ( b_ > 0 ? "+" : "") + (String)b_ + "=y";
  lcd.print(fillEmpty(msg));


  digitalWrite(13, LOW);
  // training:
  // Trainiere pro Durchlauf einen neuen Punkt und verbessere so die Schätzung
  //for (int i = 0; i < 10000; i++) {
  Point pt;
  float inputs[] = {pt.x, pt.y, pt.bias};
  int target = pt.label;
  brain.train(inputs, target);
  //}
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
    lcd.print("target: " + (String)m + "x"+( b_ > 0 ? "+" : "") + (String)b + "=y");
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


