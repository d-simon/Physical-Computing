#define PIN_LED 11
#define PIN_BUTTON 13

int lastState;
int buttonSequence[100][2];
int buttonSequenceLength = 0;
int inputCounter = 0;
boolean isPlaying = false;

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
        isPlaying = false;
        buttonSequence[inputCounter][0] = currentState;
        buttonSequence[inputCounter][1] = (inputCounter > 0) ? millis() - lastTimePressed : 0;
        lastTimePressed = millis();
        buttonSequenceLength = inputCounter+1;
        inputCounter++;
        digitalWrite(PIN_LED, currentState);
	}

    if ((millis() - lastTimePressed) > timeoutDelay
        && currentState != true
        && buttonSequenceLength > 1)
    {
        isPlaying = true;
        inputCounter = 0;
    }

    lastState = currentState;

	if (isPlaying) {
	    for (int i = 0; i < buttonSequenceLength; i++) {
            digitalWrite(PIN_LED, buttonSequence[i][0]);
            delay(buttonSequence[i][1]);
        }

		isPlaying = false;
        buttonSequenceLength = 0;

        digitalWrite(PIN_LED, LOW);

        // Clear Sequence
        memset(buttonSequence, 0, sizeof(buttonSequence[0][0]) * 100 * 2);
	}

}
