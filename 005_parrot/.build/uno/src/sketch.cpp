#include <Arduino.h>

void setup ();
void loop ();
#line 1 "src/sketch.ino"
#define PIN_LED 11
#define PIN_BUTTON 13

int lastState;
int buttonState;
int buttonSequence[100][2];
int buttonSequenceLength = 0;
int inputCounter = 0;
boolean playing = false;

long lastTimePressed = 0;
long timeoutDelay = 2000;

void setup () {
    Serial.begin(9600);
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_BUTTON, INPUT_PULLUP);
}

void loop () {
	int currentState = !digitalRead(PIN_BUTTON);

	if (currentState != lastState) {
        Serial.print("New State ");
        playing = false;
        buttonSequence[inputCounter][0] = currentState;
        buttonSequence[inputCounter][1] = (inputCounter > 0) ? millis() - lastTimePressed : 0;
        Serial.print(buttonSequence[inputCounter][0]);
        Serial.print(" ");
        Serial.print(buttonSequence[inputCounter][1]);
        Serial.println(" ");
        lastTimePressed = millis();
        buttonSequenceLength = inputCounter+1;
        inputCounter++;
        digitalWrite(PIN_LED, currentState);
	}

    if ((millis() - lastTimePressed) > timeoutDelay &&
        currentState != true &&
        buttonSequenceLength > 2)
    {
        playing = true;
        lastState = currentState;
        inputCounter = 0;
    }

    lastState = currentState;

	if (playing) {
	    for (int i = 0; i < buttonSequenceLength; i++) {
            digitalWrite(PIN_LED, buttonSequence[i][0]);
            Serial.print("Timeout: ");
            Serial.print(i);
            Serial.print(" ");
            Serial.print(buttonSequenceLength);
            Serial.print(" ");
            Serial.println(buttonSequence[i][1]);
            delay(buttonSequence[i][1]);
        }
        buttonState = false;
		playing = false;
        buttonSequenceLength = 0;

        digitalWrite(PIN_LED, LOW);

        memset(buttonSequence, 0, sizeof(buttonSequence[0][0]) * 100 * 2);
	}

}
