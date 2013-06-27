#include "OscilloscopeConfiguration.h"

/*=====================================================================================================*/
/*                                      HORIIZONTAL MENU ELEMENTS                                      */
/*=====================================================================================================*/

/*========================================= TRIGGER POSITION ==========================================*/
    OSC_Settings_IntegerContinuous_Type OSC_Settings_TriggerPosition_Object = {
        63,                   /*value*/
        0,                    /*lowerBound*/
        100,                  /*upperBound*/
        1,                    /*incrementStepSingle*/
        10,                   /*incrementStepMultiple*/
        NULL,                 /*callback*/
        "TriggerPos",         /*name*/
        "%"                   /*unitName*/
    };

    OSC_Settings_Type OSC_Settings_TriggerPosition = {
        OSC_Settings_TypeInfo_IntegerContinuous,
        (void*)&OSC_Settings_TriggerPosition_Object
    };

/*======================================= HORIZONTAL RESOLUTION =======================================*/
    OSC_Settings_IntegerDiscreteValue_Type OSC_Settings_HorizontalResolution_valueSet[] = {20,40,100,500,1000};   /*us/div*/
    char* OSC_Settings_HorizontalResolution_nameOfValues[] = {"20us","40us","100us","500us","1ms"};

    OSC_Settings_IntegerDiscrete_Type OSC_Settings_HorizontalResolution_Object = {
        (OSC_Settings_IntegerDiscreteValue_Type*)
        OSC_Settings_HorizontalResolution_valueSet,         /*valueSet*/
        2,                                                  /*currentIndex*/
        5,                                                  /*length*/
        NULL,                                               /*callback*/
        (char** const)
        OSC_Settings_HorizontalResolution_nameOfValues,     /*nameOfValues*/
        "HorizontalRes"                                     /*name*/
    };

    OSC_Settings_Type OSC_Settings_HorizontalResolution = {
        OSC_settings_TypeInfo_IntegerDiscrete,
        (void*)&OSC_Settings_HorizontalResolution_Object
    };

/*========================================= HORIZONTAL OFFSET =========================================*/
    OSC_Settings_IntegerContinuous_Type OSC_Settings_HorizontalOffset_Object = {
        0,                    /*value*/
       -1280,                 /*lowerBound*/
        1280,                 /*upperBound*/
        1,                    /*incrementStepSingle*/
        10,                   /*incrementStepMultiple*/
        NULL,                 /*callback*/
        "HorizOffset",        /*name*/
        "p"                   /*unitName*/
    };

    OSC_Settings_Type OSC_Settings_HorizontalOffset = {
        OSC_Settings_TypeInfo_IntegerContinuous,
        (void*)&OSC_Settings_HorizontalOffset_Object
    };


/*########################################## HORIIZONTAL MENU #########################################*/

    OSC_Menu_Element_Type* OSC_Menu_ElementList_HorizontalMenu[] = {
        &OSC_Settings_TriggerPosition,
        &OSC_Settings_HorizontalResolution,
        &OSC_Settings_HorizontalOffset
    };

    OSC_Menu_Type OSC_Menu_Horizontal = {
        OSC_Menu_ElementList_HorizontalMenu,
        sizeof(OSC_Menu_ElementList_HorizontalMenu)/sizeof(OSC_Menu_ElementList_HorizontalMenu[0]),
        "== Horizontal Menu =="
    };

/*=====================================================================================================*/
/*                                        VERTICAL MENU ELEMENTS                                       */
/*=====================================================================================================*/

/*============================================ TRIGGER LEVEL ==========================================*/
    OSC_Settings_IntegerContinuous_Type OSC_Settings_TriggerLevel_Object = {
        0,                                        /*value*/
        OSC_CFG_TRIGGER_LEVEL_LOWER_BOUND,        /*lowerBound*/
        OSC_CFG_TRIGGER_LEVEL_UPPER_BOUND,        /*upperBound*/
        5,                                        /*incrementStepSingle*/
        100,                                      /*incrementStepMultiple*/
        NULL,                                     /*callback*/
        "TriggerLvl",                             /*name*/
        "mV"                                      /*unitName*/
    };

    OSC_Settings_Type OSC_Settings_TriggerLevel = {
        OSC_Settings_TypeInfo_IntegerContinuous,
        (void*)&OSC_Settings_TriggerLevel_Object
    };

