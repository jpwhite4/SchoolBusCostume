/* SchoolBusCostume management code
 */

#include "IRremote.h"

// Outputs
const uint8_t BRAKE_LIGHTS = 8;
const uint8_t LEFT_TOP_LIGHTS = 9;
const uint8_t RIGHT_TOP_LIGHTS = 10;
const uint8_t FRONT_FLASHERS = 11;

// Inputs
const uint8_t IR_REC_PIN = 12;

// IR Remote button definitions
const unsigned long POWER_BUTTON = 3810010651;
const unsigned long VOL_PLUS = 5316027;
const unsigned long FUNC_STOP = 4001918335;
const unsigned long PLAY_BUTTON = 3622325019;

// state management
enum runmode_t { DRIVE,
                 SLOW,
                 STOP,
                 TEST };

// Globals
IRrecv irrecv(IR_REC_PIN);
decode_results ir_result;
runmode_t mode = DRIVE;

void setup() {
  irrecv.enableIRIn();

  pinMode(BRAKE_LIGHTS, OUTPUT);
  pinMode(LEFT_TOP_LIGHTS, OUTPUT);
  pinMode(RIGHT_TOP_LIGHTS, OUTPUT);
  pinMode(FRONT_FLASHERS, OUTPUT);

  digitalWrite(BRAKE_LIGHTS, LOW);
  digitalWrite(LEFT_TOP_LIGHTS, LOW);
  digitalWrite(RIGHT_TOP_LIGHTS, LOW);
  digitalWrite(FRONT_FLASHERS, LOW);
#ifdef DEBUG
  Serial.begin(9600);
#endif
}

void loop() {
  if (irrecv.decode(&ir_result)) {
#ifdef DEBUG
    Serial.println(ir_result.value);
#endif
    switch (ir_result.value) {
      case POWER_BUTTON:
        mode = DRIVE;
#ifdef DEBUG
        Serial.println("drive");
#endif
        break;

      case VOL_PLUS:
        mode = SLOW;
#ifdef DEBUG
        Serial.println("slow");
#endif
        break;

      case FUNC_STOP:
#ifdef DEBUG
        Serial.println("stop");
#endif
        mode = STOP;
        break;

      case PLAY_BUTTON:
        mode = TEST;
        break;
    }
    irrecv.resume();
  }

  const unsigned long timeMs = millis();
  const unsigned long interval = timeMs / 500;

  switch (mode) {
    case DRIVE:
      digitalWrite(LEFT_TOP_LIGHTS, LOW);
      digitalWrite(RIGHT_TOP_LIGHTS, LOW);
      digitalWrite(FRONT_FLASHERS, LOW);
      digitalWrite(BRAKE_LIGHTS, timeMs % 8 == 0);
      break;
    case SLOW:
      digitalWrite(LEFT_TOP_LIGHTS, LOW);
      digitalWrite(RIGHT_TOP_LIGHTS, LOW);
      digitalWrite(FRONT_FLASHERS, interval % 2 == 0);
      digitalWrite(BRAKE_LIGHTS, interval % 2 == 0);
      break;
    case STOP:
      digitalWrite(LEFT_TOP_LIGHTS, interval % 2 == 0);
      digitalWrite(RIGHT_TOP_LIGHTS, interval % 2 == 1);
      digitalWrite(FRONT_FLASHERS, interval % 2 == 0);
      digitalWrite(BRAKE_LIGHTS, interval % 2 == 1);
      break;
    case TEST:
      digitalWrite(LEFT_TOP_LIGHTS, HIGH);
      digitalWrite(RIGHT_TOP_LIGHTS, HIGH);
      digitalWrite(FRONT_FLASHERS, HIGH);
      digitalWrite(BRAKE_LIGHTS, HIGH);
  }
}
