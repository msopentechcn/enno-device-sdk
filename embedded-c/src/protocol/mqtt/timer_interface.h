/* All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Wei Zhaoyang - initial API and implementation and/or initial documentation
 *******************************************************************************/
#ifndef _TIMER_INTERFACE_H
#define _TIMER_INTERFACE_H


typedef struct Timer Timer;

//Add the platform specfic timer includes to the Timer struct
#include "timer_linux.h"

char expired(Timer *);

void countdown_ms(Timer *, unsigned int);

void countdown(Timer *, unsigned int);

int left_ms(Timer *);

void InitTimer(Timer *);

#endif


