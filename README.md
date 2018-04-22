[![Build Status](https://travis-ci.org/jim17/memtype.svg?branch=develop)](https://travis-ci.org/jim17/memtype)
# Memtype
Open Source solution to store your passwords encrypted, having them available everywhere.

## Forked to fix keyboard layout issue

I forked the original Memtype code in order to fix an issue with non-US keyboard layouts, especially German (European) keyboard. The issue is that the ASCII mapping table masks the ALTGR key with 0x40 which makes all HID key codes >= 0x40 not directly usable.

The idea to solve this issue is to re-map required keys with a keycode >= 0x40 to another unused key < 0x40 for the ASCII mapping table. In the HID print code such keys are then mapped back to the original key code.

Current code re-maps the KEY_EUROPE_2 to KEY_F1.

The remapping must also be considered in the keyboard layout files of the  [MemTypeTool](https://github.com/oyzzo/MemTypeTool)

### New branch "unicode"

Trying to implement unicode character lookup.

#### Issues

Not clear how the unicode is saves in the flash.

* Can save and restore special characters like äöü, but print code only seems to get first char or the sequence.
* Need to check MemTypeTool code to understand unicode handling

### Idea for 256 char code table

Instead of trying to implement unicode, another option is to increase the char table and translate the unicode chars to byte value 0x7f.

## Complie & flash#

**FLASHING COMMANDS**

1. GEN HEX: 

```
make clean && make hex
```

2. FUSES & FIRMWARE: *using USBASP programmer*

```
avrdude -c usbasp -p attiny85  -U flash:w:main.hex -U eeprom:w:main.eep -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xfe:m -U lock:w:0xfc:m 
```

For mode detailed instructions on compliling and flashing the code see [Flashing Guide](https://github.com/palto42/memtype/blob/master/documents/flashing_guide.pdf)

### Compiler notes

If more code is added, then the memory size reserved for credentials may need to be reduced.

The credentials memory size is defined in the file  `crd.h`:

```c
/** User Input Button api */

#ifndef _CRD_H_
#define _CRD_H_

#include <stdint.h>
#include <avr/pgmspace.h>

#define MAX_CRED_FLASH_SIZE (2000) // was 2048
#define CRD_START_STR  "[CREDENTIALS]"
```





## References

* IBM [Key Position Codes and Scan Codes for Keyboards](https://www.ibm.com/support/knowledgecenter/en/ssw_aix_61/com.ibm.aix.keyboardtechref/doc/kybdtech/Key.htm)
* [HID Keyboard Code.pdf](http://www.siongboon.com/projects/2007-12-08_ascii_code/HID%20keyboard%20code.pdf)
* ![HID keyboard layout](https://raw.githubusercontent.com/qlyoung/armory-keyboard/master/layouts/keyboard-layout.png)
* Unicode
* ​