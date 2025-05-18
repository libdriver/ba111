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
 * @file      driver_ba111_register_test.c
 * @brief     driver ba111 register test source file
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

#include "driver_ba111_register_test.h"

static ba111_handle_t gs_handle;        /**< ba111 handle */

/**
 * @brief  register test
 * @return status code
 *         - 0 success
 *         - 1 test failed
 * @note   none
 */
uint8_t ba111_register_test(void)
{
    uint8_t res;
    ba111_status_t status;
    ba111_info_t info;
    
    /* link interface function */
    DRIVER_BA111_LINK_INIT(&gs_handle, ba111_handle_t);
    DRIVER_BA111_LINK_UART_INIT(&gs_handle, ba111_interface_uart_init);
    DRIVER_BA111_LINK_UART_DEINIT(&gs_handle, ba111_interface_uart_deinit);
    DRIVER_BA111_LINK_UART_READ(&gs_handle, ba111_interface_uart_read);
    DRIVER_BA111_LINK_UART_FLUSH(&gs_handle, ba111_interface_uart_flush);
    DRIVER_BA111_LINK_UART_WRITE(&gs_handle, ba111_interface_uart_write);
    DRIVER_BA111_LINK_DELAY_MS(&gs_handle, ba111_interface_delay_ms);
    DRIVER_BA111_LINK_DEBUG_PRINT(&gs_handle, ba111_interface_debug_print);
    
    /* get ba111 information */
    res = ba111_info(&info);
    if (res != 0)
    {
        ba111_interface_debug_print("ba111: get info failed.\n");
       
        return 1;
    }
    else
    {
        /* print chip information */
        ba111_interface_debug_print("ba111: chip is %s.\n", info.chip_name);
        ba111_interface_debug_print("ba111: manufacturer is %s.\n", info.manufacturer_name);
        ba111_interface_debug_print("ba111: interface is %s.\n", info.interface);
        ba111_interface_debug_print("ba111: driver version is %d.%d.\n", info.driver_version / 1000, (info.driver_version % 1000) / 100);
        ba111_interface_debug_print("ba111: min supply voltage is %0.1fV.\n", info.supply_voltage_min_v);
        ba111_interface_debug_print("ba111: max supply voltage is %0.1fV.\n", info.supply_voltage_max_v);
        ba111_interface_debug_print("ba111: max current is %0.2fmA.\n", info.max_current_ma);
        ba111_interface_debug_print("ba111: max temperature is %0.1fC.\n", info.temperature_max);
        ba111_interface_debug_print("ba111: min temperature is %0.1fC.\n", info.temperature_min);
    }
    
    /* start register test */
    ba111_interface_debug_print("ba111: start register test.\n");
    
    /* ba111 init */
    res = ba111_init(&gs_handle);
    if (res != 0)
    {
        ba111_interface_debug_print("ba111: init failed.\n");

        return 1;
    }
    
    /* ba111_set_ntc_resistance test */
    ba111_interface_debug_print("ba111: ba111_set_ntc_resistance test.\n");
    
    /* set ntc resistance */
    ba111_interface_debug_print("ba111: set ntc resistance 10k.\n");
    
    /* set ntc resistance */
    res = ba111_set_ntc_resistance(&gs_handle, 10 * 1000);
    if (res != 0)
    {
        ba111_interface_debug_print("ba111: set ntc resistance failed.\n");
        (void)ba111_deinit(&gs_handle);
        
        return 1;
    }
    ba111_interface_debug_print("ba111: check ntc resistance %s.\n", res == 0 ? "ok" : "error");
    
    /* ba111_set_ntc_b test */
    ba111_interface_debug_print("ba111: ba111_set_ntc_b test.\n");
    
    /* set ntc b */
    ba111_interface_debug_print("ba111: set ntc b 3435.\n");
    
    /* set ntc b */
    res = ba111_set_ntc_b(&gs_handle, 3435);
    if (res != 0)
    {
        ba111_interface_debug_print("ba111: set ntc b failed.\n");
        (void)ba111_deinit(&gs_handle);
        
        return 1;
    }
    ba111_interface_debug_print("ba111: check ntc b %s.\n", res == 0 ? "ok" : "error");
    
#if (BA111_ENABLE_BASELINE_TEST != 0)
    /* ba111_baseline_calibration test */
    ba111_interface_debug_print("ba111: ba111_baseline_calibration test.\n");
    
    /* baseline calibration */
    ba111_interface_debug_print("ba111: baseline calibration.\n");
    
    /* baseline calibration */
    res = ba111_baseline_calibration(&gs_handle);
    if (res != 0)
    {
        ba111_interface_debug_print("ba111: baseline calibration failed.\n");
        (void)ba111_deinit(&gs_handle);
        
        return 1;
    }
    ba111_interface_debug_print("ba111: check baseline calibration %s.\n", res == 0 ? "ok" : "error");
#endif
    
    /* ba111_get_last_status test */
    ba111_interface_debug_print("ba111: ba111_get_last_status test.\n");
    
    /* get last status */
    res = ba111_get_last_status(&gs_handle, &status);
    if (res != 0)
    {
        ba111_interface_debug_print("ba111: get last status failed.\n");
        (void)ba111_deinit(&gs_handle);
        
        return 1;
    }
    ba111_interface_debug_print("ba111: last status is 0x%02X.\n", (uint8_t)status);
    
    /* finish register test */
    ba111_interface_debug_print("ba111: finish register test.\n");
    (void)ba111_deinit(&gs_handle);

    return 0;
}