/*============================================ TRIGGER SLOPE ==========================================*/
    char* OSC_Settings_TriggerSlope_statusNames[] = {OSC_FONT_CODE_EDGE_RISING,OSC_FONT_CODE_EDGE_FALLING};

    OSC_Settings_OnOff_Type OSC_Settings_TriggerSlope_Object = {
        OSC_CFG_TRIGGER_SLOPE_RISING,                       /*status*/
        NULL,                                               /*callback*/
        (char** const)
        OSC_Settings_TriggerSlope_statusNames,              /*statusNames*/
        "TriggerSlope"                                      /*name*/
    };

    OSC_Settings_Type OSC_Settings_TriggerSlope = {
        OSC_Settings_TypeInfo_OnOff,
        (void*)&OSC_Settings_TriggerSlope_Object
    };

/*=================================== CHANNEL A VERTICAL RESOLUTION ===================================*/
    OSC_Settings_IntegerDiscreteValue_Type OSC_Settings_Channel_A_VerticalResolution_valueSet[] = {500,1000,1500,2000,2500};
    char* OSC_Settings_Channel_A_VerticalResolution_nameOfValues[] = {"0.5V/dv","1V/dv","1.5V/dv","2V/dv","2.5V/dv"};

    OSC_Settings_IntegerDiscrete_Type OSC_Settings_Channel_A_VerticalResolution_Object = {
        (OSC_Settings_IntegerDiscreteValue_Type*)
        OSC_Settings_Channel_A_VerticalResolution_valueSet, /*valueSet*/
        2,                                                  /*currentIndex*/
        5,                                                  /*length*/
        NULL,                                               /*callback*/
        (char** const)
        OSC_Settings_Channel_A_VerticalResolution_nameOfValues,       /*nameOfValues*/
        "VerticalRes"                                       /*name*/
    };

    OSC_Settings_Type OSC_Settings_Channel_A_VerticalResolution = {
        OSC_settings_TypeInfo_IntegerDiscrete,
        (void*)&OSC_Settings_Channel_A_VerticalResolution_Object
    };

/*====================================== CHANNEL A VERTICAL OFFSET ====================================*/
    OSC_Settings_IntegerContinuous_Type OSC_Settings_Channel_A_VerticalOffset_Object = {
        0,                    /*value*/
       -64,                   /*lowerBound*/
        64,                   /*upperBound*/
        1,                    /*incrementStepSingle*/
        8,                    /*incrementStepMultiple*/
        NULL,                 /*callback*/
        "VertOffset",         /*name*/
        "p"                   /*unitName*/
    };

    OSC_Settings_Type OSC_Settings_Channel_A_VerticalOffset = {
        OSC_Settings_TypeInfo_IntegerContinuous,
        (void*)&OSC_Settings_Channel_A_VerticalOffset_Object
    };

/*=================================== CHANNEL B VERTICAL RESOLUTION ===================================*/
    OSC_Settings_IntegerDiscreteValue_Type OSC_Settings_Channel_B_VerticalResolution_valueSet[] = {500,1000,1500,2000,2500};
    char* OSC_Settings_Channel_B_VerticalResolution_nameOfValues[] = {"0.5V/dv","1V/dv","1.5V/dv","2V/dv","2.5V/dv"};

    OSC_Settings_IntegerDiscrete_Type OSC_Settings_Channel_B_VerticalResolution_Object = {
        (OSC_Settings_IntegerDiscreteValue_Type*)
        OSC_Settings_Channel_B_VerticalResolution_valueSet,           /*valueSet*/
        2,                                                  /*currentIndex*/
        5,                                                  /*length*/
        NULL,                                               /*callback*/
        (char** const)
        OSC_Settings_Channel_B_VerticalResolution_nameOfValues,       /*nameOfValues*/
        "VerticalRes"                                       /*name*/
    };

    OSC_Settings_Type OSC_Settings_Channel_B_VerticalResolution = {
        OSC_settings_TypeInfo_IntegerDiscrete,
        (void*)&OSC_Settings_Channel_B_VerticalResolution_Object
    };

    /*====================================== CHANNEL B VERTICAL OFFSET ====================================*/
    OSC_Settings_IntegerContinuous_Type OSC_Settings_Channel_B_VerticalOffset_Object = {
        0,                    /*value*/
       -64,                   /*lowerBound*/
        64,                   /*upperBound*/
        1,                    /*incrementStepSingle*/
        8,                    /*incrementStepMultiple*/
        NULL,                 /*callback*/
        "VertOffset",         /*name*/
        "p"                   /*unitName*/
    };

    OSC_Settings_Type OSC_Settings_Channel_B_VerticalOffset = {
        OSC_Settings_TypeInfo_IntegerContinuous,
        (void*)&OSC_Settings_Channel_B_VerticalOffset_Object
    };

