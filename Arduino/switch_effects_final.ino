
//-------------------------------------------------------------------------------//
//------------------------------- GENERAL DEFINES -------------------------------//
//-------------------------------------------------------------------------------//
#include <fix16.h>
#include <math.h>
#define fs 44100
#define fc_lf 400
#define fc_bump 1000
#define fc_hf 2000
#define fb 300
#define VOLUME 1
#define DELAY 2
#define EQUALIZER 3
#define REVERB 4
#define CHORUS 5
#define ECHO 6
#define HC06 Serial3
//buff
#define D1   2000      //  238 476
#define D2   2000      //  297 594
#define D3   2000      //  329 658
#define D4   2000      //  350 700
#define D5   4000     //  775 1550   
#define D6   2000      //  263 526 
//-----------------------------gardner small-------------------------------------
//-------------------------------------------------------------------------------
#define GARDNER_SMALL 2
#define s_D0 384
#define s_D1 560
#define s_D2 352
#define s_D3 133
#define s_D4 1056
#define s_D5 480
int s_DC0 = 0,s_DC1 = 0,s_DC2 = 0,s_DC3 = 0,s_DC4 = 0,s_DC5 = 0;
double s_g1 = 0.15,s_g2 = 0.25,s_g3 = 0.3,s_g4 = 0.08,s_g5 = 0.3,fracs,integers;
fix16_t s_G1,s_G2,s_G3,s_G4,s_G5;
fix16_t s_gpb1=44472,s_gpb2 = 41258,s_gpb = 12548;
fix16_t s_y = 0,s_x1 =0,s_x2=0,s_x3=0,s_x4=0;
fix16_t s_x5=0,s_x6=0,s_x7=0,s_x8=0,s_x9=0,s_x10=0,s_x11=0,s_x12=0;
//-----------------------------gardner small-------------------------------------
//-------------------------------------------------------------------------------


//------------------------gardner medium-----------------------------------------
//-------------------------------------------------------------------------------
#define GARDNER_MED 3
#define m_D1 560
#define m_D2 133
#define m_D3 352
#define m_D4 80
#define m_D5 480
#define m_D6 1072
#define m_D7 240
#define m_D8 624
#define m_D9 157
#define m_D10 1728
int m_DC1 = 0,m_DC2 = 0,m_DC3 = 0,m_DC4 = 0;
int m_DC5 = 0,m_DC6 = 0,m_DC7 = 0,m_DC8 = 0,m_DC9 = 0,m_DC10 = 0;
double m_g1 = 0.15,m_g2 = 0.25,m_g3 = 0.35,m_g4 = 0.45,m_g5 = 0.25;
double m_g6 = 0.35,fracm,integerm;
fix16_t m_G1,m_G2,m_G3,m_G4,m_G5,m_G6;
fix16_t m_gpb1=53886,m_gpb2 = 44472,m_gpb = 17560;
fix16_t m_y = 0,m_x1 =0,m_x2=0,m_x3=0,m_x4=0,m_outy1 = 0,m_atry = 0,m_outy2;
fix16_t m_atry2,m_hpy;
fix16_t m_x5=0,m_x6=0,m_x7=0,m_x8=0,m_x9=0,m_x10=0,m_x11=0,m_x12=0;

//------------------------gardner medium-----------------------------------------
//-------------------------------------------------------------------------------

//------------------------gardner large------------------------------------------
//-------------------------------------------------------------------------------
#define GARDNER_LARGE 4
#define l_D1 128
#define l_D2 192
#define l_D3 64
#define l_D4 272
#define l_D5 1392
#define l_D6 992
#define l_D7 496
#define l_D8 48
#define l_D9 1920
#define l_D10 1216
#define l_D11 480
int l_DC1 = 0,l_DC2 = 0,l_DC3 = 0,l_DC4 = 0,l_DC5 = 0;
int l_DC6 = 0,l_DC7 = 0,l_DC8 = 0,l_DC9 = 0,l_DC10 = 0,l_DC11 = 0;
double l_g1 = 0.2,l_g2 = 0.2,l_g3 = 0.2,l_g4 = 0.15,l_g5 = 0.3;
double l_g6 = 0.15,l_g7 = 0.15,fracl,integerl;
fix16_t l_G1,l_G2,l_G3,l_G4,l_G5,l_G6,l_G7;
fix16_t l_gpb1=53886,l_gpb2 = 44472,l_gpb = 17560;
fix16_t l_y = 0,l_x1 =0,l_x2=0,l_x3=0,l_x4=0,l_outy1 = 0,l_atry = 0,l_atr2y,l_out2y;
fix16_t l_x5=0,l_x6=0,l_x7=0,l_x8=0,l_x9=0,l_x10=0,l_x11=0,l_x12=0;

//------------------------gardner large------------------------------------------
//-------------------------------------------------------------------------------

//------------------------schroeder----------------------------------------------
//-------------------------------------------------------------------------------
#define SCHROEDER 1
fix16_t sc_a1 = 46393, sc_a2 = 42572, sc_a3 = 40632, sc_a4 = 39426, 
 sc_a5 = 28402 /*39322*/, sc_a6 = /*45875*/ 37118;
