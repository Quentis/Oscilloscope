#include "OscilloscopeDisplayManager.h"

static uint8_t   OSC_DM_DisplayMatrix[OSC_DM_MATRIX_ROW_COUNT][OSC_DM_MATRIX_COLUMN_COUNT];
static uint32_t  OSC_DM_DisplayMatrix_DirtyBits[OSC_DM_MATRIX_DIRTYBIT_ROW_COUNT][OSC_DM_MATRIX_DIRTYBIT_COLUMN_COUNT];

void OSC_DisplayManager_Init(void){
  uint8_t i,j;

  for (i = 0; i < OSC_DM_MATRIX_ROW_COUNT; ++i) {
    for (j = 0; j < OSC_DM_MATRIX_COLUMN_COUNT; ++j) {
      OSC_DM_DisplayMatrix[i][j] = 0;
    }
  }

  for (i = 0; i < OSC_DM_MATRIX_DIRTYBIT_ROW_COUNT; ++i) {
    for (j = 0; j < OSC_DM_MATRIX_DIRTYBIT_COLUMN_COUNT; ++j) {
      OSC_DM_DisplayMatrix_DirtyBits[i][j] = 0;
    }
  }

  OSC_DisplayManager_Waveform_Channel_A.dataType = OSC_DisplayManager_Waveform_DataType_Normal;
  OSC_DisplayManager_Waveform_Channel_B.dataType = OSC_DisplayManager_Waveform_DataType_Normal;
}

OSC_DisplayManager_Err_Type OSC_DisplayManager_ClearScreen(void){
  uint8_t i,j;
  for (i = 0; i < OSC_DM_MATRIX_ROW_COUNT; ++i) {
    for (j = 0; j < OSC_DM_MATRIX_COLUMN_COUNT; ++j) {
      OSC_DM_DisplayMatrix[i][j] = 0;
    }
  }
  for (i = 0; i < OSC_DM_MATRIX_DIRTYBIT_ROW_COUNT; ++i) {
    for (j = 0; j < OSC_DM_MATRIX_DIRTYBIT_COLUMN_COUNT; ++j) {
      OSC_DM_DisplayMatrix_DirtyBits[i][j] = ~0;
    }
  }
  return OSC_DisplayManager_Err_OK;
}

OSC_DisplayManager_Err_Type OSC_DisplayManager_Graphics_UpdateWaveform(OSC_DisplayManager_Waveform_Type* waveform){
  uint32_t index;
  for (index = 0; index < OSC_DM_MATRIX_COLUMN_COUNT; ++index) {
    OSC_DM_MATRIX_SET_PIXEL(index,waveform->dataPoints[0][index]);
    if(waveform->dataType == OSC_DisplayManager_Waveform_DataType_MinMax){
      OSC_DM_MATRIX_SET_PIXEL(index,waveform->dataPoints[1][index]);
    }
  }
 return OSC_DisplayManager_Err_OK;
}