/*########################################## VERTICAL MENU ############################################*/

    OSC_Menu_Element_Type* OSC_Menu_ElementList_VerticalMenu[] = {
        &OSC_Settings_TriggerLevel,
        &OSC_Settings_TriggerSlope,
        &OSC_Settings_Channel_A_VerticalResolution,
        &OSC_Settings_Channel_A_VerticalOffset,
        &OSC_Settings_Channel_B_VerticalResolution,
        &OSC_Settings_Channel_B_VerticalOffset
    };

    OSC_Menu_Type OSC_Menu_Vertical = {
        OSC_Menu_ElementList_VerticalMenu,
        sizeof(OSC_Menu_ElementList_VerticalMenu)/sizeof(OSC_Menu_ElementList_VerticalMenu[0]),
        "=== Vertical Menu ==="
    };

/*=====================================================================================================*/
/*                                          MODE MENU ELEMENTS                                         */
/*=====================================================================================================*/

/*=========================================== CHANNEL SELECT ==========================================*/
    char* OSC_Settings_ChannelSelect_statusNames[] = {"CHN_A","CHN_B"};

    OSC_Settings_OnOff_Type OSC_Settings_ChannelSelect_Object = {
        OSC_CFG_CHANNEL_SELECT_CHANNEL_A_SELECTED,         /*status*/
        NULL,                                              /*callback*/
        (char** const)
        OSC_Settings_ChannelSelect_statusNames,            /*statusNames*/
        "ChnSelect"                                        /*name*/
    };

    OSC_Settings_Type OSC_Settings_ChannelSelect = {
        OSC_Settings_TypeInfo_OnOff,
        (void*)&OSC_Settings_ChannelSelect_Object
    };

/*=========================================== TRIGGER SOURCE ==========================================*/
    char* OSC_Settings_TriggerSource_statusNames[] = {"CHN_A","CHN_B"};

    OSC_Settings_OnOff_Type OSC_Settings_TriggerSource_Object = {
        OSC_CFG_TRIGGER_SOURCE_CHANNEL_A,                  /*status*/
        NULL,                                              /*callback*/
        (char** const)
        OSC_Settings_TriggerSource_statusNames,            /*statusNames*/
        "TriggerSrc"                                       /*name*/
    };

    OSC_Settings_Type OSC_Settings_TriggerSource = {
        OSC_Settings_TypeInfo_OnOff,
        (void*)&OSC_Settings_TriggerSource_Object
    };

/*========================================== CHANNEL A STATUS =========================================*/
    char* OSC_Settings_Channel_A_Status_statusNames[] = {OSC_FONT_CODE_RADIOBUTTON_OFF,OSC_FONT_CODE_RADIOBUTTON_ON};

    OSC_Settings_OnOff_Type OSC_Settings_Channel_A_Status_Object = {
        OSC_CFG_CHANNEL_A_STATUS_DISABLED,                 /*status*/
        OSC_Settings_OnOffCallback_Channel_A_Status,       /*callback*/
        (char** const)
        OSC_Settings_Channel_A_Status_statusNames,         /*statusNames*/
        "Chn A Status"                                     /*name*/
    };

    OSC_Settings_Type OSC_Settings_Channel_A_Status = {
        OSC_Settings_TypeInfo_OnOff,
        (void*)&OSC_Settings_Channel_A_Status_Object
    };

