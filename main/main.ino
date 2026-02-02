#include <Arduino.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_TCS34725.h>

#define X_STEP_PIN    25
#define X_DIR_PIN     23
#define X_ENABLE_PIN  27

#define Z_STEP_PIN    37
#define Z_DIR_PIN     39
#define Z_ENABLE_PIN  35


AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperZ(AccelStepper::DRIVER, Z_STEP_PIN, Z_DIR_PIN);

LiquidCrystal_I2C lcd(0x27, 16, 2);

Adafruit_TCS34725 capteur;

enum Couleur {
  VERT, BLEU, JAUNE, REPOS, VIOLET, ORANGE,
  CINQ, QUATRE, TROIS, DEUX, UN
}

int nb_pas = 4096;

struct Position {
  int x;
  int z;
};

Position getPosition(Couleur c) {
  switch(c) {
    case VERT:   return {1, 1};
    case JAUNE:  return {3, 1};
    case VIOLET: return {5, 1};
    case BLEU:   return {1, 2};
    case REPOS:  return {3, 2};
    case ORANGE: return {5, 2};
    case CINQ:   return {1, 3};
    case QUATRE: return {3, 3};
    case TROIS:  return {3, 3};
    case DEUX:   return {4, 3};
    case UN:     return {5, 3};
    default:     return {0, 0};
  }
}

void setup(){
  capteur.begin();
  lcd.init();
  lcd.backlight();

  pinMode(X_ENABLE_PIN, OUTPUT);
  pinMode(Z_ENABLE_PIN, OUTPUT);
  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Z_ENABLE_PIN, LOW);

  stepperX.setMaxSpeed(2000);
  stepperX.setAcceleration(1000);

  stepperZ.setMaxSpeed(2000);
  stepperZ.setAcceleration(1000);

}

void loop(){


}


void oled(){
  lcd.setCursor(0, 0);
  lcd.print("Hello world !");
  lcd.clear();
}

void servo(){
  //control des servo moteur
}

void stepper(){
  stepperX.moveTo(nb_pas * 2);
  stepperZ.moveTo(nb_pas * 2);
  stepperX.run();
  stepperZ.run();
}


void color() {
  uint16_t red, green, blue, light;
  capteur.getRawData(&red, &green, &blue, &light);
  String couleur = "INCONNUE";

  if (r < 50 && g < 50 && b < 50) {
    couleur = "NOIR";
  }

  else if (r > 300 && g > 300 && b > 300) {
    couleur = "BLANC";
  }

  else if (r > g && r > b) {
    couleur = "ROUGE";
  }

  else if (g > r && g > b) {
    couleur = "VERT";
  }

  else if (b > r && b > g) {
    couleur = "BLEU";
  }
  // JAUNE = rouge + vert
  else if (r > 200 && g > 200 && b < 150) {
    couleur = "JAUNE";
  }
  // VIOLET = rouge + bleu
  else if (r > 200 && b > 200 && g < 150) {
    couleur = "VIOLET";
  }

  Serial.println(couleur);
  delay(1000);
}