OSC_DisplayManager_Err_Type OSC_DisplayManager_Text_PrintChar(char ch,OSC_DisplayManager_LineNumber_Type line, OSC_DisplayManager_CharPosition_Type pos){
  uint8_t j;
  uint8_t columnIndex = pos * OSC_DM_TEXT_FONT_SIZE_WIDTH + 1;
  uint8_t rowIndex = OSC_DM_MATRIX_LINE_TO_ROW(line);
  if(line >= OSC_DM_MATRIX_ROW_COUNT) return OSC_DisplayManager_Err_InvalidParameter_Text_Line;
  if(pos >= OSC_DM_TEXT_CHARACTER_PER_LINE) return OSC_DisplayManager_Err_InvalidParameter_Text_CharPosition;

  for(j = 0; j < OSC_DM_TEXT_FONT_SIZE_WIDTH; j++){
    OSC_DM_DisplayMatrix[rowIndex][columnIndex + j] = OSC_fontData[(uint8_t)ch][j];
  }

  OSC_DM_DisplayMatrix_DirtyBits[rowIndex][columnIndex / OSC_DM_MATRIX_DIRTYBIT_COLUMN_BIT_COUNT] |=
      OSC_DM_TEXT_FONT_SIZE_DIRTYMASK << (columnIndex % OSC_DM_MATRIX_DIRTYBIT_COLUMN_BIT_COUNT);
  if(((columnIndex % OSC_DM_MATRIX_DIRTYBIT_COLUMN_BIT_COUNT) + OSC_DM_TEXT_FONT_SIZE_WIDTH) >= OSC_DM_MATRIX_DIRTYBIT_COLUMN_BIT_COUNT){
    OSC_DM_DisplayMatrix_DirtyBits[rowIndex][columnIndex / OSC_DM_MATRIX_DIRTYBIT_COLUMN_BIT_COUNT + 1] |=
          OSC_DM_TEXT_FONT_SIZE_DIRTYMASK >> (OSC_DM_TEXT_FONT_SIZE_WIDTH - ((columnIndex + OSC_DM_TEXT_FONT_SIZE_WIDTH) % OSC_DM_MATRIX_DIRTYBIT_COLUMN_BIT_COUNT));
  }

  return OSC_DisplayManager_Err_OK;
}

OSC_DisplayManager_Err_Type OSC_DisplayManager_Text_PrintLine(char*                                   str,
                                                              OSC_DisplayManager_LineNumber_Type      line,
                                                              OSC_DisplayManager_PrintMode_Type       printMode){
  uint8_t i = 0,j = 0;
  uint8_t rowIndex = OSC_DM_MATRIX_LINE_TO_ROW(line);
  uint8_t printModeMask = (printMode == OSC_DisplayManager_PrintMode_Negative) ? 0xFF : 0x00;

  if(line >= OSC_DM_MATRIX_ROW_COUNT) return OSC_DisplayManager_Err_InvalidParameter_Text_Line;
  if(str == 0) return OSC_DisplayManager_Err_InvalidParameter_Text_StrNullPtr;

  OSC_DM_DisplayMatrix[rowIndex][0] = printModeMask ^ 0;    /*First column is empty because it looks better in negative mode*/

  for(i = 0; str[i] != '\0' && i < OSC_DM_TEXT_CHARACTER_PER_LINE; i++){
    uint8_t charPos = OSC_DM_TEXT_FONT_SIZE_WIDTH * i + 1;    /*+1 because it looks better in negative mode*/
    for(j = 0; j < OSC_DM_TEXT_FONT_SIZE_WIDTH; j++){
      OSC_DM_DisplayMatrix[rowIndex][charPos + j] = printModeMask ^ OSC_fontData[(uint8_t)(str[i])][j];
    }
  }

  for(j = i * OSC_DM_TEXT_FONT_SIZE_WIDTH + 1;j < OSC_DM_MATRIX_COLUMN_COUNT; j++){   /*+1 because it looks better in negative mode*/
    OSC_DM_DisplayMatrix[rowIndex][j] = printModeMask ^ 0;
  }

  for(j = 0; j < OSC_DM_MATRIX_DIRTYBIT_COLUMN_COUNT; j++) OSC_DM_DisplayMatrix_DirtyBits[rowIndex][j] = 0xFFFFFFFF;  /*whole line is dirty*/
  return OSC_DisplayManager_Err_OK;
}

