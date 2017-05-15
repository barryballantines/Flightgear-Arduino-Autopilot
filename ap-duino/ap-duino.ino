
#include "AutoPilot.h"

#define SR_LATCH_PIN 8
#define SR_OUTPUT_ENABLE_PIN 9
#define SR_CLEAR_PIN 10
#define SR_CLOCK_PIN 12

#define HEADING_DISPLAY_SER 11
#define HEADING_CONTROLLER_A_PIN 3
#define HEADING_CONTROLLER_B_PIN 4


AutoPilot ap;

void setup() {
  // put your setup code here, to run once:
  ap.setupShiftRegisterControl(SR_CLOCK_PIN,SR_LATCH_PIN, SR_CLEAR_PIN, SR_OUTPUT_ENABLE_PIN);
  ap.setupHeadingDisplayDataPin(HEADING_DISPLAY_SER);
  ap.setupHeadingRotaryEncoder(HEADING_CONTROLLER_A_PIN,HEADING_CONTROLLER_B_PIN);
  ap.setHeading(0);

  attachInterrupt(digitalPinToInterrupt(3), interruptRotaryEncoder, RISING);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (false) {
    // testing the heading rotary encoder
    int hdg = ap.getHeading();
    Serial.print("Heading: ");
    Serial.println(hdg);
  }
  ap.updateDisplay();
  int hdg = ap.getHeading() + 3;
  ap.setHeading(hdg);
  delay(1000);
}

void interruptRotaryEncoder() {
  ap.onHeadingRotaryEncoderInterrupt();
}
