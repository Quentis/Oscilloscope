#ifndef MONITORIO_H_
#define MONITORIO_H_

#include "stdint.h"
#include "MonitorUtils.h"
#include "MonitorTypes.h"

extern MTR_InDesc* defaultIn;
extern MTR_InDesc* defaultOut;

void MTR_sendChar(MTR_OutDesc* out,uint8_t ch);
MTR_Err MTR_sendString(MTR_OutDesc* out);
uint8_t MTR_getChar(MTR_InDesc* in);
MTR_Err MTR_getString(MTR_InDesc* in);

#endif /* MONITORIO_H_ */