OSC_DisplayManager_Err_Type OSC_DisplayManager_Graphics_DrawLine(const OSC_DisplayManager_Graphics_Line_Type* lineCoords){
  int32_t x1,x2,dx,y1,y2,dy,gradient;
  if( ((lineCoords->pointFirst.x == lineCoords->pointSecond.x) && (lineCoords->pointFirst.y == lineCoords->pointSecond.y)) ||
       (lineCoords->pointFirst.x  <  0) || (lineCoords->pointSecond.x <   0) || (lineCoords->pointFirst.y  <   0) ||
       (lineCoords->pointSecond.y <  0) || (lineCoords->pointFirst.x  > 127) || (lineCoords->pointSecond.x > 127) ||
       (lineCoords->pointFirst.y  > 63) || (lineCoords->pointSecond.y >  63)){
    return OSC_DisplayManager_Err_InvalidParameter_Graphics_LinePoints;
  }

  dx = OSC_DM_ABS(lineCoords->pointFirst.x - lineCoords->pointSecond.x);
  dy = OSC_DM_ABS(lineCoords->pointFirst.y - lineCoords->pointSecond.y);

  if(dy < dx){
    if(lineCoords->pointFirst.x < lineCoords->pointSecond.x){
      x1 = lineCoords->pointFirst.x;
      y1 = lineCoords->pointFirst.y;
      x2 = lineCoords->pointSecond.x;
      y2 = lineCoords->pointSecond.y;
    } else {
      x1 = lineCoords->pointSecond.x;
      y1 = lineCoords->pointSecond.y;
      x2 = lineCoords->pointFirst.x;
      y2 = lineCoords->pointFirst.y;
    }
    dx = x2 - x1;
    dy = y2 - y1;
    gradient = (dy << 16)/dx;
    for(dx = 0; x1 + dx < x2; dx++){
      int32_t pixel_y = ((gradient * dx) >> 16) + y1;
      OSC_DM_MATRIX_SET_PIXEL(x1 + dx,pixel_y);
    }
  } else {
    if(lineCoords->pointFirst.y < lineCoords->pointSecond.y) {
      y1 = lineCoords->pointFirst.y;
      x1 = lineCoords->pointFirst.x;
      y2 = lineCoords->pointSecond.y;
      x2 = lineCoords->pointSecond.x;
    } else {
      y1 = lineCoords->pointSecond.y;
      x1 = lineCoords->pointSecond.x;
      y2 = lineCoords->pointFirst.y;
      x2 = lineCoords->pointFirst.x;
    }
    dx = x2 - x1;
    dy = y2 - y1;
    gradient = (dx << 16)/dy;
    for(dy = 0; y1 + dy < y2; dy++){
      int32_t pixel_x = (((gradient * dy) >> 16) + x1);
      OSC_DM_MATRIX_SET_PIXEL(pixel_x,y1 + dy);
    }
  }
  return OSC_DisplayManager_Err_OK;
}
/*=======================================DISPLAY_MANAGER_SCREEN_UPDATER=======================================*/
/*
 * This function should run behind the scenes: it automatically searches for changes (dirty bits)
 * and performs the update of the dirty bytes and clears the corresponding dirty bits after an update event.
 *
 * The update rate is determined by the frequency of the calls of this function and the definition
 * of maximum update count per function invocation
 * The updater call rate should be in 10ms order of magnitude with an update count of 128-256 (1-2 rows)
 *
 * It is important that the function will continue the updating from the last location of the
 * previous invocation so this way the screen update rate and density will be balanced and the CPU load
 * will be minimal
 */

