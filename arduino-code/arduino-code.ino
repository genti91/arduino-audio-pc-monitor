#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <FastLED.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#define LED_PIN 2
#define NUM_LEDS 18
#define BRIGHTNESS 10
#define LED_TYPE WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

int red = 50;
int green = 240;
int blue = 120;

#define BUTTON_PIN 3
bool buttonState = 0;
const int ledPin = 4;

const int NUM_SLIDERS = 4;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A3};

int analogSliderValues[NUM_SLIDERS];

void setup() {
  pinMode(ledPin, OUTPUT);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("CPU:"); 
  lcd.setCursor (0,1);
  lcd.print("GPU:");
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }

  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);

  pinMode(BUTTON_PIN,INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  digitalWrite(4, LOW);
  int buttonReading = digitalRead(BUTTON_PIN); // read the state of the button
  if (buttonReading == LOW) { // check if the button is pressed
    delay(50); // wait for 50 milliseconds to debounce the button
    buttonState++; // increment the buttonState variable
    if (buttonState == 2) { // check if the button has been pressed twice
      digitalWrite(ledPin, HIGH); // turn on the LED
    }
    else if (buttonState == 1) { // check if the button has been pressed once
      digitalWrite(ledPin, LOW); // turn off the LED
      //buttonState = 0; // reset the buttonState variable
    }
    //digitalWrite(4, HIGH);
    //buttonState = !buttonState; // invert the state of the buttonState variable
    //delay(500); // wait for 50 milliseconds to debounce the button
  }

  for (int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(red, green, blue); // Set all LEDs to red
  }
  FastLED.show();

//   String content = "";
//   char character;


//   while(Serial.available()) {
//       character = Serial.read();
//       content.concat(character);
//   }

//   if (content != "") {

//     if (content == "`") {
//   content = "";
//   lcd.setCursor(0,1);
// }

// if (content == "*") {
//   content = "";
//   lcd.setCursor(0,0);
// }

//     Serial.println(content);
//     lcd.print(content);
//   }

//     if (content == "~") {
//     lcd.clear();
//   }



  // String content = "";
  // char character;


  // while(Serial.available()) {
  //     character = Serial.read();
  //     content.concat(character);
  // }

  // if (content != "") {


  //   lcd.setCursor(5,0);

  //   Serial.println(content);
  //   lcd.print(content);
  // }



  if(Serial.available() > 0) {
    //lcd.clear();
    String s = Serial.readStringUntil('\n');
    
    //if (s.length() <= 16){
      //lcd.setCursor(0, 0);
      //lcd.clear();
      int porIndex = s.indexOf("%");
      int end = s.indexOf("|");
      lcd.setCursor(5, 0);
      lcd.print(s.substring(0,2) + char(223) + "C" + s.substring(2, porIndex + 1) + " ");
      lcd.setCursor(5, 1);
      lcd.print(s.substring(porIndex + 2,porIndex + 4) + char(223) + "C" + s.substring(porIndex + 4, end) + " ");
    //}
    // else {
    //   lcd.setCursor(0,0);
    //   lcd.print(s.substring(0,16));
    //   lcd.setCursor(0,1);
    //   lcd.print(s.substring(16, s.length()-1)); 
    // }  
  }
  updateSliderValues();
  if(!buttonState){
    sendSliderValues(); // Actually send data (all the time)
  }else{
    setRGBValues();
  }
  // printSliderValues(); // For debug
  //Serial.println(buttonState);
  delay(10);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}

void setRGBValues() {

  red = map((int)analogSliderValues[0],0,900,0,255);
  green = map((int)analogSliderValues[1],0,900,0,255);
  blue = map((int)analogSliderValues[2],0,900,0,255);
  FastLED.setBrightness(map((int)analogSliderValues[3],0,900,0,255));
  Serial.println("*" + String(red) + "|" + String(green) + "|" + String(blue));
}
