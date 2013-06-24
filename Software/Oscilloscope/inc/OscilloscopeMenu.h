#ifndef OSCILLOSCOPEMENU_H_
#define OSCILLOSCOPEMENU_H_

#include "OscilloscopeSettings.h"
#include "OscilloscopeDisplayManager.h"
#include "OscilloscopeCommonTypes.h"
#include "stdio.h"
#include "string.h"

#define OSC_MENU_SCREEN_LINE_COUNT                   OSC_DM_MATRIX_LINE_COUNT

#define OSC_MENU_NAME_LINE                           0
#define OSC_MENU_ELEMENTS_ON_DISPLAY_COUNT           7
#define OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY      1
#define OSC_MENU_ELEMENT_FIRST_LINE                  OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY
#define OSC_MENU_ELEMENT_LAST_LINE                  (OSC_MENU_ELEMENT_FIRST_LINE + OSC_MENU_ELEMENTS_ON_DISPLAY_COUNT)
#define OSC_MENU_TEXT_CHARACTER_BUFFER_SIZE         (OSC_DM_TEXT_CHARACTER_PER_LINE + 1)  /* + 1 because of '\0'*/

#define OSC_MENU_TEXT_BUFFER_INDEX_TO_LINE(bufferIndex,bufferIndexFirstLine)\
                    ( ( (bufferIndex) >= (bufferIndexFirstLine) )?\
                      ( (bufferIndex) - (bufferIndexFirstLine) + OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY):\
                      ( OSC_MENU_SCREEN_LINE_COUNT - (bufferIndexFirstLine) + (bufferIndex) ) )

#define OSC_MENU_PRINT_MENU_NAME_FALSE        0
#define OSC_MENU_PRINT_MENU_NAME_TRUE         (!OSC_MENU_PRINT_MENU_NAME_FALSE)

typedef uint8_t OSC_Menu_PrintMenuName_Type;

typedef enum {
  OSC_Menu_Err_OK,
  OSC_Menu_Err_InvalidPointer,
  OSC_Menu_Err_InvalidMenuName,
  OSC_Menu_Err_InvalidLineNumber,
  OSC_Menu_Err_InvalidType
} OSC_Menu_Err_Type;

typedef enum {
  OSC_Menu_Name_HorizontalMenu,
  OSC_Menu_Name_VerticalMenu,
  OSC_Menu_Name_ModeMenu,
  OSC_Menu_Name_HardwareMenu,
  OSC_Menu_Name_Count,
  OSC_Menu_Name_None
} OSC_Menu_Name_Type;

/*
 *                                    !!!WARNING!!!
 * Be extremely careful when you set the typeInfo because the wrong settings can
 * lead to invalid memory accesses
 */
typedef OSC_Settings_Type OSC_Menu_Element_Type;

typedef OSC_Menu_Element_Type**    OSC_Menu_ElementList_Type;
typedef uint32_t                   OSC_Menu_ElementIndex_Type;

typedef struct {
  const OSC_Menu_ElementList_Type   menuElementList;
  OSC_Menu_ElementIndex_Type        length;
  char * const                      name;
} OSC_Menu_Type;

OSC_Menu_Err_Type  OSC_Menu_Display(OSC_Menu_Name_Type menuName,OSC_Settings_Event_Type menuEvent);

extern OSC_Menu_Type* OSC_MenuList[OSC_Menu_Name_Count];

#endif /* OSCILLOSCOPEMENU_H_ */
