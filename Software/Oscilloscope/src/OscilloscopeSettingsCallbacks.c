#include "OscilloscopeSettingsCallbacks.h"

OSC_Settings_IntegerContinuousValue_Type
OSC_Settings_IntegerContinuous_BacklightIntensity(OSC_Settings_IntegerContinuous_Type*  this,
                                                  OSC_Settings_Event_Type                   menuEvent)
{
  OSC_Settings_IntegerContinuousCallback_Default(this,menuEvent);
  OSC_LCD_BacklightPWM_Set(this->value);
  return this->value;
}
