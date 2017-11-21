#include <autoWah.h>
#include <fix32.hpp>

autoWah myWah;
const uint8_t q = 16;
const double maxInt16 = 32767.0 / 32768.0;
const fp::fix32<q> max16(maxInt16);
const fp::fix32<q> min16(-1.0);
const fp::fix32<q> half(0.5);
// const fp::fix32<q> minQ(0.02);

// Quality Factor
const int32_t minQ = 0.02 * 32768;
const int32_t maxQ = 0.707 * 32768;

// Range (Bandwidth)
const int32_t minBW = 0.01 * 32768;
const int32_t maxBW = 0.7 * 32768;

// Attack
const int32_t minAtk = 32630;
const int32_t maxAtk = 32760;

// Release
const int32_t minRel = 32092;
const int32_t maxRel = 32767;
// const int32_t minRel = 1000;
// const int32_t maxRel = 32767;


const int toggleBufferSize = 10;

uint32_t in_ADC0, in_ADC1;  // variables for 2 ADCs values (ADC0, ADC1)
uint32_t out_DAC0, out_DAC1; // variables for 2 DACs (DAC0, DAC1)

int16_t POT0, POT1, POT2; // variables for 3 pots (ADC8, ADC9, ADC10)
int LED = 3;
int FOOTSWITCH = 7;
int TOGGLE = 2;

void setup() {
    // Turn on the timer clock in the Power Management Controller (PMC)
    // Atmel Software Framework (ASF)
    pmc_set_writeprotect(false); // Disable write protection in the PMC
    pmc_enable_periph_clk(ID_TC4); // Enable Timer 4


    /*------------------------------------- TC------------------------------------//
     * Sao um total de 9 canais de 32 bits
     * 
     * Podemos escolher essas frequencias de amostragem
     * 8KHz,  value = 1312 (10500000/8000 = 1312)
     * 16KHz, value = 656 (10500000/16000 = 656)
     * 32KHz, value = 328 (10500000/32000 = 328)
     * 44.1Hz, value = 238 (10500000/44100 = 238) -------------- 44.1KHz (1/44KHz=22.6us)
     * 48KHz, value = 218 (10500000/48000 = 218)
     * 88.2KHz, value = 119 (10500000/88200 = 119)
     * 96KHz, value = 109  (10500000/96000 = 109)
     */
    TC_Configure(TC1,1,TC_CMR_WAVE|TC_CMR_WAVSEL_UP_RC|TC_CMR_TCCLKS_TIMER_CLOCK2);
    TC_SetRC(TC1, 1, 238); // sets <> 16 Khz interrupt rate
    TC_Start(TC1, 1);

    // enable timer interrupts on the timer
    TC1->TC_CHANNEL[1].TC_IER=TC_IER_CPCS;
    TC1->TC_CHANNEL[1].TC_IDR=~TC_IER_CPCS;

    //Enable the interrupt in the nested vector interrupt controller 
    //TC4_IRQn where 4 is the timer number * timer channels (3) + the channel number 
    //(=(1*3)+1) for timer1 channel1 
    NVIC_EnableIRQ(TC4_IRQn);

    /* ADC & DAC Setup */

    ADC->ADC_MR |= 0x80;   // DAC in free running mode.
    ADC->ADC_CR=2;         // Starts ADC conversion.
    ADC->ADC_CHER=0x1CC0;  // Enable ADC channels 0 and 1. 
    analogWrite(DAC0,0);   // Enables DAC0
    analogWrite(DAC1,0);   // Enables DAC0

    // Setup autoWah effect
    myWah.setSampleRate(44.1);
    myWah.setQualityFactor(0.5);

    // Initialize Toggle Switch as input
    pinMode(2, INPUT);
}

//-------------------------------------------------------------------------------//
//--------------------------------- LOOP SECTION --------------------------------//
//-------------------------------------------------------------------------------//
void loop() {
    // while((ADC->ADC_ISR & 0x1CC0) != 0x1CC0);// wait for ADC 0, 1, 8, 9, 10 conversion complete.
    // in_ADC0 = ADC->ADC_CDR[7];               // read data from ADC0
    // in_ADC1 = ADC->ADC_CDR[6];               // read data from ADC1

    // POT0=ADC->ADC_CDR[10];                 // read data from ADC8        
    // POT1=ADC->ADC_CDR[11];                 // read data from ADC9   
    // POT2=ADC->ADC_CDR[12];                 // read data from ADC10 
}

