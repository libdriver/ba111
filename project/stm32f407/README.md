### 1. Chip

#### 1.1 Chip Info

Chip Name: STM32F407ZGT6.

Extern Oscillator: 8MHz.

UART Pin: TX/RX PA9/PA10.

UART2 Pin: TX/RX PA2/PA3.

### 2. Development and Debugging

#### 2.1 Integrated Development Environment

LibDriver provides both Keil and IAR integrated development environment projects.

MDK is the Keil ARM project and your Keil version must be 5 or higher.Keil ARM project needs STMicroelectronics STM32F4 Series Device Family Pack and you can download from https://www.keil.com/dd2/stmicroelectronics/stm32f407zgtx.

EW is the IAR ARM project and your IAR version must be 9 or higher.

#### 2.2 Serial Port Parameter

Baud Rate: 115200.

Data Bits : 8.

Stop Bits: 1.

Parity: None.

Flow Control: None.

#### 2.3 Serial Port Assistant

We use '\n' to wrap lines.If your serial port assistant displays exceptions (e.g. the displayed content does not divide lines), please modify the configuration of your serial port assistant or replace one that supports '\n' parsing.

### 3. BA111

#### 3.1 Command Instruction

1. Show ba111 chip and driver information.

   ```shell
   ba111 (-i | --information)
   ```

2. Show ba111 help.

   ```shell
   ba111 (-h | --help)
   ```

3. Show ba111 pin connections of the current board.

   ```shell
   ba111 (-p | --port)
   ```
   
4. Run ba111 register test.

   ```shell
   ba111 (-t reg | --test=reg)
   ```
   
5. Run ba111 read test, num is the test times.

   ```shell
   ba111 (-t read | --test=read) [--times=<num>]
   ```

6. Run ba111 read function, num is the test times.

   ```shell
   ba111 (-e read | --example=read) [--times=<num>]
   ```

7. Run ba111 status function.

   ```shell
   ba111 (-e status | --example=status)
   ```
8. Run ba111 baseline function.

   ```shell
   ba111 (-e baseline | --example=baseline)
   ```

#### 3.2 Command Example

```shell
ba111 -i

ba111: chip is AtomBit BA111.
ba111: manufacturer is AtomBit.
ba111: interface is UART.
ba111: driver version is 1.0.
ba111: min supply voltage is 3.3V.
ba111: max supply voltage is 5.0V.
ba111: max current is 3.00mA.
ba111: max temperature is 75.0C.
ba111: min temperature is -10.0C.
```

```shell
ba111 -p

ba111: TX connected to GPIOA PIN3.
ba111: RX connected to GPIOA PIN2.
```

```shell
ba111 -t reg

ba111: chip is AtomBit BA111.
ba111: manufacturer is AtomBit.
ba111: interface is UART.
ba111: driver version is 1.0.
ba111: min supply voltage is 3.3V.
ba111: max supply voltage is 5.0V.
ba111: max current is 3.00mA.
ba111: max temperature is 75.0C.
ba111: min temperature is -10.0C.
ba111: start register test.
ba111: ba111_set_ntc_resistance test.
ba111: set ntc resistance 10k.
ba111: check ntc resistance ok.
ba111: ba111_set_ntc_b test.
ba111: set ntc b 3435.
ba111: check ntc b ok.
ba111: ba111_get_last_status test.
ba111: last status is 0x00.
ba111: finish register test.
```

```shell
ba111 -t read --times=3

ba111: chip is AtomBit BA111.
ba111: manufacturer is AtomBit.
ba111: interface is UART.
ba111: driver version is 1.0.
ba111: min supply voltage is 3.3V.
ba111: max supply voltage is 5.0V.
ba111: max current is 3.00mA.
ba111: max temperature is 75.0C.
ba111: min temperature is -10.0C.
ba111: start read test.
ba111: tds is 225 ppm.
ba111: temperature is 26.99C.
ba111: tds is 225 ppm.
ba111: temperature is 26.99C.
ba111: tds is 225 ppm.
ba111: temperature is 26.99C.
ba111: finish read test.
```

```shell
ba111 -e read --times=3

ba111: 1/3.
ba111: tds is 225 ppm.
ba111: temperature is 27.02C.
ba111: 2/3.
ba111: tds is 225 ppm.
ba111: temperature is 27.02C.
ba111: 3/3.
ba111: tds is 225 ppm.
ba111: temperature is 27.02C.
```

```shell
ba111 -e status

ba111: last status is 0x00.
```
```shell
ba111 -e baseline

ba111: baseline calibration.
```

```shell
ba111 -h

Usage:
  ba111 (-i | --information)
  ba111 (-h | --help)
  ba111 (-p | --port)
  ba111 (-t reg | --test=reg)
  ba111 (-t read | --test=read) [--times=<num>]
  ba111 (-e read | --example=read) [--times=<num>]
  ba111 (-e status | --example=status)
  ba111 (-e baseline | --example=baseline)

Options:
  -e <read | status | baseline>, --example=<read | status | baseline>
                                  Run the driver example.
  -h, --help                      Show the help.
  -i, --information               Show the chip information.
  -p, --port                      Display the pins used by this device to connect the chip.
  -t <reg | read>, --test=<reg | read>
                                  Run the driver test.
      --times=<num>               Set the running times.([default: 3])
```