/*========================================== CHANNEL B STATUS =========================================*/
    char* OSC_Settings_Channel_B_Status_statusNames[] = {OSC_FONT_CODE_RADIOBUTTON_OFF,OSC_FONT_CODE_RADIOBUTTON_ON};

    OSC_Settings_OnOff_Type OSC_Settings_Channel_B_Status_Object = {
        OSC_CFG_CHANNEL_B_STATUS_DISABLED,                 /*status*/
        OSC_Settings_OnOffCallback_Channel_B_Status,       /*callback*/
        (char** const)
        OSC_Settings_Channel_B_Status_statusNames,         /*statusNames*/
        "Chn B Status"                                     /*name*/
    };

    OSC_Settings_Type OSC_Settings_Channel_B_Status = {
        OSC_Settings_TypeInfo_OnOff,
        (void*)&OSC_Settings_Channel_B_Status_Object
    };

/*========================================= DATA ACQUISITION MODE =====================================*/
    char* OSC_Settings_DataAcquisitionMode_statusNames[] = {"Single","Repet"};

    OSC_Settings_OnOff_Type OSC_Settings_DataAcquisitionMode_Object = {
        OSC_CFG_DATA_ACQUISITION_MODE_SINGLE,              /*status*/
        NULL,                                              /*callback*/
        (char** const)
        OSC_Settings_DataAcquisitionMode_statusNames,      /*statusNames*/
        "DataAcqMode"                                      /*name*/
    };

    OSC_Settings_Type OSC_Settings_DataAcquisitionMode = {
        OSC_Settings_TypeInfo_OnOff,
        (void*)&OSC_Settings_DataAcquisitionMode_Object
    };

/*========================================== DATA PROCESSING MODE =====================================*/
    char* OSC_Settings_DataProcessingMode_optionNames[] = {"Normal","Avg","Peak"};

    OSC_Settings_Option_Type OSC_Settings_DataProcessingMode_Object = {
        OSC_CFG_DATA_PROCESSING_MODE_NORMAL,          /*optionID*/
        OSC_CFG_DATA_PROCESSING_MODE_COUNT,           /*optionCount*/
        NULL,                                         /*callback*/
        (char** const)
        OSC_Settings_DataProcessingMode_optionNames,  /*nameOfOptions*/
        "DataProcMode"                                /*name*/
    };

    OSC_Settings_Type OSC_Settings_DataProcessingMode = {
        OSC_Settings_TypeInfo_Option,
        (void*)&OSC_Settings_DataProcessingMode_Object
    };

/*============================================== WAVEFORM MODE ========================================*/
    char* OSC_Settings_WaveformMode_statusNames[] = {"Normal","Smooth"};

    OSC_Settings_OnOff_Type OSC_Settings_WaveformMode_Object = {
        OSC_CFG_WAVEFORM_MODE_NORMAL,                       /*status*/
        OSC_Settings_OnOffCallback_WaveformMode,            /*callback*/
        (char** const)
        OSC_Settings_WaveformMode_statusNames,              /*statusNames*/
        "WaveformMode"                                      /*name*/
    };

    OSC_Settings_Type OSC_Settings_WaveformMode = {
        OSC_Settings_TypeInfo_OnOff,
        (void*)&OSC_Settings_WaveformMode_Object
    };

/*############################################ MODE MENU ##############################################*/

    OSC_Menu_Element_Type* OSC_Menu_ElementList_ModeMenu[] = {
        &OSC_Settings_ChannelSelect,
        &OSC_Settings_TriggerSource,
        &OSC_Settings_Channel_A_Status,
        &OSC_Settings_Channel_B_Status,
        &OSC_Settings_DataAcquisitionMode,
        &OSC_Settings_DataProcessingMode,
        &OSC_Settings_WaveformMode
    };

    OSC_Menu_Type OSC_Menu_Mode = {
        OSC_Menu_ElementList_ModeMenu,
        sizeof(OSC_Menu_ElementList_ModeMenu)/sizeof(OSC_Menu_ElementList_ModeMenu[0]),
        "===== Mode Menu ====="
    };

