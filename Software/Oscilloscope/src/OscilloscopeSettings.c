#include "OscilloscopeSettings.h"

#define NULL 0

OSC_Settings_IntegerContinuousValue_Type   OSC_Settings_IntegerContinuousCallback_Default(OSC_Settings_IntegerContinuous_Type*  this,
                                                                                          OSC_Menu_Event_Type                   menuEvent)
{
  if(this == NULL) return ~0;
  switch(menuEvent){
    case OSC_Menu_Event_MenuElement_StepUpSingle:
      if((this->value + this->incrementStepSingle) > this->upperBound){
        this->value = this->upperBound;
      } else {
        this->value += this->incrementStepSingle;
      }
      break;
    case OSC_Menu_Event_MenuElement_StepUpMultiple:
      if((this->value + this->incrementStepMultiple) > this->upperBound){
        this->value = this->upperBound;
      } else {
        this->value += this->incrementStepMultiple;
      }
      break;
    case OSC_Menu_Event_MenuElement_StepDownSingle:
      if((this->value - this->incrementStepSingle) < this->lowerBound){
        this->value = this->lowerBound;
      } else {
        this->value -= this->incrementStepSingle;
      }
      break;
    case OSC_Menu_Event_MenuElement_StepDownMultiple:
      if((this->value - this->incrementStepMultiple) < this->lowerBound){
        this->value = this->lowerBound;
      } else {
        this->value -= this->incrementStepMultiple;
      }
      break;
    default: return ~0;
  }
  return this->value;
}

OSC_Settings_IntegerDiscreteIndex_Type     OSC_Settings_IntegerDiscreteCallback_Default(OSC_Settings_IntegerDiscrete_Type*    this,
                                                                                        OSC_Menu_Event_Type                   menuEvent)
{
  if(this == NULL) return ~0;
  switch(menuEvent){
    case OSC_Menu_Event_MenuElement_StepUpSingle:
    case OSC_Menu_Event_MenuElement_StepUpMultiple:
      this->currentIndex = ((this->currentIndex + 1) < this->length) ? this->currentIndex + 1: 0;
      break;

    case OSC_Menu_Event_MenuElement_StepDownSingle:
    case OSC_Menu_Event_MenuElement_StepDownMultiple:
      this->currentIndex = ((this->currentIndex == 0)? this->length : this->currentIndex) - 1;
      break;
    default: return ~0;
  }
  return this->currentIndex;
}

OSC_Settings_OnOffValue_Type               OSC_Settings_OnOffCallback_Default(OSC_Settings_OnOff_Type*                          this,
                                                                              OSC_Menu_Event_Type                               menuEvent)
{
  if(this == NULL) return ~0;
  switch(menuEvent){
    case OSC_Menu_Event_MenuElement_StepUpSingle:
    case OSC_Menu_Event_MenuElement_StepUpMultiple:
    case OSC_Menu_Event_MenuElement_StepDownSingle:
    case OSC_Menu_Event_MenuElement_StepDownMultiple:
      this->status = !this->status;
      break;
    default: return ~0;
  }
  return this->status;
}

OSC_Settings_OptionValue_Type              OSC_Settings_OptionCallback_Default(OSC_Settings_Option_Type*                        this,
                                                                               OSC_Menu_Event_Type                              menuEvent)
{
  if(this == NULL) return ~0;
  switch(menuEvent){
    case OSC_Menu_Event_MenuElement_StepUpSingle:
    case OSC_Menu_Event_MenuElement_StepUpMultiple:
      if(this->optionID == (this->optionCount - 1)){
        this->optionID = 0;
      } else {
        this->optionID++;
      }
      break;
    case OSC_Menu_Event_MenuElement_StepDownSingle:
    case OSC_Menu_Event_MenuElement_StepDownMultiple:
      if(this->optionID == 0){
        this->optionID = this->optionCount - 1;
      } else {
        this->optionID--;
      }
      break;
    default: return ~0;
  }
  return this->optionID;
}
