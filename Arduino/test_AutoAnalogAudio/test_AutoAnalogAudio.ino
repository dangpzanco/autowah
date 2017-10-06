#include <AutoAnalogAudio.h>

AutoAnalog aaAudio;

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
    aaAudio.setSampleRate(48000);

    // Set ADC channel
    // aaAudio.enableAdcChannel(0);
    aaAudio.enableAdcChannel(1);
    aaAudio.disableAdcChannel(0);
    
    // Start loading ADC buffers
    aaAudio.getADC(bufferSize);
    aaAudio.feedDAC(dacChannel, bufferSize);
}

/*********************************************************/
void loop() {
    // Get samples from the ADC at the sample rate defined above
    // Note: This function only blocks if the ADC is currently sampling and autoAdjust is set to 0
    // As long as any additional code completes before the ADC is finished sampling, a continuous stream of ADC data
    // at the defined sample rate will be available 
    aaAudio.getADC(bufferSize);

    // Pass-through
    for(int i = 0; i < bufferSize; i++){
        aaAudio.dacBuffer16[i] = aaAudio.adcBuffer16[i];
    }
    
    // Pass-through
    // for(int i = 0; i < bufferSize; i+=2){
    //    aaAudio.dacBuffer16[i] = aaAudio.adcBuffer16[i];
    //    aaAudio.dacBuffer16[i + 1] = aaAudio.adcBuffer16[i + 1];
    // }
    
    // Output
    aaAudio.feedDAC(dacChannel, bufferSize);
    
}
