#include <fix32.hpp>

const uint8_t q = 16;

const int toggleBufferSize = 10;

void setup() {
    // initialize serial communication at 9600 bits per second:
    Serial.begin(9600);

    // Initialize Toggle Switch as input
    pinMode(2, INPUT);
    analogReadResolution(12);
}

void updatePot() {
    // Read Toggle Switch state
    bool toggleSwitch = digitalRead(2);
    static bool toggleBuffer[10] = { 0 };
    static int toggleIndex = 0;

    static int16_t atk = 2048;
    static int16_t rel = 2048;
    static int16_t mix = 2048;
    static int16_t range = 2048;
    static int16_t quality = 2048;


    // Toggle Switch debouncing //
    toggleBuffer[toggleIndex] = toggleSwitch;
    if (toggleIndex++ >= toggleBufferSize) toggleIndex = 0;

    int temp = 0;
    for (int i = 0; i < toggleBufferSize; ++i)
    {
        temp += toggleBuffer[i];
    }
    // if(temp == toggleBufferSize) toggleSwitch = true;
    // else toggleSwitch = false;


    if(temp > 0) toggleSwitch = true;
    else toggleSwitch = false;

    if (toggleSwitch)
    {
        atk = analogRead(A8);
        rel = analogRead(A10);

        fp::fix32<q> alphaA = (int16_t)((atk - 2048) << 4);
        fp::fix32<q> alphaR = (int16_t)((rel - 2048) << 4);
    }
    else{
        range = analogRead(A8);
        quality = analogRead(A10);

        fp::fix32<q> bw = (int16_t)((range - 2048) << 4);
        fp::fix32<q> qFactor = (int16_t)((quality - 2048) << 4);
    }

    mix = analogRead(A9);
    fp::fix32<q> alphaMix = (int16_t)((mix - 2048) << 4);

    // // Print
    Serial.println(toggleSwitch);
    // Serial.print(atk);
    // Serial.print(",");
    // Serial.print(rel);
    // Serial.print(",");
    // Serial.print(range);
    // Serial.print(",");
    // Serial.print(quality);
    // Serial.print(",");
    // Serial.println(mix);
}

// the loop routine runs over and over again forever:
void loop() {
    

    updatePot();

    // bool toggleSwitch = digitalRead(2);
    // Serial.println(toggleSwitch);
    delay(1);
}