fix16_t sc_b1 = 64881,sc_b2 = 62259 ,sc_b3 = 58982 ,/*sc_b4 = 52428*/ sc_b4 = 39321;
fix16_t S1 = 0, S2 =  0, S3 =  0, S4 =  0, S5 = 0, S6 = 0, S7 = 0;
int DV1 = 950;
int DV2 = 1188;
int DV3 = 1316;
int DV4 = 1398;
int DV5 = 588;
int DV6 = 1425;
int DC1 = 0, DC2 = 0, DC3 = 0, DC4 = 0, DC5 = 1, DC6 = 1;
//------------------------schroeder----------------------------------------------
//-------------------------------------------------------------------------------


//------------------------ADC----------------------------------------------------
//-------------------------------------------------------------------------------
int in_ADC0, in_ADC1;  //variables for 2 ADCs values (ADC0, ADC1)
int  out_DAC0, out_DAC1; //variables for 3 pots (ADC8, ADC9, ADC10)
//------------------------ADC----------------------------------------------------
//-------------------------------------------------------------------------------


//-------------------------------------------------------------------------------//
//------------------------------ EQUALIZER DEFINES ------------------------------//
//-------------------------------------------------------------------------------//
int shelvlf_tb = 0,temp,temp2,temp4,temp5,temp6,bump_tb = 0,temp3,shelvhf_tb =0,volume = 0;
fix16_t actualSample,lastSample,lastoutLf,actualy1_lf,actualy_lf,lastouty1Lf;
fix16_t lastouty1Bump,actualy1Bump,seclastouty1Bump,seclastoutLf,lastoutyBump;
fix16_t abc_lf[16],H0[30],abc_bump[16],abc_hf[16];
fix16_t actualy_bump,d,b0,b1,b2,a1,a2;
fix16_t actualy1_hf,actualy_hf,lastouthf;
double mat_H0[30],mat_ac_lf[15],mat_ac_bump[15],mat_ac_hf[15]; //Filters response from matlab
double mat_ab_lf,integer,mat_d,mat_ab_bump,mat_ab_hf;

//-------------------------------------------------------------------------------//
//------------------------------ EQUALIZER DEFINES ------------------------------//
//-------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------//
//------------------------------ BLUETOOTH DEFINES ------------------------------//
//-------------------------------------------------------------------------------//
int CMD_ANALOG = 1;
int rxState, rxLength, rxCmd;
const int rxState_0_idle = 0;
const int rxState_1_sync = 1;  //SYNC_BYTE received
const int rxState_2_cmd = 2;   //length received, waiting CMD
//---- CONTROL VARIABLES ----//
 //Os sync byte são para variacoes de parametros de acordo com as barras
 //do aplicativo como são varios efeitos foram nomeados genericamente
 int PRM = 0, PRM2 = 0,PRM3 = 0,PRM4 = 0,PRM5 = 0,PRM6 = 0,PRM7 = 0;
 int SYNC_BYTE = 0xB0;
 int SYNC_BYTE2 = 0xB1;
 int SYNC_BYTE3 = 0xB2;
 int SYNC_BYTE4 = 0xB3;
 int SYNC_BYTE5 = 0xB4;
 int SYNC_BYTE6 = 0xB5;
 int SYNC_BYTE7 = 0xB6;
 int RETURN_BYTE = 0xFF;
 int VOLUME_BYTE = 0xA0;
 int DELAY_BYTE = 0xA1;
 int EQUALIZER_BYTE = 0xA7;
 int ECHO_BYTE = 0xA2;
 int REVERB_BYTE = 0xA3;
short legal;
char teste = false;
const int rxState_3_data = 3;  //CMD received, receiving data

//-------------------------------------------------------------------------------//
//------------------------------ BLUETOOTH DEFINES ------------------------------//
//-------------------------------------------------------------------------------//


//-------------------------------------------------------------------------------//
//------------------------------- GENERAL EFFECTS DEFINES -------------------------------//
//-------------------------------------------------------------------------------//
#define TRUE 1
#define GAIN  1        // change this
#define MAX_DELAY 18000    // DELAY BUFFER SIZE
uint16_t sDelayBuffer0[MAX_DELAY];
//uint16_t sDelayBuffer1[MAX_DELAY];
unsigned int DelayCounter = 0;
int flag_effect = 0, decim = 0;
int depth_delay = 0, intensity_delay = 0;
unsigned int Delay_Depth = MAX_DELAY,Delay = 0,Delay_Depth_echo,a =2;
int sample, count, LFO, n;
int echoi = 0,echoii = 0;
int delayi = 0;


//------------------------------------------------------------------------------------
//----------------------------REVERB GENERAL SETTINGS------------------------------------------
//------------------------------------------------------------------------------------
  fix16_t in0,in1,out0,out1;

int index5 = 0, index6 = 0;
fix16_t X1[D1] = {0};
fix16_t X2[D2] = {0};
fix16_t X3[D3] = {0};
fix16_t X4[D4] = {0};
fix16_t X5[D5] = {0};
fix16_t X6[D6] = {0};
//-------------------------------------------------------------------------------------
//----------------------------REVERB GENERAL SETTINGS------------------------------------------
//-------------------------------------------------------------------------------------

int effect=0;
int volume3 = 0;
//reverb functions
fix16_t gardner_small(fix16_t in0);
fix16_t gardner_medium(fix16_t in0);
fix16_t gardner_large(fix16_t in0);
fix16_t schroeder(fix16_t in0);
//------------------------------------------------------------------------------------



