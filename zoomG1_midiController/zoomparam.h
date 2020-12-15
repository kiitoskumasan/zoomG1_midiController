#pragma once
#ifndef _ZOOMPARAM_H
#define _ZOOMPARAM_H
#include <EEPROM.h>

#define MAX_ID_LENGTH 2

typedef struct _GX_PARAM {
  byte programID[3];
  byte bankSelect;
  byte patchNumber;
} GX_PARAM, *PGX_PARAM;

class EEPROMParam {
	protected:
		//MS_PARAM gxParam;
    char zoomProgramID[MAX_ID_LENGTH + 1];
	public:
	  EEPROMParam(const char *gxID, GX_PARAM &gxParam);
    void initializeParameter(GX_PARAM &gxParam);
    void setDefaultParameter(GX_PARAM &gxParam);
    void setParameterToEEPROM(GX_PARAM &gxParam);
    void getParameterFromEEPROM(GX_PARAM &gxParam);
    bool operator==(EEPROMParam &rhs);
};
#endif