void updatePot(int16_t leftPOT, int16_t middlePOT, int16_t rightPOT) {
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

    // Toggle Switch debouncing //

    if (toggleSwitch)
    {
        // atk = analogRead(A8);
        // rel = analogRead(A10);
        atk = leftPOT;
        rel = rightPOT;

        // fp::fix32<q> alphaA = (int16_t)(atk << 3);
        // fp::fix32<q> alphaR = (int16_t)(rel << 3);

        fp::fix32<q> alphaA = (int16_t)map(atk, 0, 4095, minAtk, maxAtk);
        fp::fix32<q> alphaR = (int16_t)map(rel, 0, 4095, minRel, maxRel);
        myWah.setAlphaA(alphaA);
        myWah.setAlphaR(alphaR);
    }
    else{
        // range = analogRead(A8);
        // quality = analogRead(A10);
        range = leftPOT;
        quality = rightPOT;

        // fp::fix32<q> bw = (int16_t)(range << 3);
        // fp::fix32<q> qFactor = (int16_t)(quality << 3) + minQ;

        fp::fix32<q> qFactor = (int16_t)map(quality, 0, 4095, minQ, maxQ);
        fp::fix32<q> bw = (int16_t)map(range, 0, 4095, minBW, maxBW);

        myWah.setQualityFactor(qFactor);
        myWah.setBandwidth(bw);
    }

    // mix = analogRead(A9);
    mix = middlePOT;
    fp::fix32<q> alphaMix = (int16_t)(mix << 3);
    myWah.setMixing(alphaMix);
}


//-------------------------------------------------------------------------------//
//------------------------------- TIMER FUNCTION(AUDIO EFFECTS) -----------------//
//-------------------------------------------------------------------------------//

//Effects at 48kHz at timer 4 interruption
void TC4_Handler()
{
    static uint16_t count = 0;

    while((ADC->ADC_ISR & 0x1CC0) != 0x1CC0);// wait for ADC 0, 1, 8, 9, 10 conversion complete.
    in_ADC0 = ADC->ADC_CDR[7];               // read data from ADC0
    in_ADC1 = ADC->ADC_CDR[6];               // read data from ADC1

    POT0 = ADC->ADC_CDR[10];                 // read data from ADC8        
    POT1 = ADC->ADC_CDR[11];                 // read data from ADC9   
    POT2 = ADC->ADC_CDR[12];                 // read data from ADC10 

    //Add volume feature with POT2
    //out_DAC0 = map(in_ADC0,0,4095,1,POT2);
    //out_DAC1 = map(in_ADC1,0,4095,1,POT2);
    // out_DAC0 = in_ADC0;
    // out_DAC1 = 0;

    // Update parameters each 10ms
    if (count++ >= 44100/100) {
        updatePot(POT0, POT1, POT2);
        count = 0;
    }

    // Run Effect
    // fp::fix32<q> x ((int16_t)(((int16_t)in_ADC0 - 2048) << 4));

    fp::fix32<q> x0 ((int16_t)(((int16_t)in_ADC0 - 2048) << 4));
    fp::fix32<q> x1 ((int16_t)(((int16_t)in_ADC1 - 2048) << 4));
    fp::fix32<q> x = (x0 - x1) * half;
    fp::fix32<q> y = myWah.runEffect(x);

    // Gain
    y.rawVal <<= 1;

    // Saturation
    if (y > max16) y = max16;
    else if (y < min16) y = min16;

    out_DAC0 = (uint32_t)(((int16_t)y >> 4)+2048);
    out_DAC1 = (uint32_t)(((int16_t)-y >> 4)+2048);
    // out_DAC1 = 0;

    //Write the DACs
    dacc_set_channel_selection(DACC_INTERFACE, 0);       //select DAC channel 0
    dacc_write_conversion_data(DACC_INTERFACE, out_DAC0);//write on DAC
    dacc_set_channel_selection(DACC_INTERFACE, 1);       //select DAC channel 1
    dacc_write_conversion_data(DACC_INTERFACE, out_DAC1);//write on DAC

    // Timer shenanigangs
    TC_GetStatus(TC1, 1);
}