void geracoefs(){
  //--------------------SMALL_ROOM---------------------------------------------
  //---------------------------------------------------------------------------
  s_g1 *= 65536.0; s_g2 *= 65536.0; s_g3 *= 65536.0; s_g4 *= 65536.0; 
  s_g5 *= 65536.0;
  fracs = modf(s_g1,&integers);
  s_G1 = (fix16_t) integers; 
  fracs = modf(s_g2,&integers);
  s_G2 = (fix16_t) integers;
  fracs = modf(s_g3,&integers);
  s_G3 = (fix16_t) integers;
  fracs = modf(s_g4,&integers);
  s_G4 = (fix16_t) integers;
  fracs = modf(s_g5,&integers);
  s_G5 = (fix16_t) integers;
  //--------------------SMALL_ROOM---------------------------------------------
  //---------------------------------------------------------------------------
  
  //--------------------MEDIUM_ROOM---------------------------------------------
  //---------------------------------------------------------------------------
  m_g1 *= 65536.0; m_g2 *= 65536.0; m_g3 *= 65536.0; m_g4 *= 65536.0; m_g5 *= 65536.0;
  m_g6 *= 65536.0;
  fracm = modf(m_g1,&integerm);
  m_G1 = (fix16_t) integerm; 
  fracm = modf(m_g2,&integerm);
  m_G2 = (fix16_t) integerm;
  fracm = modf(m_g3,&integerm);
  m_G3 = (fix16_t) integerm;
  fracm = modf(m_g4,&integerm);
  m_G4 = (fix16_t) integerm;
  fracm = modf(m_g5,&integerm);
  m_G5 = (fix16_t) integerm;
  fracm = modf(m_g6,&integerm);
  m_G6 = (fix16_t) integerm;
  //--------------------MEDIUM_ROOM---------------------------------------------
  //----------------------------------------------------------------------------
  
 //--------------------LARGE_ROOM---------------------------------------------
 //----------------------------------------------------------------------------
  fracl = modf(l_g1,&integerl);
  l_G1 = (fix16_t) integerl; 
  fracl = modf(l_g2,&integerl);
  l_G2 = (fix16_t) integerl;
  fracl = modf(l_g3,&integerl);
  l_G3 = (fix16_t) integerl;
  fracl = modf(l_g4,&integerl);
  l_G4 = (fix16_t) integerl;
  fracl = modf(l_g5,&integerl);
  l_G5 = (fix16_t) integerl;
  fracl = modf(l_g6,&integerl);
  l_G6 = (fix16_t) integerl;
  fracl = modf(l_g7,&integerl);
  l_G7 = (fix16_t) integerl;
  
 //--------------------LARGE_ROOM---------------------------------------------
 //----------------------------------------------------------------------------
 
 //--------------------EQUALIZER---------------------------------------------
 //----------------------------------------------------------------------------
  double v0;
  double frac;
  int count = 0;
  mat_d = -1*cos(2*M_PI*fc_bump/fs);
  Serial.println(mat_d);  
  mat_d *= 65536.0;
  frac = modf(mat_d,&integer);
  d = (fix16_t) integer;
  Serial.println(d);
  mat_ab_lf = (tan(M_PI*fc_lf/fs)-1)/(tan(M_PI*fc_lf/fs)+1);
  mat_ab_bump = (tan(M_PI*fb/fs)-1)/(tan(M_PI*fb/fs)+1);
  mat_ab_hf = (tan(M_PI*fc_hf/fs)-1)/(tan(M_PI*fc_hf/fs)+1);
 
  Serial.println("Ab hf:");
  Serial.println(mat_ab_hf);
  for(int i = 0;i <30;i++){
    count = i -15;
    v0 = pow(10,((float)count/20.0));
    if(count < 0){
      mat_ac_lf[i] = (tan(M_PI*fc_lf/fs)-v0)/(tan(M_PI*fc_lf/fs)+v0);      
      mat_ac_bump[i] = (tan(M_PI*fb/fs)-v0)/(tan(M_PI*fb/fs)+v0);
      mat_ac_hf[i] = (v0*tan(M_PI*fc_hf/fs)-1)/(v0*tan(M_PI*fc_hf/fs)+1);
      Serial.println("mat_ac_hf:");
      Serial.println(mat_ac_hf[i],DEC);  
    }
    mat_H0[i] = v0 - 1;
    Serial.println("H0 :");
    Serial.println(mat_H0[i],DEC);
  }
  for( int i = 0;i<30;i++){
    if(mat_H0[i] >= 1){
     frac  = modf(mat_H0[i],&integer);
     frac *= 65535.0;
     integer *= 65536;
     H0[i] = integer;
     frac = modf(frac,&integer);
     H0[i] += (fix16_t)integer;
    Serial.print("H0[");Serial.print(i);Serial.println("]");
     Serial.println(H0[i]);
   H0[i] >>=1;  
    }
    else{
      frac = mat_H0[i]*65535.0;
      frac = modf(frac,&integer);
     H0[i] = (fix16_t)integer;
     Serial.print("H0[");Serial.print(i);Serial.println("]");
     Serial.println(H0[i]);
     H0[i] >>=1;
    }
  }
  Serial.println(mat_ab_lf);
  //shelving lf
  frac = mat_ab_lf*65535.0;
  frac = modf(frac,&integer);
  abc_lf[0] = (fix16_t)integer;
  //fim shelving lf
  //Parte do bump
  frac = mat_ab_bump*65535.0;
  frac = modf(frac,&integer);
  abc_bump[0] = (fix16_t)integer;
  //fim bump
  //shelving HF
  frac = mat_ab_hf*65535.0;
  frac = modf(frac,&integer);
  abc_hf[0] = (fix16_t)integer;
  //fim shelving hf
  Serial.println("ab_hf:");
  Serial.println(abc_hf[0],DEC);
  Serial.println(abc_hf[0],BIN);
  for(int i = 0; i<15;i++){
   //shelv_lf coefs
    frac = mat_ac_lf[i]*65535.0;
    frac = modf(frac,&integer);
    abc_lf[i+1] = (fix16_t)integer;
    
    //bump coefs
    frac = mat_ac_bump[i]*65535.0;
    frac = modf(frac,&integer);
    abc_bump[i+1] = (fix16_t)integer;
    
    //shelving hf coefs
    frac = mat_ac_hf[i]*65535.0;
    frac = modf(frac,&integer);
    abc_hf[i+1] = (fix16_t)integer;
    Serial.println("AC hf:");
    Serial.println(abc_hf[i+1],DEC);
    Serial.println(abc_hf[i+1],BIN);
 //--------------------EQUALIZER-------------------------------------------------
 //------------------------------------------------------------------------------
    }
}
void setup() {
   //turn on the timer clock in the power management controller
  Serial.begin(9600);
  HC06.begin(9600);
  pmc_set_writeprotect(false);
  pmc_enable_periph_clk(ID_TC4);
  pmc_enable_periph_clk(ID_TC7);
  geracoefs();
  //we want wavesel 01 with RC 
  TC_Configure(TC1,1,TC_CMR_WAVE|TC_CMR_WAVSEL_UP_RC|TC_CMR_TCCLKS_TIMER_CLOCK2);
  TC_SetRC(TC1, 1, 238); // sets <> 44.1 Khz interrupt rate
  TC_Start(TC1, 1);
 
  // enable timer interrupts on the timer
  TC1->TC_CHANNEL[1].TC_IER=TC_IER_CPCS;
  TC1->TC_CHANNEL[1].TC_IDR=~TC_IER_CPCS;
  
  TC_Configure(TC2,1,TC_CMR_WAVE|TC_CMR_WAVSEL_UP_RC|TC_CMR_TCCLKS_TIMER_CLOCK2);
  TC_SetRC(TC2, 1, 656); // sets <> 16 Khz interrupt rate
  TC_Start(TC2, 1);
   TC2->TC_CHANNEL[1].TC_IER=TC_IER_CPCS;
  TC2->TC_CHANNEL[1].TC_IDR=~TC_IER_CPCS;
 
  //Enable the interrupt in the nested vector interrupt controller 
  //TC4_IRQn where 4 is the timer number * timer channels (3) + the channel number 
  //(=(1*3)+1) for timer1 channel1 
  NVIC_EnableIRQ(TC4_IRQn);
  NVIC_EnableIRQ(TC7_IRQn);

  rxState = rxState_0_idle;
   ADC->ADC_MR |= 0x80;   // DAC in free running mode.
  ADC->ADC_CR=2;         // Starts ADC conversion.
  ADC->ADC_CHER=0x1CC0;  // Enable ADC channels 0 and 1.  
  analogWrite(DAC0,0);  // Enables DAC0
  analogWrite(DAC1,0);  // Enables DAC0
}

