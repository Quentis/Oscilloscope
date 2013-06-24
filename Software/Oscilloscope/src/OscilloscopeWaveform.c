#include "OscilloscopeWaveform.h"

void OSC_Waveform_DrawDivLines(void){
  const OSC_DisplayManager_Graphics_Line_Type divLines[] = {
      {{ 3,61},{124,61}},
      {{ 3,51},{124,51}},
      {{ 3,41},{124,41}},
      {{ 3,31},{124,31}},
      {{ 3,21},{124,21}},
      {{ 3,11},{124,11}},
      {{ 3, 1},{124, 1}},
      {{123,1},{123,61}},
      {{103,1},{103,61}},
      {{ 83,1},{ 83,61}},
      {{ 63,1},{ 63,61}},
      {{ 43,1},{ 43,61}},
      {{ 23,1},{ 23,61}},
      {{  3,1},{  3,61}},
  };
  const uint32_t divLinesCount = sizeof(divLines)/sizeof(divLines[0]);
  uint32_t i;

  for (i = 0; i < divLinesCount; ++i) {
    OSC_DisplayManager_Graphics_DrawLine(&divLines[i]);
  }
}
