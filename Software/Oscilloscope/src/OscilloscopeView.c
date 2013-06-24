#include "OscilloscopeView.h"

OSC_View_State_Type OSC_View_State = {
    OSC_View_DisplayController_Waveform,
    OSC_Menu_Name_None
};

OSC_View_EventFifoIndex_Type  OSC_View_EventFifoIndex_FirstFree = 0;
OSC_View_EventFifoIndex_Type  OSC_View_EventFifoIndex_FirstIn   = 0;

OSC_View_Err_Type     OSC_View_Event_RegisterEvent(OSC_View_Event_Type event){
  if(((OSC_View_EventFifoIndex_FirstFree + 1) != OSC_VIEW_EVENTFIFO_SIZE ? OSC_View_EventFifoIndex_FirstFree + 1 : 0) != OSC_View_EventFifoIndex_FirstIn){
    OSC_View_EventFifo[OSC_View_EventFifoIndex_FirstFree] = event;
    OSC_View_EventFifoIndex_FirstFree = (OSC_View_EventFifoIndex_FirstFree + 1) != OSC_VIEW_EVENTFIFO_SIZE ? OSC_View_EventFifoIndex_FirstFree + 1 : 0;
    return OSC_View_Err_OK;
  } else {
    return OSC_View_Err_FifoFull;
  }
}

OSC_View_Event_Type   OSC_View_Event_GetEvent(void){
  OSC_View_Event_Type retVal;
  if(OSC_View_EventFifoIndex_FirstIn == OSC_View_EventFifoIndex_FirstFree){
    return OSC_View_Event_EmptyEvent;
  } else {
    retVal = OSC_View_EventFifo[OSC_View_EventFifoIndex_FirstIn];
    OSC_View_EventFifoIndex_FirstIn = (OSC_View_EventFifoIndex_FirstIn + 1) != OSC_VIEW_EVENTFIFO_SIZE ? OSC_View_EventFifoIndex_FirstIn + 1 : 0;
    return retVal;
  }
}

static void OSC_View_MenuOpen(OSC_View_Event_Type event){
  OSC_View_State.displayController = OSC_View_DisplayController_Menu;
  switch(event){
    case OSC_View_Event_HorizontalLeftClickDouble:
      OSC_Menu_Display(OSC_Menu_Name_HorizontalMenu,OSC_Menu_Event_Menu_Open);
      OSC_View_State.activeMenuName = OSC_Menu_Name_HorizontalMenu;
      break;
    case OSC_View_Event_VerticalUpClickDouble:
      OSC_Menu_Display(OSC_Menu_Name_VerticalMenu,OSC_Menu_Event_Menu_Open);
      OSC_View_State.activeMenuName = OSC_Menu_Name_VerticalMenu;
      break;
    case OSC_View_Event_HorizontalRightClickDouble:
      OSC_Menu_Display(OSC_Menu_Name_ModeMenu,OSC_Menu_Event_Menu_Open);
      OSC_View_State.activeMenuName = OSC_Menu_Name_ModeMenu;
      break;
    case OSC_View_Event_VerticalDownClickDouble:
      OSC_Menu_Display(OSC_Menu_Name_HardwareMenu,OSC_Menu_Event_Menu_Open);
      OSC_View_State.activeMenuName = OSC_Menu_Name_HardwareMenu;
      break;
    default:
      return;
  }
}

static void OSC_View_WaveformOpen(OSC_View_Event_Type event){
  OSC_View_State.displayController = OSC_View_DisplayController_Waveform;
  OSC_DisplayManager_ClearScreen();
  OSC_Waveform_DrawDivLines();
}

static void OSC_View_NotificationOpen(OSC_View_Event_Type event){
  OSC_View_State.displayController = OSC_View_DisplayController_Notification;
  OSC_Notification_PrintAll();
}

static void OSC_View_MenuStateMachine(OSC_View_Event_Type event){
  switch(event){
    case OSC_View_Event_HorizontalLeftClickSingle:
      OSC_Menu_Display(OSC_View_State.activeMenuName,OSC_Menu_Event_MenuElement_StepDownSingle);
      break;
    case OSC_View_Event_HorizontalLeftClickDouble:
      OSC_Menu_Display(OSC_View_State.activeMenuName,OSC_Menu_Event_MenuElement_StepDownMultiple);
      break;
    case OSC_View_Event_HorizontalRightClickSingle:
      OSC_Menu_Display(OSC_View_State.activeMenuName,OSC_Menu_Event_MenuElement_StepUpSingle);
      break;
    case OSC_View_Event_HorizontalRightClickDouble:
      OSC_Menu_Display(OSC_View_State.activeMenuName,OSC_Menu_Event_MenuElement_StepUpMultiple);
      break;
    case OSC_View_Event_VerticalUpClickSingle:
    case OSC_View_Event_VerticalUpClickDouble:
      OSC_Menu_Display(OSC_View_State.activeMenuName,OSC_Menu_Event_MenuElement_Previous);
      break;
    case OSC_View_Event_VerticalDownClickSingle:
    case OSC_View_Event_VerticalDownClickDouble:
      OSC_Menu_Display(OSC_View_State.activeMenuName,OSC_Menu_Event_MenuElement_Next);
      break;
    case OSC_View_Event_HorizontalMiddleClickSingle:
    case OSC_View_Event_HorizontalMiddleClickDouble:
    case OSC_View_Event_VerticalMiddleClickSingle:
    case OSC_View_Event_VerticalMiddleClickDouble:
      OSC_View_State.activeMenuName    = OSC_Menu_Name_None;
      OSC_View_WaveformOpen(event);
      break;
    default: return;
  }
}

static void OSC_View_WaveformStateMachine(OSC_View_Event_Type event){
  switch(event){
    case OSC_View_Event_HorizontalMiddleClickDouble:
    case OSC_View_Event_VerticalMiddleClickDouble:
      OSC_View_NotificationOpen(event);
      break;
    case OSC_View_Event_VerticalUpClickDouble:
    case OSC_View_Event_VerticalDownClickDouble:
    case OSC_View_Event_HorizontalLeftClickDouble:
    case OSC_View_Event_HorizontalRightClickDouble:
      OSC_View_MenuOpen(event);
      break;
    default:
      return;
  }
}

static void OSC_View_NotificationStateMachine(OSC_View_Event_Type event){
  switch(event){
    case OSC_View_Event_HorizontalMiddleClickDouble:
    case OSC_View_Event_VerticalMiddleClickDouble:
      OSC_View_WaveformOpen(event);
      break;
    case OSC_View_Event_VerticalUpClickDouble:
    case OSC_View_Event_VerticalDownClickDouble:
    case OSC_View_Event_HorizontalLeftClickDouble:
    case OSC_View_Event_HorizontalRightClickDouble:
      OSC_View_MenuOpen(event);
      break;
    default: return;
  }
}

void OSC_View_EventManager(void){
  OSC_View_Event_Type event = OSC_View_Event_GetEvent();

  if(event != OSC_View_Event_EmptyEvent){

    switch(OSC_View_State.displayController){
      case OSC_View_DisplayController_Waveform:
        OSC_View_WaveformStateMachine(event);
        break;
      case OSC_View_DisplayController_Notification:
        OSC_View_NotificationStateMachine(event);
        break;
      case OSC_View_DisplayController_Menu:
        OSC_View_MenuStateMachine(event);
        break;
      default:
        return;
    }

  }
}
