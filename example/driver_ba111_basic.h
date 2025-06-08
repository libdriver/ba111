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
 * @file      driver_ba111_basic.h
 * @brief     driver ba111 basic header file
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

#ifndef DRIVER_BA111_BASIC_H
#define DRIVER_BA111_BASIC_H

#include "driver_ba111_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup ba111_example_driver ba111 example driver function
 * @brief    ba111 example driver modules
 * @ingroup  ba111_driver
 * @{
 */

/**
 * @brief ba111 basic send command configure
 */
#ifndef BA111_BASIC_SEND_CONFIG
    #define BA111_BASIC_SEND_CONFIG        0        /**< don't send */
#endif

/**
 * @brief ba111 basic example default definition
 */
#define BA111_BASIC_DEFAULT_NTC_RESISTANCE        (10 * 1000)        /**< 10k */
#define BA111_BASIC_DEFAULT_NTC_B                  3435              /**< 3435 */

/**
 * @brief  basic example init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t ba111_basic_init(void);

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t ba111_basic_deinit(void);

/**
 * @brief      basic example read
 * @param[out] *tds_ppm pointer to a tds ppm data buffer
 * @param[out] *temperature_deg pointer to a converted temperature data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t ba111_basic_read(uint16_t *tds_ppm, float *temperature_deg);

/**
 * @brief  basic example baseline calibration
 * @return status code
 *         - 0 success
 *         - 1 baseline calibration failed
 * @note   none
 */
uint8_t ba111_basic_baseline_calibration(void);

/**
 * @brief      basic example get last status
 * @param[out] *status pointer to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get last status failed
 * @note       none
 */
uint8_t ba111_basic_get_last_status(ba111_status_t *status);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
