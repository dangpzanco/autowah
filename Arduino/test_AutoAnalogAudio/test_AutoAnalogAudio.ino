#include <AutoAnalogAudio.h>
#include <autoWah.h>
#include <fix32.hpp>

AutoAnalog aaAudio;
autoWah myWah;
const uint8_t q = 16;
const double maxInt16 = 32767.0 / 32768.0;
const fp::fix32<q> max16(maxInt16);
const fp::fix32<q> min16(-1.0);

// int32_t bufferSize = MAX_BUFFER_SIZE; // bad size
// int32_t bufferSize = 32; // good size
int32_t bufferSize = 8;

// uint8_t dacChannel = 0; // Use DAC0 for output
uint8_t dacChannel = 1; // Use DAC1 for output
// uint8_t dacChannel = 2; // Use both DAC0 and DAC1

void DACC_Handler(void){ 
    //Link the DAC ISR/IRQ library. Called by the MCU when DAC is ready for data.
    aaAudio.dacHandler();
}

/*********************************************************/
void setup() {    
    // Setup aaAudio using both ADC and DAC
    aaAudio.begin(true, true);
    
    // Disable auto adjust of timers (ensure a constant sample rate)
    aaAudio.autoAdjust = false;
    
    // Set quantization to 12 bits
    aaAudio.adcBitsPerSample = 12;
    aaAudio.dacBitsPerSample = 12;
    
    // Set a 16kHz sample rate
    aaAudio.setSampleRate(44100);
    // aaAudio.setSampleRate(16000);

    // Set ADC channel
    // aaAudio.enableAdcChannel(0);
    aaAudio.enableAdcChannel(1);
    aaAudio.disableAdcChannel(0);
    
    // Start loading ADC buffers
    aaAudio.getADC(bufferSize);
    aaAudio.feedDAC(dacChannel, bufferSize);

    // Setup autoWah
    myWah.setSampleRate(44.1);
    myWah.setQualityFactor(0.5);
    
    // Initialize Toggle Switch as input
    pinMode(2, INPUT);
    analogReadResolution(12);

    // Debug
    // Serial.begin(250000);
}
/*********************************************************/

void updatePot() {
    // Read Toggle Switch state
    bool toggleSwitch = digitalRead(2);
    static int16_t atk = 2048;
    static int16_t rel = 2048;
    static int16_t mix = 2048;
    static int16_t range = 2048;
    static int16_t quality = 2048;

    int16_t POT0 = ADC->ADC_CDR[10];    // read data from ADC8        
    int16_t POT1 = ADC->ADC_CDR[11];    // read data from ADC9   
    int16_t POT2 = ADC->ADC_CDR[12];    // read data from ADC10 

    if (toggleSwitch)
    {
        // atk = analogRead(A8);
        // rel = analogRead(A10);
        atk = POT0;
        rel = POT2;

        fp::fix32<q> alphaA = (int16_t)((atk - 2048) << 4);
        fp::fix32<q> alphaR = (int16_t)((rel - 2048) << 4);
    }
    else{
        // range = analogRead(A8);
        // quality = analogRead(A10);
        range = POT0;
        quality = POT2;

        fp::fix32<q> bw = (int16_t)((range - 2048) << 4);
        fp::fix32<q> qFactor = (int16_t)((quality - 2048) << 4);
    }

    // mix = analogRead(A9);
    mix = POT1;
    fp::fix32<q> alphaMix = (int16_t)((mix - 2048) << 4);
    myWah.setMixing(alphaMix);
}

/*********************************************************/
void loop() {
    // Get samples from the ADC at the sample rate defined above
    // Note: This function only blocks if the ADC is currently sampling and autoAdjust is set to 0
    // As long as any additional code completes before the ADC is finished sampling, a continuous stream of ADC data
    // at the defined sample rate will be available 
    aaAudio.getADC(bufferSize);

    updatePot();

    // Pass-through
    for(int i = 0; i < bufferSize; i++){
        fp::fix32<q> x ((int16_t)((aaAudio.adcBuffer16[i]-2048) << 4));
        fp::fix32<q> y = myWah.runEffect(x);

        // Gain
        y.rawVal <<= 1;

        // Saturation
        if (y > max16) y = max16;
        else if (y < min16) y = min16;

        aaAudio.dacBuffer16[i] = (uint16_t)(((int16_t)y >> 4)+2048);

        // fp::fix32<q> x = (int16_t)aaAudio.adcBuffer16[i];
        // fp::fix32<q> y = myWah.runEffect(x);
        // aaAudio.dacBuffer16[i] = (uint16_t)((int16_t)y);
    }
    
    // Pass-through
    // for(int i = 0; i < bufferSize; i++){
    //    aaAudio.dacBuffer16[i] = aaAudio.adcBuffer16[i];
    // }

    // Debug
    // for(int i = 0; i < bufferSize; i++){
    //     fp::fix32<q> x ((int16_t)((aaAudio.adcBuffer16[i]-2048) << 4));
    //     fp::fix32<q> y = myWah.runEffect(x);
    //     aaAudio.dacBuffer16[i] = (int16_t)y >> 4;

    //     // Serial.print((int16_t)((aaAudio.adcBuffer16[i]-2048) << 4));
    //     // Serial.print(",");
    //     // Serial.println((int16_t)y);

    //     Serial.print(aaAudio.adcBuffer16[i]);
    //     Serial.print(",");
    //     Serial.println( (uint16_t)(((int16_t)y >> 4)+2048) );
    // }
    // Output
    aaAudio.feedDAC(dacChannel, bufferSize);
    
}
