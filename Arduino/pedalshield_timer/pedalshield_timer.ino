
//-------------------------------------------------------------------------------//
//------------------------------- GENERAL DEFINES -------------------------------//
//-------------------------------------------------------------------------------//

int in_ADC0, in_ADC1;  //variables for 2 ADCs values (ADC0, ADC1)
int out_DAC0, out_DAC1; //variables for 2 DACs (DAC0, DAC1)

int POT0, POT1, POT2; //variables for 3 pots (ADC8, ADC9, ADC10)
int LED = 3;
int FOOTSWITCH = 7; 
int TOGGLE = 2; 

void setup() {
    //turn on the timer clock in the power management controller
    Serial.begin(9600);

    

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
    TC_SetRC(TC1, 1, 656); // sets <> 16 Khz interrupt rate
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


//-------------------------------------------------------------------------------//
//------------------------------- TIMER FUNCTION(AUDIO EFFECTS) -----------------//
//-------------------------------------------------------------------------------//

//Effects at 48kHz at timer 4 interruption
void TC4_Handler()
{
    while((ADC->ADC_ISR & 0x1CC0) != 0x1CC0);// wait for ADC 0, 1, 8, 9, 10 conversion complete.
    in_ADC0 = ADC->ADC_CDR[7];               // read data from ADC0
    in_ADC1 = ADC->ADC_CDR[6];               // read data from ADC1

    POT0=ADC->ADC_CDR[10];                 // read data from ADC8        
    POT1=ADC->ADC_CDR[11];                 // read data from ADC9   
    POT2=ADC->ADC_CDR[12];                 // read data from ADC10 

    //Add volume feature with POT2
    //out_DAC0 = map(in_ADC0,0,4095,1,POT2);
    //out_DAC1 = map(in_ADC1,0,4095,1,POT2);
    out_DAC0 = in_ADC0;
    out_DAC1 = in_ADC1;

    //Write the DACs
    dacc_set_channel_selection(DACC_INTERFACE, 0);       //select DAC channel 0
    dacc_write_conversion_data(DACC_INTERFACE, out_DAC0);//write on DAC
    dacc_set_channel_selection(DACC_INTERFACE, 1);       //select DAC channel 1
    dacc_write_conversion_data(DACC_INTERFACE, out_DAC1);//write on DAC

    // Timer shenanigangs
    TC_GetStatus(TC1, 1);
}