/*=====================================================================================================*/
/*                                         HARDWARE MENU ELEMENTS                                      */
/*=====================================================================================================*/

/*========================================= BACKLIGHT INTENSITY =======================================*/

    OSC_Settings_IntegerContinuous_Type OSC_Settings_BacklightIntensity_Object = {
        100,                                                        /*value*/
        0,                                                          /*lowerBound*/
        100,                                                        /*upperBound*/
        1,                                                          /*incrementStepSingle*/
        10,                                                         /*incrementStepMultiple*/
        OSC_Settings_IntegerContinuousCallback_BacklightIntensity,  /*callback*/
        "BacklightInt",                                             /*name*/
        "%"                                                         /*unitName*/
    };

    OSC_Settings_Type OSC_Settings_BacklightIntensity = {
        OSC_Settings_TypeInfo_IntegerContinuous,
        (void*)&OSC_Settings_BacklightIntensity_Object
    };

/*=========================================== VOLTAGE PER LSB =========================================*/

    OSC_Settings_IntegerContinuous_Type OSC_Settings_VoltagePerLSB_Object = {
        57,                                                     /*value*/
        0,                                                      /*lowerBound*/
        1000,                                                   /*upperBound*/
        1,                                                      /*incrementStepSingle*/
        10,                                                     /*incrementStepMultiple*/
        NULL,                                                   /*callback*/
        "Voltage/LSB",                                          /*name*/
        "mV"                                                    /*unitName*/
    };

    OSC_Settings_Type OSC_Settings_VoltagePerLSB = {
        OSC_Settings_TypeInfo_IntegerContinuous,
        (void*)&OSC_Settings_VoltagePerLSB_Object
    };

/*============================================= SAMPLE RATE ===========================================*/
    OSC_Settings_IntegerDiscreteValue_Type OSC_Settings_SampleRate_valueSet[] = {500,1000};   /*sample/ms*/
    char* OSC_Settings_SampleRate_nameOfValues[] = {"0,5MS/s","1MS/s"};

    OSC_Settings_IntegerDiscrete_Type OSC_Settings_SampleRate_Object = {
        (OSC_Settings_IntegerDiscreteValue_Type*)
        OSC_Settings_SampleRate_valueSet,                   /*valueSet*/
        1,                                                  /*currentIndex*/
        2,                                                  /*length*/
        NULL,                                               /*callback*/
        (char** const)
        OSC_Settings_SampleRate_nameOfValues,               /*nameOfValues*/
        "SampleRate"                                        /*name*/
    };

    OSC_Settings_Type OSC_Settings_SampleRate = {
        OSC_settings_TypeInfo_IntegerDiscrete,
        (void*)&OSC_Settings_SampleRate_Object
    };

/*########################################## HARDWARE MENU ############################################*/

    OSC_Menu_Element_Type* OSC_Menu_ElementList_HardwareMenu[] = {
        &OSC_Settings_BacklightIntensity,
        &OSC_Settings_VoltagePerLSB,
        &OSC_Settings_SampleRate
    };

    OSC_Menu_Type OSC_Menu_Hardware = {
        OSC_Menu_ElementList_HardwareMenu,
        sizeof(OSC_Menu_ElementList_HardwareMenu)/sizeof(OSC_Menu_ElementList_HardwareMenu[0]),
        "=== Hardware Menu ==="
    };

/*#####################################################################################################*/
/*                                             MENU LIST                                               */
/*#####################################################################################################*/

    OSC_Menu_Type* OSC_MenuList[OSC_Menu_Name_Count] = {
        &OSC_Menu_Horizontal,       /*OSC_Menu_Name_HorizontalMenu*/
        &OSC_Menu_Vertical,         /*OSC_Menu_Name_VerticalMenu*/
        &OSC_Menu_Mode,             /*OSC_Menu_Name_ModeMenu*/
        &OSC_Menu_Hardware          /*OSC_Menu_Name_HardwareMenu*/
    };
