
/** User Input Button api */

#ifndef _UIB_H_
#define _UIB_H_

#include <stdint.h>

#define RANGE5_PRESSED  UP
#define RANGE4_PRESSED  CENTER
#define RANGE3_PRESSED  LEFT
#define RANGE2_PRESSED  DOWN
#define RANGE1_PRESSED  RIGHT


/* ADC INPUT BUTTON STATES */
enum
{
    NOT_PRESSED = 0,
    DOWN,
    RIGHT,
    UP,
    LEFT,
    CENTER
} buttonStates;

/* Public variables */
extern uint8_t UIB_buttonPressed;
extern uint8_t UIB_buttonChanged;

/* Public functions */
void UIB_Init(void);
void UIB_Task(void);

#endif /* _UIB_H_*/
