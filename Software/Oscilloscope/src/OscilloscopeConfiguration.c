#include "OscilloscopeConfiguration.h"
#include "OscilloscopeFont.h"
/*=====================================================================================================*/
/*                                      HORIIZONTAL MENU ELEMENTS                                      */
/*=====================================================================================================*/

/*========================================= TRIGGER POSITION ==========================================*/
    OSC_Settings_IntegerContinuous_Type OSC_Settings_TriggerPosition = {
        63,                   /*value*/
        0,                    /*lowerBound*/
        100,                  /*upperBound*/
        1,                    /*incrementStepSingle*/
        10,                   /*incrementStepMultiple*/
        NULL,                 /*callback*/
        "TriggerPos",         /*name*/
        "%"                   /*unitName*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_TriggerPosition = {
        OSC_Settings_TypeInfo_IntegerContinuous,
        (void*)&OSC_Settings_TriggerPosition
    };

/*======================================= HORIZONTAL RESOLUTION =======================================*/
    OSC_Settings_IntegerDiscreteValue_Type OSC_Settings_HorizontalResolution_valueSet[] = {20,40,100,500,1000};   /*us/div*/
    char* OSC_Settings_HorizontalResolution_nameOfValues[] = {"20us","40us","100us","500us","1ms"};

    OSC_Settings_IntegerDiscrete_Type OSC_Settings_HorizontalResolution = {
        (OSC_Settings_IntegerDiscreteValue_Type*)
        OSC_Settings_HorizontalResolution_valueSet,         /*valueSet*/
        2,                                                  /*currentIndex*/
        5,                                                  /*length*/
        NULL,                                               /*callback*/
        (char** const)
        OSC_Settings_HorizontalResolution_nameOfValues,     /*nameOfValues*/
        "HorizontalRes"                                     /*name*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_HorizontalResolution = {
        OSC_settings_TypeInfo_IntegerDiscrete,
        (void*)&OSC_Settings_HorizontalResolution
    };

/*========================================= HORIZONTAL OFFSET =========================================*/
    OSC_Settings_IntegerContinuous_Type OSC_Settings_HorizontalOffset = {
        0,                    /*value*/
       -1280,                 /*lowerBound*/
        1280,                 /*upperBound*/
        1,                    /*incrementStepSingle*/
        10,                   /*incrementStepMultiple*/
        NULL,                 /*callback*/
        "HorizOffset",        /*name*/
        "p"                   /*unitName*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_HorizontalOffset = {
        OSC_Settings_TypeInfo_IntegerContinuous,
        (void*)&OSC_Settings_HorizontalOffset
    };


/*########################################## HORIIZONTAL MENU #########################################*/

    OSC_Menu_Element_Type* OSC_Menu_ElementList_HorizontalMenu[] = {
        &OSC_Menu_Element_TriggerPosition,
        &OSC_Menu_Element_HorizontalResolution,
        &OSC_Menu_Element_HorizontalOffset
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
    OSC_Settings_IntegerContinuous_Type OSC_Settings_TriggerLevel = {
        0,                                        /*value*/
        OSC_CFG_TRIGGER_LEVEL_LOWER_BOUND,        /*lowerBound*/
        OSC_CFG_TRIGGER_LEVEL_UPPER_BOUND,        /*upperBound*/
        5,                                        /*incrementStepSingle*/
        100,                                      /*incrementStepMultiple*/
        NULL,                                     /*callback*/
        "TriggerLvl",                             /*name*/
        "mV"                                      /*unitName*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_TriggerLevel = {
        OSC_Settings_TypeInfo_IntegerContinuous,
        (void*)&OSC_Settings_TriggerLevel
    };

/*============================================ TRIGGER SLOPE ==========================================*/
    char* OSC_Settings_TriggerSlope_statusNames[] = {OSC_FONT_CODE_EDGE_RISING,OSC_FONT_CODE_EDGE_FALLING};

    OSC_Settings_OnOff_Type OSC_Settings_TriggerSlope = {
        OSC_CFG_TRIGGER_SLOPE_RISING,                       /*status*/
        NULL,                                               /*callback*/
        (char** const)
        OSC_Settings_TriggerSlope_statusNames,              /*statusNames*/
        "TriggerSlope"                                      /*name*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_TriggerSlope = {
        OSC_Settings_TypeInfo_OnOff,
        (void*)&OSC_Settings_TriggerSlope
    };

/*======================================== VERTICAL RESOLUTION ========================================*/
    OSC_Settings_IntegerDiscreteValue_Type OSC_Settings_VerticalResolution_valueSet[] = {500,1000,1500,2000,2500};
    char* OSC_Settings_VerticalResolution_nameOfValues[] = {"0.5V/dv","1V/dv","1.5V/dv","2V/dv","2.5V/dv"};

    OSC_Settings_IntegerDiscrete_Type OSC_Settings_VerticalResolution = {
        (OSC_Settings_IntegerDiscreteValue_Type*)
        OSC_Settings_VerticalResolution_valueSet,           /*valueSet*/
        2,                                                  /*currentIndex*/
        5,                                                  /*length*/
        NULL,                                               /*callback*/
        (char** const)
        OSC_Settings_VerticalResolution_nameOfValues,       /*nameOfValues*/
        "VerticalRes"                                       /*name*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_VerticalResolution = {
        OSC_settings_TypeInfo_IntegerDiscrete,
        (void*)&OSC_Settings_VerticalResolution
    };

/*=========================================== VERTICAL OFFSET =========================================*/
    OSC_Settings_IntegerContinuous_Type OSC_Settings_VerticalOffset = {
        0,                    /*value*/
       -64,                   /*lowerBound*/
        64,                   /*upperBound*/
        1,                    /*incrementStepSingle*/
        8,                    /*incrementStepMultiple*/
        NULL,                 /*callback*/
        "VertOffset",         /*name*/
        "p"                   /*unitName*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_VerticalOffset = {
        OSC_Settings_TypeInfo_IntegerContinuous,
        (void*)&OSC_Settings_VerticalOffset
    };

/*########################################## VERTICAL MENU ############################################*/

    OSC_Menu_Element_Type* OSC_Menu_ElementList_VerticalMenu[] = {
        &OSC_Menu_Element_TriggerLevel,
        &OSC_Menu_Element_TriggerSlope,
        &OSC_Menu_Element_VerticalResolution,
        &OSC_Menu_Element_VerticalOffset
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

    OSC_Settings_OnOff_Type OSC_Settings_ChannelSelect = {
        OSC_CFG_CHANNEL_SELECT_CHANNEL_A_SELECTED,         /*status*/
        NULL,                                              /*callback*/
        (char** const)
        OSC_Settings_ChannelSelect_statusNames,            /*statusNames*/
        "ChnSelect"                                        /*name*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_ChannelSelect = {
        OSC_Settings_TypeInfo_OnOff,
        (void*)&OSC_Settings_ChannelSelect
    };

/*=========================================== TRIGGER SOURCE ==========================================*/
    char* OSC_Settings_TriggerSource_statusNames[] = {"CHN_A","CHN_B"};

    OSC_Settings_OnOff_Type OSC_Settings_TriggerSource = {
        OSC_CFG_TRIGGER_SOURCE_CHANNEL_A,                  /*status*/
        NULL,                                              /*callback*/
        (char** const)
        OSC_Settings_TriggerSource_statusNames,            /*statusNames*/
        "TriggerSrc"                                       /*name*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_TriggerSource = {
        OSC_Settings_TypeInfo_OnOff,
        (void*)&OSC_Settings_TriggerSource
    };

/*========================================== CHANNEL A STATUS =========================================*/
    char* OSC_Settings_Channel_A_Status_statusNames[] = {OSC_FONT_CODE_RADIOBUTTON_OFF,OSC_FONT_CODE_RADIOBUTTON_ON};

    OSC_Settings_OnOff_Type OSC_Settings_Channel_A_Status = {
        OSC_CFG_CHANNEL_A_STATUS_DISABLED,                 /*status*/
        NULL,                                              /*callback*/
        (char** const)
        OSC_Settings_Channel_A_Status_statusNames,         /*statusNames*/
        "Chn A Status"                                     /*name*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_Channel_A_Status = {
        OSC_Settings_TypeInfo_OnOff,
        (void*)&OSC_Settings_Channel_A_Status
    };

/*========================================== CHANNEL B STATUS =========================================*/
    char* OSC_Settings_Channel_B_Status_statusNames[] = {OSC_FONT_CODE_RADIOBUTTON_OFF,OSC_FONT_CODE_RADIOBUTTON_ON};

    OSC_Settings_OnOff_Type OSC_Settings_Channel_B_Status = {
        OSC_CFG_CHANNEL_B_STATUS_DISABLED,                 /*status*/
        NULL,                                              /*callback*/
        (char** const)
        OSC_Settings_Channel_B_Status_statusNames,         /*statusNames*/
        "Chn B Status"                                     /*name*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_Channel_B_Status = {
        OSC_Settings_TypeInfo_OnOff,
        (void*)&OSC_Settings_Channel_B_Status
    };

/*========================================= DATA ACQUISITION MODE =====================================*/
    char* OSC_Settings_DataAcquisitionMode_statusNames[] = {"Single","Repet"};

    OSC_Settings_OnOff_Type OSC_Settings_DataAcquisitionMode = {
        OSC_CFG_DATA_ACQUISITION_MODE_SINGLE,              /*status*/
        NULL,                                              /*callback*/
        (char** const)
        OSC_Settings_DataAcquisitionMode_statusNames,      /*statusNames*/
        "DataAcqMode"                                      /*name*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_DataAcquisitionMode = {
        OSC_Settings_TypeInfo_OnOff,
        (void*)&OSC_Settings_DataAcquisitionMode
    };

/*========================================== DATA PROCESSING MODE =====================================*/
    char* OSC_Settings_DataProcessingMode_optionNames[] = {"Normal","Avg","Peak"};

    OSC_Settings_Option_Type OSC_Settings_DataProcessingMode = {
        OSC_CFG_DATA_PROCESSING_MODE_NORMAL,          /*optionID*/
        OSC_CFG_DATA_PROCESSING_MODE_COUNT,           /*optionCount*/
        NULL,                                         /*callback*/
        (char** const)
        OSC_Settings_DataProcessingMode_optionNames,  /*nameOfOptions*/
        "DataProcMode"                                /*name*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_DataProcessingMode = {
        OSC_Settings_TypeInfo_Option,
        (void*)&OSC_Settings_DataProcessingMode
    };

/*############################################ MODE MENU ##############################################*/

    OSC_Menu_Element_Type* OSC_Menu_ElementList_ModeMenu[] = {
        &OSC_Menu_Element_ChannelSelect,
        &OSC_Menu_Element_TriggerSource,
        &OSC_Menu_Element_Channel_A_Status,
        &OSC_Menu_Element_Channel_B_Status,
        &OSC_Menu_Element_DataAcquisitionMode,
        &OSC_Menu_Element_DataProcessingMode
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

    OSC_Settings_IntegerContinuous_Type OSC_Settings_BacklightIntensity = {
        100,                                                    /*value*/
        0,                                                      /*lowerBound*/
        100,                                                    /*upperBound*/
        1,                                                      /*incrementStepSingle*/
        10,                                                     /*incrementStepMultiple*/
        OSC_Settings_IntegerContinuous_BacklightIntensity,      /*callback*/
        "BacklightInt",                                         /*name*/
        "%"                                                     /*unitName*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_BacklightIntensity = {
        OSC_Settings_TypeInfo_IntegerContinuous,
        (void*)&OSC_Settings_BacklightIntensity
    };

/*=========================================== VOLTAGE PER LSB =========================================*/

    OSC_Settings_IntegerContinuous_Type OSC_Settings_VoltagePerLSB = {
        57,                                                     /*value*/
        0,                                                      /*lowerBound*/
        1000,                                                   /*upperBound*/
        1,                                                      /*incrementStepSingle*/
        10,                                                     /*incrementStepMultiple*/
        NULL,                                                   /*callback*/
        "Voltage/LSB",                                          /*name*/
        "mV"                                                    /*unitName*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_VoltagePerLSB = {
        OSC_Settings_TypeInfo_IntegerContinuous,
        (void*)&OSC_Settings_VoltagePerLSB
    };

/*============================================= SAMPLE RATE ===========================================*/
    OSC_Settings_IntegerDiscreteValue_Type OSC_Settings_SampleRate_valueSet[] = {500,1000};   /*sample/ms*/
    char* OSC_Settings_SampleRate_nameOfValues[] = {"0,5MS/s","1MS/s"};

    OSC_Settings_IntegerDiscrete_Type OSC_Settings_SampleRate = {
        (OSC_Settings_IntegerDiscreteValue_Type*)
        OSC_Settings_SampleRate_valueSet,                   /*valueSet*/
        1,                                                  /*currentIndex*/
        2,                                                  /*length*/
        NULL,                                               /*callback*/
        (char** const)
        OSC_Settings_SampleRate_nameOfValues,               /*nameOfValues*/
        "SampleRate"                                        /*name*/
    };

    OSC_Menu_Element_Type OSC_Menu_Element_SampleRate = {
        OSC_settings_TypeInfo_IntegerDiscrete,
        (void*)&OSC_Settings_SampleRate
    };

/*########################################## HARDWARE MENU ############################################*/

    OSC_Menu_Element_Type* OSC_Menu_ElementList_HardwareMenu[] = {
        &OSC_Menu_Element_BacklightIntensity,
        &OSC_Menu_Element_VoltagePerLSB,
        &OSC_Menu_Element_SampleRate
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