//-------------------------------------------------------------------------------//
//--------------------------------- LOOP SECTION --------------------------------//
//-------------------------------------------------------------------------------//
void loop() {
 while((ADC->ADC_ISR & 0x1CC0)!=0x1CC0);// wait for ADC 0, 1, 8, 9, 10 conversion complete.
    in_ADC0=ADC->ADC_CDR[7];               // read data from ADC0
    in_ADC1=ADC->ADC_CDR[6];               // read data from ADC1  
    while(HC06.available())
  {
    troca_param();
  }     
  if(flag_effect == EQUALIZER){
    temp = PRM2;              // parametro para o shelving_lf
    temp2 = PRM3;              // parametro para o bump
    temp3 = PRM4;            // parametro para o shelving_hf
  }
  else if(flag_effect == ECHO){
    temp = PRM2;              // parametro para a intensidade do echo 
    temp2 = PRM3;              // parametro para a profundidade do echo 
  }
  else if(flag_effect == DELAY){
  temp = PRM2;
  }
  
}

int tt = 0,r = 0;
//-------------------------------------------------------------------------------//
//------------------------------- BlUETOOTH FUNCTION ----------------------------//
//-------------------------------------------------------------------------------//
void troca_param(){
  
     // get the new byte:
     // Serial.println(PRM);
    char rxData = (int)HC06.read();
    teste = true;
 //   Serial.println(out_DAC0);
    //Serial.println("out1" + (int)out_DAC1);
    switch(rxState){
      case rxState_0_idle:
          if(rxData == SYNC_BYTE){
            rxState = rxState_1_sync;
            legal = 1;
       // Serial.println("SYNC_BYTE received");
          }
          else if(rxData == SYNC_BYTE2){
            rxState = rxState_1_sync;
            legal = 2;
         //Serial.println("SYNC_BYTE2 received");
          }
          else if(rxData == SYNC_BYTE3){
            rxState = rxState_1_sync;
            legal = 3;
           // Serial.println("SYNC_BYTE3 received");
          }
          else if(rxData == SYNC_BYTE4){
            rxState = rxState_1_sync;
            legal = 4;
           // Serial.println("SYNC_BYTE4 received");
          }
          else if(rxData == SYNC_BYTE5){
            rxState = rxState_1_sync;
            legal = 5;
          }
          else if(rxData == SYNC_BYTE6){
            rxState = rxState_1_sync;
            legal = 6;
          }
          else if(rxData == SYNC_BYTE7){
            rxState = rxState_1_sync;
            legal = 7;
          }
          else if(rxData == RETURN_BYTE){
            legal = 0;
            flag_effect = 0;
            rxState = rxState_1_sync;
        //   Serial.println("RETURN_BYTE received");
          }
          else if(rxData == VOLUME_BYTE){
            legal = 0;
            flag_effect = VOLUME;
            rxState = rxState_1_sync;
        //  Serial.println("VOLUME_BYTE received");
          }
          else if(rxData == DELAY_BYTE){
             legal = 0;
             PRM = 0;
            flag_effect = DELAY;
            rxState = rxState_1_sync;
       //      Serial.println("DELAY_BYTE received");
          }
          else if(rxData == EQUALIZER_BYTE){
             legal = 0;
             PRM2 =0;
             PRM3 = 0;
             PRM4 = 0;
            flag_effect = EQUALIZER;
            rxState = rxState_1_sync;
       //      Serial.println("EQUALIZER_BYTE received");
          }
          else if(rxData == ECHO_BYTE){
            legal = 0;
            PRM = 0;
            flag_effect = ECHO;
            rxState = rxState_1_sync;
       //     Serial.println("ECHO_BYTE received");
          }
          
          else if(rxData == REVERB_BYTE){
            legal = 0;
             PRM = 0;
             r = 0;
            flag_effect = REVERB;
            rxState = rxState_1_sync;
      //      Serial.println("REVERB_BYTE received");
          }
      break;
          
      case rxState_1_sync:
          rxLength = rxData;
     // Serial.println("Length: " + String(rxLength));
          rxLength--;
          rxState = rxState_2_cmd;
          
          break;

      case rxState_2_cmd:
          rxCmd = rxData;
      // Serial.println("CMD: " + String(rxCmd));
          
          rxLength--;
          rxState = rxState_3_data;
          break;
          
      case rxState_3_data:
         // Serial.println("data: " + String(rxData));
          //we have one command only
          if(legal==1){
            if(rxCmd==CMD_ANALOG){
              if(flag_effect == REVERB){
                PRM = map(rxData, 0, 99, 10, 2900);
              }
              else{
               PRM = map(rxData, 0, 99, 10, 4095);
              }
              Serial.println("parameter: " + String(PRM));
            }
          }
          else if(legal == 2){
            if(rxCmd==CMD_ANALOG){
              if(flag_effect == EQUALIZER || flag_effect == REVERB){
                if(flag_effect == REVERB){
                    PRM2 = rxData;
                    
            //      Serial.println("parameter2: " + String(PRM2));
                }
                else if(flag_effect == EQUALIZER)    
                  PRM2 = rxData;  
              }
              else if(flag_effect == ECHO)
                PRM2 = rxData;
              else  
                PRM2 = map(rxData, 0, 99, 10, 2900);
               //Adjust Delay Depth based in pot0 position.
              }
          }
          else if(legal == 3){
           if(rxCmd==CMD_ANALOG){
              if(flag_effect == EQUALIZER || flag_effect == REVERB){
                
                if(flag_effect == REVERB){
                    PRM3 = rxData;
              //      Serial.println("parameter3: " + String(PRM3));
                }
                else if(flag_effect == EQUALIZER){    
                  PRM3 = rxData;
              //    Serial.println("parameter3: " + String(PRM3));
                }
              }
              else 
                PRM3 = map(rxData, 0, 99, 10, 2900);
               //Adjust Delay Depth based in pot0 position.
              }
          }
          else if(legal == 4){
           if(rxCmd==CMD_ANALOG)
              if(flag_effect == EQUALIZER || flag_effect == REVERB){
                
                if(flag_effect == REVERB){
                    PRM4 = rxData;
               //     Serial.println("parameter4: " + String(PRM4));
                }
                else if(flag_effect == EQUALIZER)    
                  PRM4 = rxData;
              }
              else 
                PRM4 = map(rxData, 0, 99, 10, 2900);
          }
          else if(legal == 5){
                if(rxLength == 1){
                    PRM5 = rxData;
                }
          }
          else if(legal == 6){
                  PRM6 = rxData ;
                
          }
          else if(legal == 7){
             
                if(rxLength == 1){
                    PRM7 = rxData;
                 //   Serial.println("parameter7: " + String(PRM7));
                }
          }
          rxLength--;
          if(rxLength==0){
            rxState = rxState_0_idle;
          }

          break;
          
      default:
          rxState = rxState_0_idle;
    };
      
  }
  
