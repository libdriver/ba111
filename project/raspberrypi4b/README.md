### 1. Board

#### 1.1 Board Info

Board Name: Raspberry Pi 4B.

UART Pin: TX/RX GPIO14/GPIO15.

### 2. Install

#### 2.1 Dependencies

Install the necessary dependencies.

```shell
sudo apt-get install libgpiod-dev pkg-config cmake -y
```
#### 2.2 Configuration

Enable serial port.

Disable serial console.

#### 2.3 Makefile

Build the project.

```shell
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

#### 2.4 CMake

Build the project.

```shell
mkdir build && cd build 
cmake .. 
make
```

Install the project and this is optional.

```shell
sudo make install
```

Uninstall the project and this is optional.

```shell
sudo make uninstall
```

Test the project and this is optional.

```shell
make test
```

Find the compiled library in CMake. 

```cmake
find_package(ba111 REQUIRED)
```

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
./ba111 -i

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
./ba111 -p

ba111: TX connected to GPIO15(BCM).
ba111: RX connected to GPIO14(BCM).
```

```shell
./ba111 -t reg

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
./ba111 -t read --times=3

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
ba111: tds is 235 ppm.
ba111: temperature is 28.05C.
ba111: tds is 235 ppm.
ba111: temperature is 28.05C.
ba111: tds is 235 ppm.
ba111: temperature is 28.05C.
ba111: finish read test.
```

```shell
./ba111 -e read --times=3

ba111: 1/3.
ba111: tds is 235 ppm.
ba111: temperature is 28.07C.
ba111: 2/3.
ba111: tds is 235 ppm.
ba111: temperature is 28.07C.
ba111: 3/3.
ba111: tds is 235 ppm.
ba111: temperature is 28.07C.
```

```shell
./ba111 -e status

ba111: last status is 0x00.
```
```shell
./ba111 -e baseline

ba111: baseline calibration.
```

```shell
./ba111 -h

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

