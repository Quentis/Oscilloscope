#include "OscilloscopeSettingsCallbacks.h"

OSC_Settings_IntegerContinuousValue_Type
OSC_Settings_IntegerContinuousCallback_BacklightIntensity(OSC_Settings_IntegerContinuous_Type*  this,
                                                  OSC_Settings_Event_Type               settingsEvent)
{
  OSC_Settings_IntegerContinuousCallback_Default(this,settingsEvent);
  OSC_LCD_BacklightPWM_Set(this->value);
  return this->value;
}

OSC_Settings_OnOffValue_Type
OSC_Settings_OnOffCallback_Channel_A_Status(OSC_Settings_OnOff_Type*                          this,
                                            OSC_Settings_Event_Type                           settingsEvent){
  OSC_Settings_OnOffCallback_Default(this,settingsEvent);
  #ifdef OSC_LED_INDICATION_ACTIVE
  if(this->status == OSC_CFG_CHANNEL_A_STATUS_ENABLED){
    OSC_LED_CHANNEL_A_SET();
  } else {
    OSC_LED_CHANNEL_A_CLEAR();
  }
  #endif
  return this->status;
}

OSC_Settings_OnOffValue_Type
OSC_Settings_OnOffCallback_Channel_B_Status(OSC_Settings_OnOff_Type*                          this,
                                            OSC_Settings_Event_Type                           settingsEvent){
  OSC_Settings_OnOffCallback_Default(this,settingsEvent);
  #ifdef OSC_LED_INDICATION_ACTIVE
  if(this->status == OSC_CFG_CHANNEL_B_STATUS_ENABLED){
    OSC_LED_CHANNEL_B_SET();
  } else {
    OSC_LED_CHANNEL_B_CLEAR();
  }
  #endif
  return this->status;
}
