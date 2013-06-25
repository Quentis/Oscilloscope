#ifndef OSCILLOSCOPECONFIGURATION_H_
#define OSCILLOSCOPECONFIGURATION_H_

#include "OscilloscopeSettings.h"
#include "OscilloscopeMenu.h"

#define OSC_CFG_TRIGGER_LEVEL_LOWER_BOUND           (-7500)
#define OSC_CFG_TRIGGER_LEVEL_UPPER_BOUND            (7500)

#define OSC_CFG_TRIGGER_SLOPE_RISING                     0
#define OSC_CFG_TRIGGER_SLOPE_FALLING                    1

#define OSC_CFG_CHANNEL_SELECT_CHANNEL_A_SELECTED        0
#define OSC_CFG_CHANNEL_SELECT_CHANNEL_B_SELECTED        1

#define OSC_CFG_TRIGGER_SOURCE_CHANNEL_A                 0
#define OSC_CFG_TRIGGER_SOURCE_CHANNEL_B                 1

#define OSC_CFG_CHANNEL_A_STATUS_DISABLED                0
#define OSC_CFG_CHANNEL_A_STATUS_ENABLED                 1

#define OSC_CFG_CHANNEL_B_STATUS_DISABLED                0
#define OSC_CFG_CHANNEL_B_STATUS_ENABLED                 1

#define OSC_CFG_DATA_ACQUISITION_MODE_SINGLE             0
#define OSC_CFG_DATA_ACQUISITION_MODE_REPETITIVE         1

#define OSC_CFG_DATA_PROCESSING_MODE_NORMAL              0
#define OSC_CFG_DATA_PROCESSING_MODE_AVERAGE             1
#define OSC_CFG_DATA_PROCESSING_MODE_PEAK                2
#define OSC_CFG_DATA_PROCESSING_MODE_COUNT               3

/*=================================== CALLBACK FUNCTION REFERENCES ====================================*/
extern OSC_Settings_IntegerContinuousValue_Type
OSC_Settings_IntegerContinuous_BacklightIntensity(OSC_Settings_IntegerContinuous_Type*  this,
                                                  OSC_Settings_Event_Type               settingsEvent);

/*=====================================================================================================*/
/*                                      HORIIZONTAL MENU ELEMENTS                                      */
/*=====================================================================================================*/

  extern OSC_Settings_IntegerContinuous_Type   OSC_Settings_TriggerPosition;
  extern OSC_Settings_IntegerDiscrete_Type     OSC_Settings_HorizontalResolution;
  extern OSC_Settings_IntegerContinuous_Type   OSC_Settings_HorizontalOffset;

  extern OSC_Menu_Element_Type OSC_Menu_Element_TriggerPosition;
  extern OSC_Menu_Element_Type OSC_Menu_Element_HorizontalResolution;
  extern OSC_Menu_Element_Type OSC_Menu_Element_HorizontalOffset;

/*######################################### HORIIZONTAL MENU ##########################################*/

  extern OSC_Menu_Type OSC_Menu_Horizontal;

/*=====================================================================================================*/
/*                                        VERTICAL MENU ELEMENTS                                       */
/*=====================================================================================================*/

  extern OSC_Settings_IntegerContinuous_Type   OSC_Settings_TriggerLevel;
  extern OSC_Settings_OnOff_Type               OSC_Settings_TriggerSlope;
  extern OSC_Settings_IntegerDiscrete_Type     OSC_Settings_VerticalResolution;
  extern OSC_Settings_IntegerContinuous_Type   OSC_Settings_VerticalOffset;

  extern OSC_Menu_Element_Type OSC_Menu_Element_TriggerLevel;
  extern OSC_Menu_Element_Type OSC_Menu_Element_TriggerSlope;
  extern OSC_Menu_Element_Type OSC_Menu_Element_VerticalResolution;
  extern OSC_Menu_Element_Type OSC_Menu_Element_VerticalOffset;

/*########################################## VERTICAL MENU ############################################*/

  extern OSC_Menu_Type OSC_Menu_Vertical;

/*=====================================================================================================*/
/*                                          MODE MENU ELEMENTS                                         */
/*=====================================================================================================*/

  extern OSC_Settings_OnOff_Type   OSC_Settings_ChannelSelect;
  extern OSC_Settings_OnOff_Type   OSC_Settings_TriggerSource;
  extern OSC_Settings_OnOff_Type   OSC_Settings_Channel_A_Status;
  extern OSC_Settings_OnOff_Type   OSC_Settings_Channel_B_Status;
  extern OSC_Settings_OnOff_Type   OSC_Settings_DataAcquisitionMode;
  extern OSC_Settings_Option_Type  OSC_Settings_DataProcessingMode;

  extern OSC_Menu_Element_Type OSC_Menu_Element_ChannelSelect;
  extern OSC_Menu_Element_Type OSC_Menu_Element_TriggerSource;
  extern OSC_Menu_Element_Type OSC_Menu_Element_Channel_A_Status;
  extern OSC_Menu_Element_Type OSC_Menu_Element_Channel_B_Status;
  extern OSC_Menu_Element_Type OSC_Menu_Element_DataAcquisitionMode;
  extern OSC_Menu_Element_Type OSC_Menu_Element_DataProcessingMode;

/*############################################ MODE MENU ##############################################*/

  extern OSC_Menu_Type OSC_Menu_Mode;

/*=====================================================================================================*/
/*                                         HARDWARE MENU ELEMENTS                                      */
/*=====================================================================================================*/

  extern OSC_Settings_IntegerContinuous_Type OSC_Settings_BacklightIntensity;
  extern OSC_Settings_IntegerContinuous_Type OSC_Settings_VoltagePerLSB;
  extern OSC_Settings_IntegerDiscrete_Type   OSC_Settings_SampleRate;

  extern OSC_Menu_Element_Type OSC_Menu_Element_BacklightIntensity;
  extern OSC_Menu_Element_Type OSC_Menu_Element_VoltagePerLSB;
  extern OSC_Menu_Element_Type OSC_Menu_Element_SampleRate;

/*########################################## HARDWARE MENU ############################################*/

  extern OSC_Menu_Type OSC_Menu_Hardware;

#endif /* OSCILLOSCOPECONFIGURATION_H_ */