//-------------------------------------------------------------------------------//
//------------------------------- BlUETOOTH FUNCTION ----------------------------//
//-------------------------------------------------------------------------------//



//-------------------------------------------------------------------------------//
//------------------------------- TIMER FUNCTION(AUDIO EFFECTS) -----------------//
//-------------------------------------------------------------------------------//

int vol_smother = 0,vol_regols =0,vol_rev = 0,shift_rev = 0;
float a_echo = 0;
int changedetect = -1, oldvol = 0,counter = 0,effect_act,changedetect2 = -2,counter2,delay_offset = 0,echo_offset = 0;
int Delay_out = 0;
boolean fade,fade2;
//fade for echo
int echo_changedetect = -1,echo_counter = 0,echo_changedetect2 = -2,echo_counter2;
boolean echo_fade,echo_fade2;
//fade for delay 
int delay_changedetect = -1, delay_counter = 0;
boolean delay_fade;
uint16_t echoin;

//EFFECTS in 16kHz at timer 7 interruption
void (TC7_Handler)(){

  if(flag_effect == REVERB){
    in0 = (fix16_t)(in_ADC0 << 16);
    if(changedetect != PRM2 && changedetect != -1){
      counter = 0;
      fade = true;
      fade2 = false;
      counter2 = 0;
    }
    if(changedetect2 != PRM5 && changedetect2 != -2){
      counter2 = 0;
      fade2 = true;
      fade = false;
      counter = 0;
    }
    switch(effect_act){
    
      case SCHROEDER:
        out0 = schroeder(in0);
      break;
      
      case GARDNER_SMALL:
        out0 = gardner_small(in0);
        break;
      case GARDNER_MED:
        out0 = gardner_med(in0);
        break;
      case GARDNER_LARGE:
       out0 = gardner_large(in0);
        break;
      
 
    }
     
  
    
  changedetect = PRM2;
  changedetect2 = PRM5;
  out_DAC0 = out0>>16;
  if(fade){
    if(volume3 > 0)
      volume3--;
    out_DAC0 = map(out_DAC0,0,4095,0,volume3);
    counter++;
    if(counter > 4000){
        effect_act = changedetect;
    }
    if(counter > 29000){
      fade = false;  
      counter = 0;
    }
  }
 /* else if(fade2){
    if(volume3 > 0)
      volume3--;
    out_DAC0 = map(out_DAC0,0,4095,0,volume3);
    counter2++;
    if(counter2 > 4000){
        shift_rev =  map(changedetect2,99,0,0,16);
    }
    if(counter2 > 19000){
      fade2 = false;  
      counter2 = 0;
    }
  }*/
  else{
  out_DAC0 = map(out_DAC0,0,4095,0,volume3);
  if(volume3 < PRM && (PRM < 1412 || PRM > 1590) )
    volume3++;
  else if(volume3 > PRM && (PRM < 1412 || PRM > 1590))
    volume3--;  
 }


  
  dacc_set_channel_selection(DACC_INTERFACE, 0);        //select DAC channel 0
  dacc_write_conversion_data(DACC_INTERFACE, out_DAC0);
  }
  
  
  else if(flag_effect == ECHO){
    

    
          a =  map(intensity_delay,0,99,0,4095);
          echoin = map(sDelayBuffer0[Delay_out],0,4095,0,a);
         // sDelayBuffer0[Delay] =  in_ADC0 + (sDelayBuffer0[Delay])>>a;
          //sDelayBuffer0[Delay] =  in_ADC0 + (uint16_t)(sDelayBuffer0[Delay_out]/a);
          //sDelayBuffer0[Delay] =  in_ADC0 + (uint16_t)(echoin);
           sDelayBuffer0[Delay] =  in_ADC0;
          Delay_Depth_echo = map(depth_delay>>2,0,2047,1,MAX_DELAY);
          echo_offset = MAX_DELAY - Delay_Depth_echo;
          Delay++;
          if(Delay >= MAX_DELAY) Delay = 0;
            Delay_out = Delay + echo_offset;
          if(Delay_out >= MAX_DELAY){
            Delay_out = Delay_out - MAX_DELAY;  
          }
          /*if(Delay >= Delay_Depth_echo) Delay = 0; 
          */
          
          out_DAC0 = (in_ADC0+ (echoin));
         // out_DAC0=map(out_DAC0,0,8100,1,volume);
  
  if(temp > intensity_delay){
            intensity_delay++;
        }
        else if(temp < intensity_delay){
            intensity_delay--;
        }
        
        if(temp2 > depth_delay){
          if(echoii > 6){
              echoii = 0;
              depth_delay++;
            }
            echoii++;
        }
          else if(temp2 < depth_delay){
            if(echoii > 6){
              echoii = 0;
              depth_delay--;
            }
            echoii++;
          }
        
  
  

  out_DAC0 = map(out_DAC0,0,4095,0,volume);
 if(PRM > volume && volume < 3300){
          volume++;
        } else if(PRM < volume)
            volume--;
  
  dacc_set_channel_selection(DACC_INTERFACE, 0);       //select DAC channel 0
  dacc_write_conversion_data(DACC_INTERFACE, out_DAC0);//write on DAC
  }
  else if(flag_effect == DELAY){
    
    
        sDelayBuffer0[DelayCounter] = in_ADC0;
   //     sDelayBuffer1[DelayCounter] = in_ADC1;
        Delay_Depth=map(intensity_delay>>2,0,1023,1,MAX_DELAY);
        delay_offset = MAX_DELAY - Delay_Depth;
        //Increse/reset delay counter.   
        DelayCounter++;
      /*  if(DelayCounter >= Delay_Depth) DelayCounter = 0; 
        out_DAC0 = ((sDelayBuffer0[DelayCounter]));
        out_DAC0=map(out_DAC0,0,4095,1,volume);
      */
      if(DelayCounter >= MAX_DELAY) DelayCounter = 0;
        Delay_out = DelayCounter + delay_offset;
      if(Delay_out >= MAX_DELAY)
        Delay_out = Delay_out - MAX_DELAY;  
      
   out_DAC0 = ((sDelayBuffer0[Delay_out]));
  out_DAC0 = map(out_DAC0,0,4095,0,volume);
 
 
 if(PRM > volume && volume < 3800){
          volume++;
        } else if(PRM < volume)
            volume--;
      
      
      
      
      dacc_set_channel_selection(DACC_INTERFACE, 0);       //select DAC channel 0
      dacc_write_conversion_data(DACC_INTERFACE, out_DAC0);//write on DAC
      
     /* if(temp > intensity_delay && DelayCounter == 0 )
          intensity_delay = temp;
      
      else if(temp < intensity_delay && DelayCounter == 0)
          intensity_delay = temp;
          
      
      if(PRM > volume && volume < 3800){
        volume++;
      } else if(PRM < volume)
          volume--;
          */
  }

  TC_GetStatus(TC2, 1);

}
int volume2 = 0;

