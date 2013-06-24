/*
 * OscilloscopeNotification.h
 *
 *  Created on: 2013.06.24.
 *      Author: Somhegyi Tamas
 */

#ifndef OSCILLOSCOPENOTIFICATION_H_
#define OSCILLOSCOPENOTIFICATION_H_

#include "OscilloscopeDisplayManager.h"
#include "OscilloscopeDSP.h"
#include "OscilloscopeMenu.h"
#include "OscilloscopeConfiguration.h"

char OSC_Notification_Info[OSC_DM_TEXT_LINE_COUNT][OSC_DM_TEXT_CHARACTER_PER_LINE + 1];  /*+1 because of '\0'*/

void OSC_Notification_PrintAll(void);

#endif /* OSCILLOSCOPENOTIFICATION_H_ */
