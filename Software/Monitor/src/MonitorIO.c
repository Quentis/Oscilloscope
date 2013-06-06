#include "MonitorIO.h"

MTR_InDesc* defaultIn;
MTR_InDesc* defaultOut;

void MTR_sendChar(MTR_OutDesc* out,uint8_t ch)
{
  out->sendChar(ch);
  return;
}

MTR_Err MTR_sendString(MTR_OutDesc* out)
{
  return out->sendString(out->outputBuffer,out->outputBufferLength);
}

uint8_t MTR_getChar(MTR_InDesc* in)
{
  return in->getChar();
}

MTR_Err MTR_getString(MTR_InDesc* in)
{
  return in->getString(in->inputBuffer,in->inputBufferLength,in->separator);
}
