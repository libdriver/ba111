/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      main.c
 * @brief     main source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2025-05-15
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2025/05/15  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_ba111_basic.h"
#include "driver_ba111_register_test.h"
#include "driver_ba111_read_test.h"
#include <getopt.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief     ba111 full function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 5 param is invalid
 * @note      none
 */
uint8_t ba111(uint8_t argc, char **argv)
{
    int c;
    int longindex = 0;
    const char short_options[] = "hipe:t:";
    const struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"information", no_argument, NULL, 'i'},
        {"port", no_argument, NULL, 'p'},
        {"example", required_argument, NULL, 'e'},
        {"test", required_argument, NULL, 't'},
        {"times", required_argument, NULL, 1},
        {NULL, 0, NULL, 0},
    };
    char type[33] = "unknown";
    uint32_t times = 3;
    
    /* if no params */
    if (argc == 1)
    {
        /* goto the help */
        goto help;
    }
    
    /* init 0 */
    optind = 0;
    
    /* parse */
    do
    {
        /* parse the args */
        c = getopt_long(argc, argv, short_options, long_options, &longindex);
        
        /* judge the result */
        switch (c)
        {
            /* help */
            case 'h' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "h");
                
                break;
            }
            
            /* information */
            case 'i' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "i");
                
                break;
            }
            
            /* port */
            case 'p' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "p");
                
                break;
            }
            
            /* example */
            case 'e' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "e_%s", optarg);
                
                break;
            }
            
            /* test */
            case 't' :
            {
                /* set the type */
                memset(type, 0, sizeof(char) * 33);
                snprintf(type, 32, "t_%s", optarg);
                
                break;
            }
            
            /* times */
            case 1 :
            {
                /* set times */
                times = atoi(optarg);
                
                break;
            }
            
            /* the end */
            case -1 :
            {
                break;
            }
            
            /* others */
            default :
            {
                return 5;
            }
        }
    } while (c != -1);

    /* run the function */
    if (strcmp("t_reg", type) == 0)
    {
        /* run register test */
        if (ba111_register_test() != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("t_read", type) == 0)
    {
        /* run read test */
        if (ba111_read_test(times) != 0)
        {
            return 1;
        }
        
        return 0;
    }
    else if (strcmp("e_read", type) == 0)
    {
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
        
        for (i = 0; i < times; i++)
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
            ba111_interface_debug_print("ba111: %d/%d.\n", i + 1, times);
            ba111_interface_debug_print("ba111: tds is %d ppm.\n", tds_ppm);
            ba111_interface_debug_print("ba111: temperature is %0.2fC.\n", temperature);
        }
        
        /* deinit */
        (void)ba111_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_baseline", type) == 0)
    {
        uint8_t res;
        
        /* init */
        res = ba111_basic_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* baseline calibration */
        res = ba111_basic_baseline_calibration();
        if (res != 0)
        {
            (void)ba111_basic_deinit();
            
            return 1;
        }
        
        /* baseline calibration */
        ba111_interface_debug_print("ba111: baseline calibration.\n");
        
        /* deinit */
        (void)ba111_basic_deinit();
        
        return 0;
    }
    else if (strcmp("e_status", type) == 0)
    {
        uint8_t res;
        ba111_status_t status;
        
        /* init */
        res = ba111_basic_init();
        if (res != 0)
        {
            return 1;
        }
        
        /* get last status */
        res = ba111_basic_get_last_status(&status);
        if (res != 0)
        {
            (void)ba111_basic_deinit();
            
            return 1;
        }
        
        /* get last status */
        ba111_interface_debug_print("ba111: last status is 0x%02X.\n", (uint8_t)status);
        
        /* deinit */
        (void)ba111_basic_deinit();
        
        return 0;
    }
    else if (strcmp("h", type) == 0)
    {
        help:
        ba111_interface_debug_print("Usage:\n");
        ba111_interface_debug_print("  ba111 (-i | --information)\n");
        ba111_interface_debug_print("  ba111 (-h | --help)\n");
        ba111_interface_debug_print("  ba111 (-p | --port)\n");
        ba111_interface_debug_print("  ba111 (-t reg | --test=reg)\n");
        ba111_interface_debug_print("  ba111 (-t read | --test=read) [--times=<num>]\n");
        ba111_interface_debug_print("  ba111 (-e read | --example=read) [--times=<num>]\n");
        ba111_interface_debug_print("  ba111 (-e status | --example=status)\n");
        ba111_interface_debug_print("  ba111 (-e baseline | --example=baseline)\n");
        ba111_interface_debug_print("\n");
        ba111_interface_debug_print("Options:\n");
        ba111_interface_debug_print("  -e <read | status | baseline>, --example=<read | status | baseline>\n");
        ba111_interface_debug_print("                                  Run the driver example.\n");
        ba111_interface_debug_print("  -h, --help                      Show the help.\n");
        ba111_interface_debug_print("  -i, --information               Show the chip information.\n");
        ba111_interface_debug_print("  -p, --port                      Display the pins used by this device to connect the chip.\n");
        ba111_interface_debug_print("  -t <reg | read>, --test=<reg | read>\n");
        ba111_interface_debug_print("                                  Run the driver test.\n");
        ba111_interface_debug_print("      --times=<num>               Set the running times.([default: 3])\n");
        
        return 0;
    }
    else if (strcmp("i", type) == 0)
    {
        ba111_info_t info;
        
        /* print ba111 info */
        ba111_info(&info);
        ba111_interface_debug_print("ba111: chip is %s.\n", info.chip_name);
        ba111_interface_debug_print("ba111: manufacturer is %s.\n", info.manufacturer_name);
        ba111_interface_debug_print("ba111: interface is %s.\n", info.interface);
        ba111_interface_debug_print("ba111: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ba111_interface_debug_print("ba111: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ba111_interface_debug_print("ba111: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ba111_interface_debug_print("ba111: max current is %0.2fmA.\n", info.max_current_ma);
        ba111_interface_debug_print("ba111: max temperature is %0.1fC.\n", info.temperature_max);
        ba111_interface_debug_print("ba111: min temperature is %0.1fC.\n", info.temperature_min);
        
        return 0;
    }
    else if (strcmp("p", type) == 0)
    {
        /* print pin connection */
        ba111_interface_debug_print("ba111: TX connected to GPIO15(BCM).\n");
        ba111_interface_debug_print("ba111: RX connected to GPIO14(BCM).\n");
        
        return 0;
    }
    else
    {
        return 5;
    }
}

/**
 * @brief     main function
 * @param[in] argc arg numbers
 * @param[in] **argv arg address
 * @return    status code
 *             - 0 success
 * @note      none
 */
int main(uint8_t argc, char **argv)
{
    uint8_t res;

    res = ba111(argc, argv);
    if (res == 0)
    {
        /* run success */
    }
    else if (res == 1)
    {
        ba111_interface_debug_print("ba111: run failed.\n");
    }
    else if (res == 5)
    {
        ba111_interface_debug_print("ba111: param is invalid.\n");
    }
    else
    {
        ba111_interface_debug_print("ba111: unknown status code.\n");
    }

    return 0;
}
