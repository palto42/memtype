
#include "uib.h"
#include "adm.h"

/** User Input Interface Component */

#define NUM_OF_BUT  (5u)
#define ADC_SPAN    (ADC_MAX/(NUM_OF_BUT))

#define RANGE1      ((1*ADC_SPAN)-(ADC_SPAN/2))
#define RANGE2      ((2*ADC_SPAN)-(ADC_SPAN/2))
#define RANGE3      ((3*ADC_SPAN)-(ADC_SPAN/2))
#define RANGE4      ((4*ADC_SPAN)-(ADC_SPAN/2))
#define RANGE5      ((5*ADC_SPAN)-(ADC_SPAN/2))

/* Button Debounce Time */
#define uib_INIT_DEBOUNCE   (5u)
#define uib_SECOND_DEBOUNCE (100u)
#define uib_REPEAT_DEBOUNCE (25u) /* debounce time for repeated pushes */

enum UIB_states {
    INIT_DEBOUNCE = 0u,SECOND_DEBOUNCE,REPEAT_DEBOUNCE,
};

/* Debug Code */
#define DBG 0
#if DBG
#include "print.h"
#define PRINT(x)    printStr((void*)x, RAM);
#else /* if DBG */
#define PRINT(x)
#endif

/* public variable */
uint8_t UIB_buttonPressed = NOT_PRESSED;
uint8_t UIB_buttonChanged = 0u;
static uint8_t uib_lastButtonPressed;
static uint16_t uib_debounceCtr = 0u;
static uint8_t uib_state = INIT_DEBOUNCE;

void UIB_Task(void);

static void uib_buttonChanged(void){
    UIB_buttonChanged = 1u;
    uib_debounceCtr = 0u;
}

void UIB_Task(void){
    uib_lastButtonPressed = UIB_buttonPressed;

    /* The if comparison goes from highest ADC value to the lowest one*/
    if(ADM_GetAdcValue() > RANGE5)
    {
        UIB_buttonPressed = RANGE5_PRESSED;
    }
    else if(ADM_GetAdcValue() > RANGE4 )
    {
        UIB_buttonPressed = RANGE4_PRESSED;
    }
    else if(ADM_GetAdcValue() > RANGE3 )
    {
        UIB_buttonPressed = RANGE3_PRESSED;
    }
    else if(ADM_GetAdcValue() > RANGE2)
    {
        UIB_buttonPressed = RANGE2_PRESSED;
    }
    else if(ADM_GetAdcValue() > RANGE1)
    {
        UIB_buttonPressed = RANGE1_PRESSED;
    }
    else
    {
        UIB_buttonPressed = NOT_PRESSED;
    }

    if(uib_lastButtonPressed == UIB_buttonPressed)
    {
        uib_debounceCtr++;
    }
    else
    {
        uib_state = INIT_DEBOUNCE;
        uib_debounceCtr = 0u;
    }

    // by default buttonChanged = 0
    UIB_buttonChanged = 0u;

    switch (uib_state)
    {
    case INIT_DEBOUNCE:
        if( uib_debounceCtr >= uib_INIT_DEBOUNCE )
        {
            uib_buttonChanged();
            uib_state++;
        }
        break;

    case SECOND_DEBOUNCE:
        if( uib_debounceCtr >= uib_SECOND_DEBOUNCE )
        {
            uib_buttonChanged();
            uib_state++;
        }
        break;

    case REPEAT_DEBOUNCE:
        if( uib_debounceCtr >= uib_REPEAT_DEBOUNCE )
        {
            uib_buttonChanged();
        }
        break;
    }

    #if DBG
    if(UIB_buttonChanged == 1)
    {
        switch(UIB_buttonPressed) {
        case LEFT:
            PRINT("LEFT\n");
            break;
        case UP:
            PRINT("UP\n");
            break;
        case RIGHT:
            PRINT("RIGHT\n");
            break;
        case DOWN:
            PRINT("DOWN\n");
            break;
        case CENTER:
            PRINT("CENTER\n");
            break;
        default:
            return;
        }
    }
    #endif
}