void OSC_DisplayManager_ScreenUpdater(void){
  static uint8_t rowIndex = 0,wordIndex = 0,bitPosIndex = 0;
  static uint8_t adjacentDirtyBytes = 0,firstBytePos = 0; /*These variables needs to be static in order to detect the BufferFull event*/
  uint8_t rowIndex_start = rowIndex;
  uint32_t updateCount = OSC_DM_SCREEN_UPDATER_UPDATECOUNT_PER_INVOCATION;
  OSC_LCD_Err_Type retVal;

  if(adjacentDirtyBytes != 0){ /*This can happen if the last time the buffer of the transport layer was full -> Retry*/
    retVal = OSC_LCD_TransportLine_Start(rowIndex,firstBytePos,&OSC_DM_DisplayMatrix[rowIndex][firstBytePos],adjacentDirtyBytes);
    if(retVal == OSC_LCD_Err_BufferFull) return;    /*There is no point to try further transport action while the buffer is full*/
    rowIndex = (rowIndex + 1) % OSC_DM_MATRIX_DIRTYBIT_ROW_COUNT;
    /*The row scan has been finished independently from Transport but because of the return, the rowindex must be updated here*/
    adjacentDirtyBytes = 0;   /*This represents the fact that the last transfer was successful*/
  }

  do{
    for(;wordIndex < OSC_DM_MATRIX_DIRTYBIT_COLUMN_COUNT;wordIndex++){  /*It continues where it has ended*/
      if(OSC_DM_DisplayMatrix_DirtyBits[rowIndex][wordIndex] != 0){  /*If the value of the word is 0 then there isn't any dirty bytes*/
        for(;bitPosIndex < OSC_DM_MATRIX_DIRTYBIT_COLUMN_BIT_COUNT;bitPosIndex++){  /*It continues where it has ended*/
          if(OSC_DM_DisplayMatrix_DirtyBits[rowIndex][wordIndex] & (1 << bitPosIndex)){   /*Byte is dirty*/
            if(adjacentDirtyBytes == 0) firstBytePos = wordIndex * OSC_DM_MATRIX_DIRTYBIT_COLUMN_BIT_COUNT + bitPosIndex;
            adjacentDirtyBytes++;
            OSC_DM_DisplayMatrix_DirtyBits[rowIndex][wordIndex] &= ~(1 << bitPosIndex);
            updateCount--;
            /*
             * However the dirty bit is cleared it means only that the display manager has scheduled the byte for the transport layer
             * If the buffer of the transport layer is full then these bytes aren't updated on the display for a while
             */
            if(updateCount == 0){
              bitPosIndex++;  /*On the next invocation it is needless to check the same byte for dirtiness*/
              retVal = OSC_LCD_TransportLine_Start(rowIndex,firstBytePos,&OSC_DM_DisplayMatrix[rowIndex][firstBytePos],adjacentDirtyBytes);
              /*
               * If the update count has been reached then the display manager schedules a transport packet independently of the other possible
               * adjacent dirty bytes -> This way the CPU load will be balanced (better response time for other tasks)
               * (update count is measured in dirty bytes)
               */
              if(retVal == OSC_LCD_Err_BufferFull) return;
              adjacentDirtyBytes = 0;
              return;
            }
          } else {  /*Byte is not dirty*/
            if(adjacentDirtyBytes > 0){
              retVal = OSC_LCD_TransportLine_Start(rowIndex,firstBytePos,&OSC_DM_DisplayMatrix[rowIndex][firstBytePos],adjacentDirtyBytes);
              /*
               * Only adjacent bytes on the display can be sent in one packet so if the display manager finds the last of the dirty adjacent bytes
               * then it schedules a transport packet
               */
              if(retVal == OSC_LCD_Err_BufferFull) return;
              adjacentDirtyBytes = 0;   /*firstBytePos doesn't present in any condition so it is not important to null it*/
            }
            /*If adjacentDirtyBytes == 0 then nothing to do because there weren't any dirty bytes*/
          }
        } /*End of word*/
      }
      bitPosIndex = 0;
    } /*End of row*/
    wordIndex = 0;

    if(adjacentDirtyBytes > 0){
      retVal = OSC_LCD_TransportLine_Start(rowIndex,firstBytePos,&OSC_DM_DisplayMatrix[rowIndex][firstBytePos],adjacentDirtyBytes);
      if(retVal == OSC_LCD_Err_BufferFull){
        return;
      }
      adjacentDirtyBytes = 0;   /*End of row (or line) and the OSC_LCD_TransportLine_Start(...) function can't be called on multiple lines*/
    }
    rowIndex = (rowIndex + 1) % OSC_DM_MATRIX_DIRTYBIT_ROW_COUNT;
  } while(rowIndex != rowIndex_start);
  /*(rowIndex == rowIndex_start) ==> No need for further scanning - Everything is up to date*/
  /*At the end of scan there is no need for transport because the end of row event guarantees that*/
}
