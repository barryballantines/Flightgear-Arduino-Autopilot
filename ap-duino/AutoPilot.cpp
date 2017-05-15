#include "AutoPilot.h"

#define SPACE 10
#define MINUS 11
#define DOT   12

byte headingBytes[4];

AutoPilot::AutoPilot() {
    // nothing to do yet...
}

AutoPilot::~AutoPilot() {
    // nothing to destruct yet...
}

void AutoPilot::setupShiftRegisterControl(byte clockPin, byte latchPin, byte clearPin, byte outputEnablePin) {
  _shiftRegClockPin = clockPin;
  _shiftRegLatchPin = latchPin;
  _shiftRegClearPin = clearPin;
  _shiftRegOutputEnablePin = outputEnablePin;

  pinMode(_shiftRegClockPin, OUTPUT);
  pinMode(_shiftRegLatchPin, OUTPUT);
  pinMode(_shiftRegClearPin, OUTPUT);
  pinMode(_shiftRegOutputEnablePin, OUTPUT);
  digitalWrite(_shiftRegClockPin, LOW);
  digitalWrite(_shiftRegLatchPin, LOW);
  digitalWrite(_shiftRegClearPin, LOW);
  digitalWrite(_shiftRegClearPin, HIGH);
  digitalWrite(_shiftRegOutputEnablePin, LOW);
}

void AutoPilot::setupHeadingDisplayDataPin(byte dataPin) {
  _headingDisplayDataPin = dataPin;
  pinMode(_headingDisplayDataPin, OUTPUT);
  digitalWrite(_headingDisplayDataPin, LOW);
}

void AutoPilot::setupHeadingRotaryEncoder(byte pinA, byte pinB) {
    _headingEncoderPinA = pinA;
    _headingEncoderPinB = pinB;
    pinMode(_headingEncoderPinA, INPUT_PULLUP);
    pinMode(_headingEncoderPinB, INPUT_PULLUP);
}

void AutoPilot::setHeading(int hdg) {
    _heading = (hdg+360) % 360;
}

int AutoPilot::getHeading() {
    return _heading;
}

void AutoPilot::updateDisplay() {
    // heading...
    translateUnsignedIntToByteArray(_heading, headingBytes);

    digitalWrite(_shiftRegLatchPin, LOW);
    for (int i=0; i<5; i++) {
      shiftOut(_headingDisplayDataPin, _shiftRegClockPin, LSBFIRST, headingBytes[i]);
    }
    digitalWrite(_shiftRegLatchPin, HIGH);
}


void AutoPilot::onHeadingRotaryEncoderInterrupt() {
    int valA = digitalRead(_headingEncoderPinA);
    int valB = digitalRead(_headingEncoderPinB);
    if (valA == valB) {
        setHeading(_heading - 1);
    }
    else {
        setHeading(_heading + 1);
    }
}


void AutoPilot::testHeadingDisplay() {
  for (int i=0; i<13; i++) {
    digitalWrite(_shiftRegLatchPin, LOW);
    byte segments = DIGIT_TO_BYTES[i];
    shiftOut(_headingDisplayDataPin, _shiftRegClockPin, LSBFIRST, segments);
    digitalWrite(_shiftRegLatchPin, HIGH);
    delay(1000);
  }
}

// === Utility Methods ===

byte digitToByte(int digit) {
  if (digit>=0 && digit<13) {
    return DIGIT_TO_BYTES[digit];
  } else {
    return B00000000;
  }
}

void translateUnsignedIntToByteArray(unsigned int input, byte output[5]) {
  if (input > 99999) {
    for (int i=0; i<5; i++) {
      output[i] = B00000010;
    }
  }
  else {
    unsigned int modulo = 10000;
    unsigned int value = input;
    for (int i=0; i<5; i++) {
      int digit = value / modulo;
      output[i] = digitToByte(digit);
      value = value - digit * modulo;
      modulo = modulo / 10;
    }
  }
}

