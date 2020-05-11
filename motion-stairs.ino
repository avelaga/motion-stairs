#include <FastLED.h>

#define NUM_LEDS 150
#define PIN 7
#define LEN 8

CRGB leds[NUM_LEDS];
int brightness[NUM_LEDS];
int hue[NUM_LEDS];
boolean on = false;
int inc = 0;
int hueInc = 0;

int ledPin = 13;                // choose the pin for the LED
int inputPin = 3;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;
int timeLeft = 0;

void setup() {
  LEDS.addLeds<WS2812, PIN, RGB>(leds, NUM_LEDS);
  LEDS.setBrightness(255);
  pinMode(inputPin, INPUT);     // declare sensor as input
  Serial.begin(9600);
}

void readMotion() {
  val = digitalRead(inputPin);  // read input value
  if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // We only want to print on the output change, not state
      pirState = HIGH;
      timeLeft = 500;
    }
  } else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH) {
      // we have just turned of
      Serial.println("Motion ended!");
      // We only want to print on the output change, not state
      pirState = LOW;
//      timeLeft = 0;
    }
  }
}

void loop() {
  readMotion();
  //  if (pirState == HIGH) {
  if (timeLeft > 0) {
    timeLeft--;
    if (inc > LEN) {
      on = !on;
      inc = 0;
      hueInc = 97;
    } else {
      inc++;
      hueInc += 3;
    }

    for (int a = NUM_LEDS - 1; a > 0; a--) {
      hue[a] = hue[a - 1];

      if (brightness[a - 1] == 255) {
        brightness[a] = 255;
      }
      else {
        brightness[a] = 0;
      }

    }

    if (on) {
      brightness[0] = 255;
      hue[0] = hueInc;
    }
    else {
      brightness[0] = 0;
    }

    // fill leds with determined pattern
    for (int x = 0; x < NUM_LEDS; x++) {
      leds[x] = CHSV(hue[x], 255, brightness[x]);
    }

    delay(15);
    FastLED.show();

  }
  if(timeLeft==0) {
    for (int x = 0; x < NUM_LEDS; x++) {
      leds[x] = CHSV(0, 0, 0);
      for (int a = NUM_LEDS - 1; a > 0; a--) {
        hue[a] = 0;
        brightness[a] = 0;
      }
      FastLED.show();
    }
  }
}
