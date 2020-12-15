#include <EEPROM.h>
#include "zoomparam.h"

EEPROMParam::EEPROMParam(const char *gxID, GX_PARAM &gxParam)
{
  strncpy(zoomProgramID, gxID, MAX_ID_LENGTH);
  zoomProgramID[MAX_ID_LENGTH] = NULL;
  
  getParameterFromEEPROM(gxParam);
  if (strcmp((char *)gxParam.programID, gxID) != 0){
    setDefaultParameter(gxParam);
    setParameterToEEPROM(gxParam);
    getParameterFromEEPROM(gxParam);
  }
}

bool EEPROMParam::operator==(EEPROMParam &rhs) 
{
  return (this==&rhs);
}

void EEPROMParam::initializeParameter(GX_PARAM &gxParam)
{
  gxParam.programID[0] = NULL;
  gxParam.bankSelect = 0;
  gxParam.patchNumber = 1;
}

void EEPROMParam::setDefaultParameter(GX_PARAM &gxParam)
{
  strcpy((char *)gxParam.programID, zoomProgramID);
  gxParam.bankSelect = 0;
  gxParam.patchNumber = 1;
}

void EEPROMParam::setParameterToEEPROM(GX_PARAM &gxParam)
{
  EEPROM.begin(sizeof(GX_PARAM));
  EEPROM.put(0, gxParam);
  EEPROM.commit();
}

void EEPROMParam::getParameterFromEEPROM(GX_PARAM &gxParam)
{
  EEPROM.begin(sizeof(GX_PARAM));
  EEPROM.get(0, gxParam);
  EEPROM.commit();
}
