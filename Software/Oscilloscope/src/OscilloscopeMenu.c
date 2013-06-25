#include "OscilloscopeMenu.h"

static char OSC_Menu_OnScreenStringList[OSC_MENU_SCREEN_LINE_COUNT][OSC_MENU_TEXT_CHARACTER_BUFFER_SIZE];
static uint32_t OSC_Menu_OnScreenStringList_FirstElement = 1;
static OSC_Menu_ElementIndex_Type OSC_Menu_Element_Selected = 0;
static OSC_Menu_ElementIndex_Type OSC_Menu_Element_FirstOnScreen = 0;

static OSC_Menu_Err_Type  OSC_Menu_AssembleStringList(OSC_Menu_Name_Type menuName){
  OSC_Menu_ElementIndex_Type index;
  OSC_Menu_OnScreenStringList_FirstElement = 1;
  uint32_t position;
  /*This variable determines what is in the first menu element line on the display*/

  strncpy(OSC_Menu_OnScreenStringList[OSC_MENU_NAME_LINE],OSC_MenuList[menuName]->name,OSC_MENU_TEXT_CHARACTER_BUFFER_SIZE);
  OSC_Menu_OnScreenStringList[OSC_MENU_NAME_LINE][OSC_MENU_TEXT_CHARACTER_BUFFER_SIZE - 1] = '\0';   /*Null character is omitted if the string is too long*/

  for (index = 0;
      (index < OSC_MenuList[menuName]->length) &&
      (index < (OSC_MENU_SCREEN_LINE_COUNT - OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY));
      /* "- OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY" because the first line is the menu name*/
      ++index)
  {
    position = index + OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY;
    OSC_Settings_StringBuilder(OSC_MenuList[menuName]->menuElementList[index],
                           OSC_Menu_OnScreenStringList[position]);
    /* "+ OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY" because the first line is the menu name*/
  }

  for(index = index + 1;index < OSC_MENU_SCREEN_LINE_COUNT; ++index) OSC_Menu_OnScreenStringList[index][0] = '\0';

  return OSC_Menu_Err_OK;
}

static OSC_Menu_Err_Type  OSC_Menu_PrintAll(OSC_Menu_PrintMenuName_Type printMenuName){
  OSC_DisplayManager_LineNumber_Type line;
  OSC_DisplayManager_LineNumber_Type lineSelected;

  lineSelected = OSC_Menu_Element_Selected - OSC_Menu_Element_FirstOnScreen + OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY;

  if(printMenuName == OSC_MENU_PRINT_MENU_NAME_TRUE){
    OSC_DisplayManager_Text_PrintLine(OSC_Menu_OnScreenStringList[OSC_MENU_NAME_LINE],OSC_MENU_NAME_LINE,OSC_DisplayManager_PrintMode_Positive);
  }

  for(line = OSC_MENU_ELEMENT_FIRST_LINE; line <= (OSC_MENU_SCREEN_LINE_COUNT - OSC_Menu_OnScreenStringList_FirstElement); ++line){
    OSC_DisplayManager_Text_PrintLine(
        OSC_Menu_OnScreenStringList[OSC_Menu_OnScreenStringList_FirstElement + line - OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY],
        line,
        (lineSelected == line) ? OSC_DisplayManager_PrintMode_Negative : OSC_DisplayManager_PrintMode_Positive
    );
  }

  for(; line < OSC_MENU_SCREEN_LINE_COUNT; ++line){
    OSC_DisplayManager_Text_PrintLine(
        OSC_Menu_OnScreenStringList[line - OSC_Menu_OnScreenStringList_FirstElement],
        line,
        (lineSelected == line) ? OSC_DisplayManager_PrintMode_Negative : OSC_DisplayManager_PrintMode_Positive
    );
  }
  return OSC_Menu_Err_OK;
}

static OSC_Menu_Err_Type  OSC_Menu_PrintLine(OSC_DisplayManager_LineNumber_Type line){
  OSC_DisplayManager_LineNumber_Type lineSelected = OSC_Menu_Element_Selected - OSC_Menu_Element_FirstOnScreen +
                                                    OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY;

  if(line >= OSC_MENU_SCREEN_LINE_COUNT) return OSC_Menu_Err_InvalidLineNumber;

  if(line == OSC_MENU_NAME_LINE){
    OSC_DisplayManager_Text_PrintLine(OSC_Menu_OnScreenStringList[OSC_MENU_NAME_LINE],OSC_MENU_NAME_LINE,OSC_DisplayManager_PrintMode_Positive);
  } else {
    OSC_DisplayManager_Text_PrintLine(
        OSC_Menu_OnScreenStringList[OSC_Menu_OnScreenStringList_FirstElement + line - OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY],
        line,
        (lineSelected == line) ? OSC_DisplayManager_PrintMode_Negative : OSC_DisplayManager_PrintMode_Positive
    );
  }
  return OSC_Menu_Err_OK;
}

