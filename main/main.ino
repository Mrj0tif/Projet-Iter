#include <Arduino.h>
#include <AccelStepper.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_TCS34725.h>

#define X_STEP_PIN    25
#define X_DIR_PIN     23
#define X_ENABLE_PIN  27
#define X_STOP_PIN    22
#define Y_STEP_PIN    37
#define Y_DIR_PIN     39
#define Y_ENABLE_PIN  35
#define Y_STOP_PIN    26

AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);
Adafruit_TCS34725 capteur;

enum Couleur {
  VERT, BLEU, JAUNE, REPOS, VIOLET, ORANGE,
  CINQ, QUATRE, TROIS, DEUX, UN
};

int nb_pas = 4096;

struct Position {
  int x;
  int y;
};

long start_step_X;
long start_step_Y;
long step_unit_x = 0;
long step_unit_y = 0; 

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
  Serial.begin(9600);
  capteur.begin();
  lcd.init();
  lcd.backlight();
  pinMode(X_ENABLE_PIN, OUTPUT);
  pinMode(Y_ENABLE_PIN, OUTPUT);
  pinMode(X_STOP_PIN, INPUT);
  pinMode(Y_STOP_PIN, INPUT);
  digitalWrite(X_ENABLE_PIN, LOW);
  digitalWrite(Y_ENABLE_PIN, LOW);
  stepperX.setMaxSpeed(2000);
  stepperX.setAcceleration(1000);
  stepperY.setMaxSpeed(2000);
  stepperY.setAcceleration(1000);
  
  start_step_X = stepperX.currentPosition();
  start_step_Y = stepperY.currentPosition();
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


void calibrage_x(){

  while (digitalRead(X_STOP_PIN) == 0)
  { 
    stepperX.move(10);
    stepperX.runToPosition();
  }
  
  long step_fais_x = stepperX.currentPosition() - start_step_X;
  Serial.print("step en X: ");
  Serial.println(step_fais_x);
  
  long step_unit_x = step_fais_x / 6;
  Serial.print("step/unit pr X: ");
  Serial.println(step_unit_x);
}

void calibrage_y(){

  while (digitalRead(Y_STOP_PIN) == 0)
  { 
    stepperY.move(10);
    stepperY.runToPosition();
  }
  
  long step_fais_y = stepperY.currentPosition() - start_step_Y;
  Serial.print("step en Y: ");
  Serial.println(step_fais_y);
  
  long step_unit_y = step_fais_y / 3;
  Serial.print("step/unit pr Y: ");
  Serial.println(step_unit_y);
}


void stepper(int x, int y){
  stepperX.moveTo(step_unit_x * x);
  stepperY.moveTo(step_unit_y * y);
}


String detecterCouleur() {
  uint16_t red, green, blue, light;
  capteur.getRawData(&red, &green, &blue, &light);
  
  if (red < 50 && green < 50 && blue < 50) {
    return "NOIR";
  }
  else if (red > 300 && green > 300 && blue > 300) {
    return "BLANC";
  }
  else if (red > green && red > blue) {
    return "ROUGE";
  }
  else if (green > red && green > blue) {
    return "VERT";
  }
  else if (blue > red && blue > green) {
    return "BLEU";
  }
  else if (red > 200 && green > 200 && blue < 150) {
    return "JAUNE";
  }
  else if (red > 200 && blue > 200 && green < 150) {
    return "VIOLET";
  }
  
  return "INCONNUE";
}


void automatique(){
  // Besoin de convertir String → enum Couleur
  String couleurDetectee = detecterCouleur();
  
  
  Couleur c = REPOS;  
  if (couleurDetectee == "VERT") c = VERT;
  else if (couleurDetectee == "BLEU") c = BLEU;
  else if (couleurDetectee == "JAUNE") c = JAUNE;
  else if (couleurDetectee == "VIOLET") c = VIOLET;
  else if (couleurDetectee == "ORANGE") c = ORANGE;
  
  Position pos = getPosition(c);
  stepper(pos.x, pos.y);  // Déplacer aux coordonnées
}
