#ifndef OSCILLOSCOPEDISPLAYMANAGER_H_
#define OSCILLOSCOPEDISPLAYMANAGER_H_

#include "OscilloscopeLCD.h"
#include "OscilloscopeFont.h"

#define OSC_DM_MATRIX_ROW_COUNT                      8
#define OSC_DM_MATRIX_ROW_PIXEL_COUNT                8    /*A Row element is a group of 8 pixels*/
#define OSC_DM_MATRIX_COLUMN_COUNT                 128

#define OSC_DM_MATRIX_DIRTYBIT_ROW_COUNT             8
#define OSC_DM_MATRIX_DIRTYBIT_COLUMN_COUNT          4
#define OSC_DM_MATRIX_DIRTYBIT_COLUMN_BIT_COUNT     32

#define OSC_DM_TEXT_FONT_SIZE_HEIGHT                OSC_FONT_SIZE_HEIGHT
#define OSC_DM_TEXT_FONT_SIZE_WIDTH                 OSC_FONT_SIZE_WIDTH
#define OSC_DM_TEXT_FONT_SIZE_DIRTYMASK             0x3F
#define OSC_DM_TEXT_LINE_COUNT                      OSC_DM_MATRIX_ROW_COUNT
#define OSC_DM_TEXT_CHARACTER_PER_LINE             (OSC_DM_MATRIX_COLUMN_COUNT / OSC_FONT_SIZE_WIDTH)
#define OSC_DM_MATRIX_LINE_COUNT                    OSC_DM_MATRIX_ROW_COUNT
#define OSC_DM_MATRIX_LINE_TO_ROW(LINE)            (OSC_DM_MATRIX_LINE_COUNT - 1 - LINE)

#define OSC_DM_SCREEN_UPDATER_UPDATECOUNT_PER_INVOCATION    256

#define OSC_DM_MATRIX_SET_PIXEL(X,Y)    do{\
                                          OSC_DM_DisplayMatrix[(Y) / OSC_DM_MATRIX_ROW_PIXEL_COUNT][X] |= (1 << ((Y) % OSC_DM_MATRIX_ROW_PIXEL_COUNT));\
                                          OSC_DM_DisplayMatrix_DirtyBits[(Y) / OSC_DM_MATRIX_ROW_PIXEL_COUNT][(X) / OSC_DM_MATRIX_DIRTYBIT_COLUMN_BIT_COUNT]\
                                                                        |= (1 << ((X) % OSC_DM_MATRIX_DIRTYBIT_COLUMN_BIT_COUNT));\
                                        }while(0)

#define OSC_DM_MATRIX_CLEAR_PIXEL(X,Y)  do{\
                                          OSC_DM_DisplayMatrix[(Y) / OSC_DM_MATRIX_ROW_PIXEL_COUNT][X] &= ~(1 << ((Y) % OSC_DM_MATRIX_ROW_PIXEL_COUNT));\
                                          OSC_DM_DisplayMatrix_DirtyBits[(Y) / OSC_DM_MATRIX_ROW_PIXEL_COUNT][(X) / OSC_DM_MATRIX_DIRTYBIT_COLUMN_BIT_COUNT]\
                                                                        |= (1 << ((X) % OSC_DM_MATRIX_DIRTYBIT_COLUMN_BIT_COUNT));\
                                        }while(0)

#define OSC_DM_ABS(X)       ( ((X) > 0) ? (X) : (-(X)) )

typedef uint8_t OSC_DisplayManager_LineNumber_Type;
typedef uint8_t OSC_DisplayManager_CharPosition_Type;

typedef enum {
  OSC_DisplayManager_PrintMode_Positive,
  OSC_DisplayManager_PrintMode_Negative
} OSC_DisplayManager_PrintMode_Type;

typedef struct {
  int32_t x;
  int32_t y;
} OSC_DisplayManager_Graphics_Point_Type;

typedef struct {
  OSC_DisplayManager_Graphics_Point_Type  pointFirst;
  OSC_DisplayManager_Graphics_Point_Type  pointSecond;
} OSC_DisplayManager_Graphics_Line_Type;

typedef struct {
  uint8_t dataPoints[2][OSC_DM_MATRIX_ROW_COUNT];
  uint8_t actualDataPoints;
} OSC_DisplayManager_WaveformType;

OSC_DisplayManager_WaveformType OSC_Waveform_Channel_A;
OSC_DisplayManager_WaveformType OSC_Waveform_Channel_B;

/*=========================================ERROR_DEFINITIONS========================================*/

typedef enum {
  OSC_DisplayManager_Err_OK,
  OSC_DisplayManager_Err_InvalidParameter_Text_Line,
  OSC_DisplayManager_Err_InvalidParameter_Text_CharPosition,
  OSC_DisplayManager_Err_InvalidParameter_Text_StrNullPtr,
  OSC_DisplayManager_Err_InvalidParameter_Graphics_LinePoints
} OSC_DisplayManager_Err_Type;

void OSC_DisplayManager_Init(void);
OSC_DisplayManager_Err_Type OSC_DisplayManager_ClearScreen(void);
OSC_DisplayManager_Err_Type OSC_DisplayManager_Graphics_DrawLine(const OSC_DisplayManager_Graphics_Line_Type* lineCoords);
OSC_DisplayManager_Err_Type OSC_DisplayManager_Graphics_UpdateWaveform(OSC_DisplayManager_WaveformType* waveform);
OSC_DisplayManager_Err_Type OSC_DisplayManager_Text_PrintChar(char                                    ch,
                                                              OSC_DisplayManager_LineNumber_Type      line,
                                                              OSC_DisplayManager_CharPosition_Type    pos);
OSC_DisplayManager_Err_Type OSC_DisplayManager_Text_PrintLine(char*                                 str,
                                                              OSC_DisplayManager_LineNumber_Type    line,
                                                              OSC_DisplayManager_PrintMode_Type     printMode);
void OSC_DisplayManager_ScreenUpdater(void);

#endif /* OSCILLOSCOPEDISPLAYMANAGER_H_ */
