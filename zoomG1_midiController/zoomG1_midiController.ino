//
//   M5 Stack basic application program
//   ZOOM G1X FOUR Patch Change using MIDI over USB
//   create date:  2020/12/15
//   Author:       kiitos_kumasan
//
// Required library;
//
// M5Stack library
// https://github.com/m5stack/M5Stack
// button2 from Lennart Hennigs
// https://github.com/LennartHennigs/Button2
// for use with USB Host Shield 2.0 from Circuitsathome.com
// https://github.com/felis/USB_Host_Shield_2.0 
//
//

#include <M5Stack.h>
#include <Usb.h>
#include <usbhub.h>
#include <usbh_midi.h>
#include <Button2.h>
#include "zoomG1X.h"
#include "zoomparam.h"

//#define DEBUG_MODE

USB  Usb;
USBH_MIDI  Midi(&Usb);

int Preset;
static bool initCtl = true;
int xpos =  0;
int ypos = 50;

GX_PARAM gxParam;
EEPROMParam eepParam = EEPROMParam(PROGRAM_ID, gxParam); 

Button2 buttonUP = Button2(btnUP);
Button2 buttonRIGHT = Button2(btnRIGHT);
Button2 buttonLEFT = Button2(btnLEFT);
Button2 buttonDOWN = Button2(btnDOWN);

void buttonActionRIGHT()
{
  #ifdef DEBUG_MODE
  Serial.println("btnRIGHT");
  #endif
  Preset += 1;
  if (Preset > 59){
    Preset = 10;
  }
  SendMIDI(Preset);
}

void buttonActionLEFT()
{
  #ifdef DEBUG_MODE
  Serial.println("btnLEFT");
  #endif
  Preset -= 1;
  if (Preset < 10){
    Preset = 59;
  }
  SendMIDI(Preset);
}

void buttonActionUP()
{
  #ifdef DEBUG_MODE
  Serial.println("btnUP");
  #endif
  Preset += 10;
  if (Preset > 59){
    Preset = Preset - 50;
  }
  SendMIDI(Preset);
}

void buttonActionDOWN()
{
  #ifdef DEBUG_MODE
  Serial.println("btnDOWN");
  #endif
  Preset = Preset - 10;
  if (Preset < 10){
    Preset = Preset + 50;
  }
  SendMIDI(Preset);
}

void buttonSelect(Button2& btn)
{
  if (btn == buttonRIGHT){
    buttonActionRIGHT();  
  } else if (btn == buttonLEFT){
    buttonActionLEFT();
  } else if (btn == buttonUP){
    buttonActionUP();
  } else if (btn == buttonDOWN){
    buttonActionDOWN();
  }
}

void button_init()
{
  buttonUP.setClickHandler(buttonSelect);
  buttonRIGHT.setClickHandler(buttonSelect);
  buttonLEFT.setClickHandler(buttonSelect);
  buttonDOWN.setClickHandler(buttonSelect);
  buttonUP.setLongClickHandler(buttonSelect);
  buttonRIGHT.setLongClickHandler(buttonSelect);
  buttonLEFT.setLongClickHandler(buttonSelect);
  buttonDOWN.setLongClickHandler(buttonSelect);
}

void button_poll()
{
  buttonUP.loop();
  buttonRIGHT.loop();
  buttonLEFT.loop();
  buttonDOWN.loop(); 
}

void SendMIDI(int number)
{
  int bankNum;
  int prgNum;
  char numChar[3];
  char numUp[2];
  char numLow[2];
  
  sprintf(numChar, "%02d", number);
  numUp[0] = numChar[0];
  numUp[1] = NULL;
  numLow[0] = numChar[1];
  numLow[1] = NULL;
  
  bankNum = atoi(numUp) - 1;
  prgNum = atoi(numLow);
  
  Usb.Task();
  if( Usb.getUsbTaskState() == USB_STATE_RUNNING ){
    #ifdef DEBUG_MODE
    Serial.printf("SendMIDI bank:%d program:%d", bankNum, prgNum);
    #endif

    byte Message[3];                   // Construct the midi message (3 bytes)

    Message[0] = MIDI_CONTROL_CHANGE;
    Message[1] = 0x00;
    Message[2] = 0x00;    
    Midi.SendData(Message);            // Send the message

    Message[0] = MIDI_CONTROL_CHANGE;
    Message[1] = MIDI_BANK_SELECT;
    Message[2] = bankNum;              // bank Number set
    Midi.SendData(Message);            // Send the message
    
    Message[0] = MIDI_PROGRAM_CHANGE;
    Message[1] = (byte)prgNum;         // prgNum is the program/patch
    Midi.SendData(Message);            // Send the message

    delay(10);
    gxParam.bankSelect = (byte)bankNum;
    gxParam.patchNumber = (byte)prgNum;
    eepParam.setParameterToEEPROM(gxParam);
    DisplayNumber(Preset);

    //Usb.Init();
    initCtl = false;
  } else {
    #ifdef DEBUG_MODE
    Serial.printf("NOT USB_STATE_RUNNING\n");
    #endif
  }
}


void DisplayNumber(int number)
{
  char buf[3];

  M5.Lcd.setTextFont(7);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextDatum(1);
  xpos = M5.Lcd.width() / 2;
  //ypos = M5.Lcd.height() / 2;
  ypos = 40;
  M5.Lcd.setTextPadding(M5.Lcd.width());
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
  sprintf(buf, "%02d", number);
  M5.Lcd.drawCentreString(buf, xpos, ypos, 7);
  
}

void initDisplay()
{
  M5.Lcd.init();
  M5.Lcd.fillScreen(TFT_BLACK);   
}

void convertParamToPreset()
{
  int bankNum;
  int prgNum;
  char numChar[3];
  char numUp[2];
  char numLow[2];

  sprintf(numChar, "%d%d", gxParam.bankSelect + 1, gxParam.patchNumber); 
  Preset = atoi(numChar);
  Serial.printf("Preset = %02d\n", Preset);
}

//***************************************************************************************************
// main loop
//***************************************************************************************************
void loop()
{
  M5.update();
  Usb.Task();
  if (initCtl) {
    SendMIDI(Preset);    
  } else {
    button_poll();
  }
}

//***************************************************************************************************
// setup
//***************************************************************************************************
void setup()
{
  Serial.begin(115200);  
  initCtl = true;
  M5.begin(true, false, true);
  M5.Power.begin();

  eepParam = EEPROMParam(PROGRAM_ID, gxParam); 
  convertParamToPreset();

  initDisplay();
  button_init();
  
  if (Usb.Init() == -1){
    while(1);               // Halt
  }
  delay(200);
}
