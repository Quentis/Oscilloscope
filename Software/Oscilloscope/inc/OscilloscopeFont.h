/*
 * OscilloscopeFont.h
 *
 *  Created on: 2013.05.21.
 *      Author: Somhegyi Tamas
 */

#ifndef OSCILLOSCOPEFONT_H_
#define OSCILLOSCOPEFONT_H_

#define OSC_FONT_ELEMENT_COUNT              132
#define OSC_FONT_SIZE_HEIGHT                  7
#define OSC_FONT_SIZE_WIDTH                   6

#define OSC_FONT_CODE_RADIOBUTTON_OFF       "\001"
#define OSC_FONT_CODE_RADIOBUTTON_ON        "\002"
#define OSC_FONT_CODE_EDGE_RISING           "\003"
#define OSC_FONT_CODE_EDGE_FALLING          "\004"

uint8_t OSC_fontData[OSC_FONT_ELEMENT_COUNT][OSC_FONT_SIZE_WIDTH];

#endif /* OSCILLOSCOPEFONT_H_ */
