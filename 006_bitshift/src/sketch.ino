#define LATCH_PIN 8  //Pin zu ST_CP vom 74HC595
#define CLOCK_PIN 12 //Pin zu SH_CP vom 74HC595
#define DATA_PIN 11  //Pin zu DS vom 74HC595
#define PIN_BUTTON1 2 // Player Button 1
#define PIN_BUTTON2 4 // Player Button 2

int currentPos = 0;

void setup()
{
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);
    pinMode(PIN_BUTTON1, INPUT_PULLUP);
    pinMode(PIN_BUTTON2, INPUT_PULLUP);
}

void loop()
{
    idle();
    // run();
    if (playerReady()) run();
}

void run() {
    int delayTime = 200;
    boolean playing = true;
    boolean hasButtonPressed[2];
    while (playing) {
        hasButtonPressed[0] = false;
        hasButtonPressed[1] = false;
        for (int i=0; i<8; i++) {
            registerWrite(i, HIGH);
            if (i < 7) {
                delay(2*delayTime);
            } else {
                if (!isButtonPressed(PIN_BUTTON1)) { // don't allow to just press the button down
                    for(int j=0; j<delayTime; j++){
                        delay(2);
                        if (isButtonPressed(PIN_BUTTON1)) hasButtonPressed[0] = true;
                    }
                } else {
                    playing = false;
                    break;
                }
            }
        }
        if (hasButtonPressed[0] != true) {
            playing = false;
            break;
        }
        for (int i=7; i>=0; i--) {
            registerWrite(i, HIGH);
            if (i == 0) {
                delay(2*delayTime);
            } else {
                if (!isButtonPressed(PIN_BUTTON2)) { // don't allow to just press the button down
                    for(int j=0; j<delayTime; j++){
                        delay(2);
                        if (isButtonPressed(PIN_BUTTON2)) hasButtonPressed[1] = true;
                    }
                } else {
                    playing = false;
                    break;
                }
            }
        }
        if (hasButtonPressed[1] != true) {
            playing = false;
            break;
        }
    }

}

void idle () {
    for (int i=20; i>=1; i--) {
        registerWriteByte(0x81);
        delay(3*i);
        registerWriteByte(0x7E);
        delay(3*i);   
    if (playerReady()) return;
    }

    for (int i=0; i<20; i++) {
        registerWriteByte(0x81);
        delay(3*i);
        registerWriteByte(0x7E);
        delay(3*i);   
    if (playerReady()) return;
    }

    for (int loop=0; loop<3; loop++) {
        for (int i=0; i<8; i++) {
            registerWrite(i, HIGH);
            delay(30);
        if (playerReady()) return;
        }
        if (loop < 2) {
            for (int i=7; i>=0; i--) {
                registerWrite(i, HIGH);
                delay(30);
            if (playerReady()) return;
            }     
        } 
    }
}


boolean playerReady () {
    return isButtonPressed(PIN_BUTTON1) && isButtonPressed(PIN_BUTTON2);
}


boolean isButtonPressed (int _whichPin)
{
    return !digitalRead(_whichPin);
}

void registerWriteByte(byte _whichByte)
{
    digitalWrite(LATCH_PIN, LOW);

    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, _whichByte);

    digitalWrite(LATCH_PIN, HIGH);

}

void registerWrite(int _whichPin, int _whichState)
{

    byte bitsToSend = 0;

    digitalWrite(LATCH_PIN, LOW);

    bitWrite(bitsToSend, _whichPin, _whichState);

    shiftOut(DATA_PIN, CLOCK_PIN, MSBFIRST, bitsToSend);

    digitalWrite(LATCH_PIN, HIGH);

}