//Effects at 48kHz at timer 4 interruption
void TC4_Handler()
{
   
  switch(flag_effect){
      case VOLUME:
        out_DAC0=map(in_ADC0,0,4095,0,volume2);
        out_DAC1=map(in_ADC1,0,4095,0,volume2);
        if(volume2 < PRM)
          volume2++;
        else if(volume2 > PRM)
           volume2--;  
      break;  
      
      case EQUALIZER:
       int i = 0;
       actualSample = in_ADC0 << 16;
       out_DAC0 = equ();
       out_DAC0=map(out_DAC0,0,4095,0,volume);
      //parte inserida para ocorrer um atraso na troca de valores dos pots e os deixar mais instaveis sem grandes variacoes
      //os valores de 21 a 24 geravam muito aliasing portanto foram retirados o que melhorou bastante o codigo,somente nos shelving esse problema é detectado
      if(temp > shelvlf_tb){
        if(i>6){
          i=0;
          shelvlf_tb++;
        }
        i++;
      }
      else if(temp < shelvlf_tb){
        if(i>6){
          i=0;
          shelvlf_tb--;
        }
        i++;
      }
      if(shelvlf_tb > 21 && shelvlf_tb < 24){
         if(shelvlf_tb >= 23)
            shelvlf_tb = 25;
         else
            shelvlf_tb = 21;
      }
      if(temp2 > bump_tb && temp2 < 30)
          bump_tb++;
        else if(temp2 < bump_tb)
            bump_tb--;      
       if(temp3 > shelvhf_tb)
          shelvhf_tb++;
        else if(temp3 < shelvhf_tb)
            shelvhf_tb--;
        if(shelvhf_tb > 21 && shelvhf_tb < 24){
           if(shelvhf_tb >= 23)
              shelvhf_tb = 25;
           else
              shelvhf_tb = 21;
        }
      if(PRM > volume){
        volume++;
      } else if(PRM < volume)
          volume--;
     
      if(in_ADC0 > 4094){
      //conferir se a entrada veio saturada, essa parte pode ser comentada na versao final do codigo
      //esta ai para detectar uma falha que nao pode ser corrigida
   //   Serial.println("sat entrada");
   //   Serial.println(in_ADC0);
      } 
      
      break;
      
      
    
      
  
  
  
  
  }
  
    //Clear status allowing the interrupt to be fired again.
    if(flag_effect == REVERB || flag_effect == ECHO || flag_effect == DELAY){
    }
  else{  
 dacc_set_channel_selection(DACC_INTERFACE, 0);       //select DAC channel 0
 dacc_write_conversion_data(DACC_INTERFACE, out_DAC0);//write on DAC
}
 TC_GetStatus(TC1, 1);
}
//-------------------------------------------------------------------------------//
//------------------------------- TIMER FUNCTION(AUDIO EFFECTS) -----------------//
//-------------------------------------------------------------------------------//


