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
                                            OSC_Settings_Event_Type                           settingsEvent)
{
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
                                            OSC_Settings_Event_Type                           settingsEvent)
{
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

OSC_Settings_OnOffValue_Type
OSC_Settings_OnOffCallback_WaveformMode(OSC_Settings_OnOff_Type*                              this,
                                        OSC_Settings_Event_Type                               settingsEvent)
{
  OSC_Settings_OnOffCallback_Default(this,settingsEvent);
  if(this->status == OSC_CFG_WAVEFORM_MODE_NORMAL){
    OSC_DisplayManager_WaveformMode = OSC_DisplayManager_WaveformMode_Normal;
  } else {    /*this->status == OSC_CFG_WAVEFORM_MODE_SMOOTH*/
    OSC_DisplayManager_WaveformMode = OSC_DisplayManager_WaveformMode_Smooth;
  }
  return this->status;
}

OSC_Settings_IntegerDiscreteIndex_Type
OSC_Settings_IntegerDiscreteCallback_Channel_A_VerticalResolution(OSC_Settings_IntegerDiscrete_Type*    this,
                                                                  OSC_Settings_Event_Type               settingsEvent)
{
  uint32_t index;
  int32_t  voltagePerDivision,voltagePerPixel; /*It is in mVs*/
  index = OSC_Settings_IntegerDiscreteCallback_Default(this,settingsEvent);
  voltagePerDivision = this->valueSet[index];
  voltagePerPixel = voltagePerDivision / OSC_DSP_WAVEFORM_PIXEL_PER_VERTICAL_DIVISION;

  OSC_Settings_Channel_A_VerticalOffset_Object.incrementStepSingle   =      voltagePerPixel;
  OSC_Settings_Channel_A_VerticalOffset_Object.incrementStepMultiple = 10 * voltagePerPixel;

  OSC_Settings_Channel_A_VerticalOffset_Object.value -= (OSC_Settings_Channel_A_VerticalOffset_Object.value % voltagePerPixel);
  /*rounds down to the multiple of single step value*/
  return index;
}

OSC_Settings_IntegerDiscreteIndex_Type
OSC_Settings_IntegerDiscreteCallback_Channel_B_VerticalResolution(OSC_Settings_IntegerDiscrete_Type*    this,
                                                                  OSC_Settings_Event_Type               settingsEvent)
{
  uint32_t index;
  int32_t  voltagePerDivision,voltagePerPixel; /*It is in mVs*/
  index = OSC_Settings_IntegerDiscreteCallback_Default(this,settingsEvent);
  voltagePerDivision = this->valueSet[index];
  voltagePerPixel = voltagePerDivision / OSC_DSP_WAVEFORM_PIXEL_PER_VERTICAL_DIVISION;

  OSC_Settings_Channel_B_VerticalOffset_Object.incrementStepSingle   =      voltagePerPixel;
  OSC_Settings_Channel_B_VerticalOffset_Object.incrementStepMultiple = 10 * voltagePerPixel;

  OSC_Settings_Channel_B_VerticalOffset_Object.value -= (OSC_Settings_Channel_B_VerticalOffset_Object.value % voltagePerPixel);
  /*rounds down to the multiple of single step value*/
  return index;
}












