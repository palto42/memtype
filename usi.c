/* USI - USER INPUT state machine */
#include "usi.h"
#include "uif.h"
#include "uib.h"
#include "crd.h"
#include "ucp.h"
#include "print.h"

#define USI_PIN_RETRIES     (2u)
#define USI_PIN_LENGTH 6
/** Global Data */
// PIN: 0000 default HASH
const uint8_t LOCK_HASH[16] EEMEM = {
    0xd4,0x4f,0xb2,0x7a,0x58,0xb4,0x27,0x4a,0x21,0xe6,0x8f,0x39,0x69,0x74,0x23,0x54
};

static const char erasePin[USI_PIN_LENGTH] PROGMEM = {'9','9','9','9','9','9'};

/** Local Data */
static char userText[16];
static char userPin[USI_PIN_LENGTH];
static uint8_t userTextIndex;
static const char USI_keys[] PROGMEM = {'0','1','2','3','4','5','6','7','8','9'};
static const char PIN_str[] PROGMEM = "PIN: ";
static const char LOCKED_str[] PROGMEM = "PIN ERR";
static const char PINRESET_str[] PROGMEM = "PIN RESET to 000000";
static const uint8_t LOCK_HASH_PGM[16] PROGMEM = {
    0xd4,0x4f,0xb2,0x7a,0x58,0xb4,0x27,0x4a,0x21,0xe6,0x8f,0x39,0x69,0x74,0x23,0x54
};
static uint8_t pin_retries = USI_PIN_RETRIES;

/** Private Function declaration */
static void usi_print(void);
static void usi_previous(void);
static void usi_next(void);
static void usi_resetPin(void);
static uint8_t usi_pinCheck(char pin[USI_PIN_LENGTH]);

void USI_Init(void){
    userTextIndex = 0;
    UIF_userInputIndex = 0;
    memcpy_P((void*)userText, (void*)PIN_str, sizeof(PIN_str));
    usi_print();
    UCP_Lock();
}

// user input finitestate machine
void USI_fsm(uint8_t button){
    switch(button) {
    case LEFT:
        print_deleteStr();
        USI_Init();
        break;
    case UP:
        print_deleteStr();
        usi_next();
        break;
    case RIGHT:
        print_deleteStr();
        if(userTextIndex == (sizeof(userPin)-1))     // real array elements (\0)
        {
            /* Device Unlocked */
            if(usi_pinCheck(userPin) == 1)
            {
                CRD_fsmStart();
            }
            /* Device Locked */
            else
            {


                if(pin_retries > 0){
                    printStr((void*)LOCKED_str,FLASH);
                    pin_retries--;
                } else{
                    printStr((void*)PINRESET_str,FLASH);
                    usi_resetPin();
                    pin_retries = USI_PIN_RETRIES;
                }

                userTextIndex = 0;
                UIF_userInputIndex = 0;
                userPin[0] = '0';
                memcpy_P((void*)userText, (void*)PIN_str, sizeof(PIN_str));
            }
        }
        else
        {
            userText[sizeof(PIN_str)-1+userTextIndex] = '*';
            userTextIndex++;
            usi_print();
        }
        break;
    case DOWN:
        print_deleteStr();
        usi_previous();
        break;
    default:
        break;
    }
}

static void usi_print(void){
    userPin[userTextIndex] = pgm_read_byte(&USI_keys[UIF_userInputIndex]);
    userText[sizeof(PIN_str)-1+userTextIndex] = userPin[userTextIndex];
    userText[sizeof(PIN_str)-1+userTextIndex+1] = 0;
    printStr(userText,RAM);
}

static void usi_previous(void){
    UIF_decrement(&UIF_userInputIndex, sizeof(USI_keys));
    usi_print();
}

static void usi_next(void){
    UIF_increment(&UIF_userInputIndex, sizeof(USI_keys));
    usi_print();
}

static uint8_t usi_pinCheck(char pin[4]){
    uint8_t i;


    if ( (pin[0]==erasePin[0]) && (pin[1]==erasePin[1]) && (pin[2]==erasePin[2]) && (pin[3]==erasePin[3] && (pin[4]==erasePin[4]) && (pin[5]==erasePin[5]) )
    {
	usi_resetPin();
	while (1) {};
    }

    for(i=0; i<16; i++) {
        cipher.key[i] = pin[(i%USI_PIN_LENGTH)];
        cipher.plain[i] = pin[(i%USI_PIN_LENGTH)];
    }
    noekeon_encrypt();

    for(i=0; i<16; i++)
    {
        if(eeprom_read_byte(LOCK_HASH+i) != cipher.plain[i]) {
            break;
        }
    }
    return i==16;
}

static void usi_resetPin(void){
    uint8_t hash[16];
    memcpy_P((void*)hash, (void*)LOCK_HASH_PGM, sizeof(LOCK_HASH_PGM));
    eeprom_update_block ((void*)hash, (void*)LOCK_HASH, sizeof(hash));
}