//---------------------------------------------------------------------------------//
//------------------------------- EQUALIZER(AUDIO EFFECTS) ------------------------//
//---------------------------------------------------------------------------------//
int equ(){
   fix16_t a1,TT;
  
  
/*----------------------------shelv low freqeuncy algorithm--------------------------------------------------------
-------------------------------------------------------------------------------------------------------------------*/
  
  
  //boost
  if(shelvlf_tb >= 16){
   actualy1_lf = fix16_mul(actualSample,abc_lf[0]);
  actualy1_lf = fix16_sub(fix16_add(actualy1_lf,lastSample),fix16_mul(abc_lf[0],lastouty1Lf));
  actualy_lf = fix16_mul(H0[shelvlf_tb]>>1,fix16_add(actualSample,actualy1_lf));
  actualy_lf = fix16_add(actualy_lf,actualSample);
  }
  else if(shelvlf_tb > 13 && shelvlf_tb < 16 )
    actualy_lf = actualSample; //evitar os ruidos na variacao dos pots, nessa faixa de valores de transicao
  
  //cut
  else{
    actualy1_lf = fix16_mul(actualSample,abc_lf[shelvlf_tb+1]);
  actualy1_lf = fix16_sub(fix16_add(actualy1_lf,lastSample),fix16_mul(abc_lf[shelvlf_tb+1],lastouty1Lf));
  actualy_lf = fix16_mul(H0[shelvlf_tb]>>1,fix16_add(actualSample,actualy1_lf));
  actualy_lf = fix16_add(actualy_lf,actualSample);
  }
  
//  actualy_lf = actualSample; //shelving desligado
   
/*------------------------------------------end shelving low frequency-----------------------------------------------
---------------------------------------------------------------------------------------------------------------------*/
   
   
/*-------------------------------------bump algorithm------------------------------------------------------------- 
------------------------------------------------------------------------------------------------------------------*/
   //reparem que como o bump nao tem problema na variacao dos pots ele nao precisa do bloco else if
   //boost
  if(bump_tb >= 15){
      a1 = fix16_sub(0,fix16_mul(fix16_mul(d,fix16_sub(65536,abc_bump[0])),lastouty1Bump));
    actualy1Bump = fix16_sub(0,abc_bump[0]);
    actualy1Bump = fix16_mul(actualy1Bump,actualy_lf);
    actualy1Bump = fix16_add(actualy1Bump,fix16_mul(fix16_mul(d,fix16_sub(65536,abc_bump[0])),lastoutLf));
    actualy1Bump = fix16_add(actualy1Bump,seclastoutLf);
    actualy1Bump = fix16_add(actualy1Bump,a1);
    actualy1Bump =fix16_add(actualy1Bump,fix16_mul(abc_bump[0],seclastouty1Bump));
    
    
    actualy_bump = fix16_mul(H0[bump_tb]>>1,fix16_sub(actualy_lf,actualy1Bump));
    actualy_bump = fix16_add(actualy_bump,actualy_lf);
    
      
  
  }
  
  //cut
  else{
    actualy1Bump = fix16_sub(0,abc_bump[bump_tb+1]);
    actualy1Bump = fix16_mul(actualy1Bump,actualy_lf);
    actualy1Bump = fix16_add(actualy1Bump,fix16_mul(fix16_mul(d,fix16_sub(65536,abc_bump[bump_tb+1])),lastoutLf));
    actualy1Bump = fix16_add(actualy1Bump,seclastoutLf);
    actualy1Bump = fix16_sub(actualy1Bump,fix16_mul(fix16_mul(d,fix16_sub(65536,abc_bump[bump_tb+1])),lastouty1Bump));
    actualy1Bump = fix16_add(actualy1Bump,fix16_mul(abc_bump[bump_tb+1],seclastouty1Bump));
    TT    = fix16_sub(0,actualy1Bump);
    actualy_bump = fix16_mul(H0[bump_tb],fix16_add(actualy_lf,TT));
    actualy_bump = fix16_add(actualy_bump,actualy_lf);
 
  }
    //actualy_bump = actualSample; //shelving lf e bump desligado
/*---------------------------------------------end bump-----------------------------------------------------------------
------------------------------------------------------------------------------------------------------------------------*/
  


  
/*----------------------------------------shelv high frequency algorithm ------------------------------------------------
-------------------------------------------------------------------------------------------------------------------------*/
  
    
    //boost
  if(shelvhf_tb >= 16){
   actualy1_hf = fix16_mul(actualy_bump,abc_hf[0]);
  actualy1_hf = fix16_sub(fix16_add(actualy1_hf,lastoutyBump),fix16_mul(abc_hf[0],lastouthf));
  actualy_hf = fix16_mul(H0[shelvhf_tb],fix16_sub(actualy_bump,actualy1_hf));
  actualy_hf = fix16_add(actualy_hf,actualy_bump);
  }
  else if(shelvhf_tb > 13 && shelvhf_tb < 16 )
    actualy_hf = actualy_bump; //evitar os ruidos na variacao dos pots, nessa faixa de valores de transicao
 
 //cut
  else{
    actualy1_hf = fix16_mul(actualy_bump,abc_hf[shelvhf_tb+1]);
  actualy1_hf = fix16_sub(fix16_add(actualy1_hf,lastoutyBump),fix16_mul(abc_hf[shelvhf_tb+1],lastouthf));
  actualy_hf = fix16_mul(H0[shelvhf_tb],fix16_sub(actualy_bump,actualy1_hf));
  actualy_hf = fix16_add(actualy_hf,actualy_bump);
  }
/*--------------------------------------end shelving high frequency--------------------------------------------------------
---------------------------------------------------------------------------------------------------------------------------*/
  //recepcao dos parametros de amostras passadas, 
  //sec de second é a n-2, last é n-1 e actual é n, lf para shelving lf, 
  //bump para o bump|peak e hf para o shelving hf
  
  seclastouty1Bump = lastouty1Bump;
  lastouty1Bump = actualy1Bump;
  seclastoutLf = lastoutLf;
  lastoutLf = actualy_lf;
  lastouthf = actualy1_hf;
  lastoutyBump = actualy_bump;
  lastouty1Lf = actualy1_lf;
   lastSample = actualSample;
   
 
  //shift de 16 devido as operacoes de ponto fixo olhem a entrada onde tem um shift para a parte alta de 16  
  return actualy_hf >> 16; //saida final 
//return actualy_lf >> 16; // saida shelving lf
//return actualy_bump >> 16; //saida bump
}
//---------------------------------------------------------------------------------//
//------------------------------- EQUALIZER(AUDIO EFFECTS) ------------------------//
//---------------------------------------------------------------------------------//


 