OSC_Menu_Err_Type  OSC_Menu_Display(OSC_Menu_Name_Type menuName,OSC_Settings_Event_Type menuEvent){
  uint32_t position;

  switch(menuEvent){
    case OSC_Settings_Event_Open:
      OSC_Menu_Element_Selected       = 0;
      OSC_Menu_Element_FirstOnScreen  = 0;
      OSC_Menu_AssembleStringList(menuName);
      OSC_Menu_PrintAll(OSC_MENU_PRINT_MENU_NAME_TRUE);
      break;
    case OSC_Settings_Event_Next:
      if((OSC_Menu_Element_Selected + 1) < OSC_MenuList[menuName]->length){
        if((OSC_Menu_Element_Selected + 1) < OSC_Menu_Element_FirstOnScreen + OSC_MENU_ELEMENTS_ON_DISPLAY_COUNT){
          OSC_Menu_Element_Selected++;
          OSC_Menu_PrintLine(OSC_Menu_Element_Selected - OSC_Menu_Element_FirstOnScreen + OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY);
          OSC_Menu_PrintLine(OSC_Menu_Element_Selected - OSC_Menu_Element_FirstOnScreen + OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY - 1);
        } else {
          OSC_Menu_Element_Selected++;
          OSC_Menu_Element_FirstOnScreen++;

          position = OSC_Menu_OnScreenStringList_FirstElement;
          OSC_Settings_StringBuilder(OSC_MenuList[menuName]->menuElementList[OSC_Menu_Element_Selected],OSC_Menu_OnScreenStringList[position]);
          OSC_Menu_OnScreenStringList_FirstElement = ( (OSC_Menu_OnScreenStringList_FirstElement + 1) == OSC_MENU_SCREEN_LINE_COUNT) ?
                                                       (OSC_MENU_ELEMENT_FIRST_LINE) :
                                                       (OSC_Menu_OnScreenStringList_FirstElement + 1);
          OSC_Menu_PrintAll(OSC_MENU_PRINT_MENU_NAME_FALSE);
        }
      }
      break;
    case OSC_Settings_Event_Previous:
      if(OSC_Menu_Element_Selected > 0){
        if(OSC_Menu_Element_Selected > OSC_Menu_Element_FirstOnScreen){
          OSC_Menu_Element_Selected--;
          OSC_Menu_PrintLine(OSC_Menu_Element_Selected - OSC_Menu_Element_FirstOnScreen + OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY);
          OSC_Menu_PrintLine(OSC_Menu_Element_Selected - OSC_Menu_Element_FirstOnScreen + OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY + 1);
        } else {
          OSC_Menu_Element_Selected--;
          OSC_Menu_Element_FirstOnScreen--;
          OSC_Menu_OnScreenStringList_FirstElement = (OSC_Menu_OnScreenStringList_FirstElement == OSC_MENU_ELEMENT_FIRST_LINE) ?
                                                     (OSC_MENU_SCREEN_LINE_COUNT - 1) :
                                                     (OSC_Menu_OnScreenStringList_FirstElement - 1);

          position = OSC_Menu_OnScreenStringList_FirstElement;
          OSC_Settings_StringBuilder(OSC_MenuList[menuName]->menuElementList[OSC_Menu_Element_Selected],OSC_Menu_OnScreenStringList[position]);
          OSC_Menu_PrintAll(OSC_MENU_PRINT_MENU_NAME_FALSE);
        }
      }
      break;
    case OSC_Settings_Event_StepUpSingle:
    case OSC_Settings_Event_StepUpMultiple:
    case OSC_Settings_Event_StepDownSingle:
    case OSC_Settings_Event_StepDownMultiple:
      OSC_Settings_Modify(OSC_MenuList[menuName]->menuElementList[OSC_Menu_Element_Selected],menuEvent);

      position = ((OSC_Menu_OnScreenStringList_FirstElement + OSC_Menu_Element_Selected - OSC_Menu_Element_FirstOnScreen) >= OSC_MENU_SCREEN_LINE_COUNT) ?
                  (OSC_Menu_Element_Selected - OSC_Menu_Element_FirstOnScreen) - (OSC_MENU_SCREEN_LINE_COUNT - OSC_Menu_OnScreenStringList_FirstElement) +
                   OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY :
                   OSC_Menu_OnScreenStringList_FirstElement + OSC_Menu_Element_Selected - OSC_Menu_Element_FirstOnScreen;
      OSC_Settings_StringBuilder(OSC_MenuList[menuName]->menuElementList[OSC_Menu_Element_Selected],OSC_Menu_OnScreenStringList[position]);
      OSC_Menu_PrintLine(OSC_Menu_Element_Selected - OSC_Menu_Element_FirstOnScreen + OSC_MENU_ELEMENT_LINE_OFFSET_ON_DISPLAY);
      break;



  }
  return OSC_Menu_Err_OK;
}

