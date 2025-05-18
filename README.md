[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver BA111

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/ba111/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

A dedicated chip suitable for detecting total dissolved solids (TDS) and water temperature in water quality, which integrates high-precision oscillation circuits, analog-to-digital conversion circuits, and floating-point arithmetic units. Adopting patented conductivity TDS conversion algorithm and temperature correction algorithm (Atombit) ®）， Quickly detect water quality TDS and achieve automatic temperature correction over a wide temperature range, reducing measurement errors caused by TDS values changing with temperature. Suitable for water quality testing of domestic water.

LibDriver BA111 is a full-featured driver of BA111 launched by LibDriver.It provides the function of TDS reading, temperature reading and other functions. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic](#example-basic)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver BA111 source files.

/interface includes LibDriver BA111 UART platform independent template.

/test includes LibDriver BA111 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver BA111 sample code.

/doc includes LibDriver BA111 offline document.

/datasheet includes BA111 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface UART platform independent template and finish your platform UART driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic

```C
#include "driver_ba111_basic.h"

uint8_t res;
uint16_t tds_ppm;
float temperature;
uint32_t i;

/* init */
res = ba111_basic_init();
if (res != 0)
{
    return 1;
}

...
    
for (i = 0; i < 3; i++)
{
    /* delay 1000ms */
    ba111_interface_delay_ms(1000);

    /* read */
    res = ba111_basic_read(&tds_ppm, &temperature);
    if (res != 0)
    {
        ba111_interface_debug_print("ba111: read failed.\n");
        (void)ba111_basic_deinit();

        return 1;
    }

    /* output */
    ba111_interface_debug_print("ba111: %d/%d.\n", i + 1, 3);
    ba111_interface_debug_print("ba111: tds is %d ppm.\n", tds_ppm);
    ba111_interface_debug_print("ba111: temperature is %0.2fC.\n", temperature);
    
    ...
}

...
    
/* deinit */
(void)ba111_basic_deinit();

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/ba111/index.html](https://www.libdriver.com/docs/ba111/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.