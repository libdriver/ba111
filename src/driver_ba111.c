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
 * @file      driver_ba111.c
 * @brief     driver ba111 source file
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

#include "driver_ba111.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "AtomBit BA111"        /**< chip name */
#define MANUFACTURER_NAME         "AtomBit"              /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        3.3f                   /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        5.0f                   /**< chip max supply voltage */
#define MAX_CURRENT               3.0f                   /**< chip max current */
#define TEMPERATURE_MIN           -10.0f                 /**< chip min operating temperature */
#define TEMPERATURE_MAX           75.0f                  /**< chip max operating temperature */
#define DRIVER_VERSION            1000                   /**< driver version */

/**
 * @brief chip command definition
 */
#define BA111_COMMAND_READ        0xA0        /**< read command */
#define BA111_COMMAND_BASELINE    0xA6        /**< baseline command */
#define BA111_COMMAND_NTC_RES     0xA3        /**< ntc resistance command */
#define BA111_COMMAND_NTC_B       0xA5        /**< ntc b command */

/**
 * @brief      make frame
 * @param[in]  command input command
 * @param[in]  data input data
 * @param[out] *output pointer to an output buffer
 * @return     status code
 *             - 0 success
 *             - 1 make frame failed
 * @note       none
 */
static uint8_t a_ba111_make_frame(uint8_t command, uint32_t data, uint8_t output[6])
{
    uint8_t i;
    uint16_t sum;
    
    output[0] = command;                    /* set command */
    output[1] = (data >> 24) & 0xFF;        /* set data */
    output[2] = (data >> 16) & 0xFF;        /* set data */
    output[3] = (data >> 8) & 0xFF;         /* set data */
    output[4] = (data >> 0) & 0xFF;         /* set data */
    
    sum = 0;                                /* init 0 */
    for (i = 0; i < 5; i++)                 /* add all */
    {
        sum += output[i];                   /* sum */
    }
    output[5] = sum & 0xFF;                 /* set sum */
    
    return 0;                               /* success return 0 */
}

/**
 * @brief      parse frame
 * @param[in]  *handle pointer to a ba111 handle structure
 * @param[in]  is_data bool value
 * @param[in]  *input pointer to an input buffer
 * @param[out] *data pointer to a data buffer
 * @return     status code
 *             - 0 success
 *             - 1 parse frame failed
 * @note       none
 */
static uint8_t a_ba111_parse_frame(ba111_handle_t *handle, uint8_t is_data, uint8_t input[6], uint32_t *data)
{
    uint8_t i;
    uint16_t sum;
    
    sum = 0;                                                                   /* init 0 */
    for (i = 0; i < 5; i++)                                                    /* add all */
    {
        sum += input[i];                                                       /* sum */
    }
    if ((sum & 0xFF) != input[5])                                              /* check sum */
    {
        handle->debug_print("ba111: checksum error.\n");                       /* checksum error */
        
        return 1;                                                              /* return error */
    }
    if (is_data != 0)                                                          /* is data */
    {
        if (input[0] != 0xAA)                                                  /* check frame header */
        {
            handle->debug_print("ba111: frame header invalid.\n");             /* frame header invalid */
            
            return 1;                                                          /* return error */
        }
        *data = ((uint32_t)input[1] << 24) | ((uint32_t)input[2] << 16) | 
                ((uint32_t)input[3] << 8) | ((uint32_t)input[4] << 0);         /* get data */
    }
    else
    {
        if (input[0] != 0xAC)                                                  /* check frame header */
        {
            handle->debug_print("ba111: frame header invalid.\n");             /* frame header invalid */
            
            return 1;                                                          /* return error */
        }
        *data = ((uint32_t)input[1] << 24) | ((uint32_t)input[2] << 26) | 
                ((uint32_t)input[3] << 8) | ((uint32_t)input[4] << 0);         /* get data */
    }
    
    return 0;                                                                  /* success return 0 */
}

/**
 * @brief     initialize the chip
 * @param[in] *handle pointer to a ba111 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 uart initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 * @note      none
 */
uint8_t ba111_init(ba111_handle_t *handle)
{
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->debug_print == NULL)                                /* check debug_print */
    {
        return 3;                                                   /* return error */
    }
    if (handle->uart_init == NULL)                                  /* check uart_init */
    {
        handle->debug_print("ba111: uart_init is null.\n");         /* uart_init is null */
        
        return 3;                                                   /* return error */
    }
    if (handle->uart_deinit == NULL)                                /* check uart_deinit */
    {
        handle->debug_print("ba111: uart_deinit is null.\n");       /* uart_deinit is null */
        
        return 3;                                                   /* return error */
    }
    if (handle->uart_read == NULL)                                  /* check uart_read */
    {
        handle->debug_print("ba111: uart_read is null.\n");         /* uart_read is null */
        
        return 3;                                                   /* return error */
    }
    if (handle->uart_flush == NULL)                                 /* check uart_flush */
    {
        handle->debug_print("ba111: uart_flush is null.\n");        /* uart_flush is null */
        
        return 3;                                                   /* return error */
    }
    if (handle->uart_write == NULL)                                 /* check uart_write */
    {
        handle->debug_print("ba111: uart_write is null.\n");        /* uart_write is null */
        
        return 3;                                                   /* return error */
    }
    if (handle->delay_ms == NULL)                                   /* check delay_ms */
    {
        handle->debug_print("ba111: delay_ms is null.\n");          /* delay_ms is null */
        
        return 3;                                                   /* return error */
    }
    
    if (handle->uart_init() != 0)                                   /* uart init */
    {
        handle->debug_print("ba111: uart init failed.\n");          /* uart init failed */
        
        return 1;                                                   /* return error */
    }
    
    handle->inited = 1;                                             /* flag finish initialization */
    
    return 0;                                                       /* success return 0 */
}

/**
 * @brief     close the chip
 * @param[in] *handle pointer to a ba111 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 uart deinit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ba111_deinit(ba111_handle_t *handle)
{
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
     
    if (handle->uart_deinit() != 0)                                 /* uart deinit */
    {
        handle->debug_print("ba111: uart deinit failed.\n");        /* uart deinit failed */
        
        return 1;                                                   /* return error */
    }         
    handle->inited = 0;                                             /* flag close */
    
    return 0;                                                       /* success return 0 */ 
}

/**
 * @brief      read the data
 * @param[in]  *handle pointer to a ba111 handle structure
 * @param[out] *tds_raw pointer to a tds raw data buffer
 * @param[out] *tds_ppm pointer to a tds ppm data buffer
 * @param[out] *temperature_raw pointer to a temperature raw data buffer
 * @param[out] *temperature pointer to a converted temperature data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 frame error
 * @note       none
 */
uint8_t ba111_read(ba111_handle_t *handle, uint16_t *tds_raw, uint16_t *tds_ppm, 
                   uint16_t *temperature_raw, float *temperature)
{
    uint8_t res;
    uint8_t output[6];
    uint8_t input[6];
    uint16_t len;
    uint32_t data;
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    
    (void)a_ba111_make_frame(BA111_COMMAND_READ, 0x00000000, output);        /* make frame */
    res = handle->uart_flush();                                              /* uart flush */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("ba111: uart flush failed.\n");                  /* uart flush failed */
        
        return 1;                                                            /* return error */
    }
    res = handle->uart_write(output, 6);                                     /* uart write */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("ba111: uart write failed.\n");                  /* uart write failed */
        
        return 1;                                                            /* return error */
    }
    handle->delay_ms(800);                                                   /* delay 800ms */
    len = handle->uart_read(input, 6);                                       /* uart read */
    if (len != 6)                                                            /* check length */
    {
        handle->debug_print("ba111: uart read failed.\n");                   /* uart read failed */
        
        return 1;                                                            /* return error */
    }
    res = a_ba111_parse_frame(handle, 1, input, &data);                      /* parse data */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("ba111: frame error.\n");                        /* frame error */
        
        return 4;                                                            /* return error */
    }
    *tds_raw = (data >> 16) & 0xFFFFU;                                       /* set tds raw */
    *tds_ppm = *tds_raw ;                                                    /* set tds ppm */
    *temperature_raw = (data >> 0) & 0xFFFFU;                                /* set temperature raw */
    *temperature = (float)(*temperature_raw) / 100.0f;                       /* set temperature */
    
    return 0;                                                                /* success return 0 */ 
}

/**
 * @brief     baseline calibration
 * @param[in] *handle pointer to a ba111 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 baseline calibration failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 frame error
 *            - 5 response error
 * @note      none
 */
uint8_t ba111_baseline_calibration(ba111_handle_t *handle)
{
    uint8_t res;
    uint8_t output[6];
    uint8_t input[6];
    uint16_t len;
    uint32_t data;
    
    if (handle == NULL)                                                          /* check handle */
    {
        return 2;                                                                /* return error */
    }
    if (handle->inited != 1)                                                     /* check handle initialization */
    {
        return 3;                                                                /* return error */
    }
    
    (void)a_ba111_make_frame(BA111_COMMAND_BASELINE, 0x00000000, output);        /* make frame */
    res = handle->uart_flush();                                                  /* uart flush */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("ba111: uart flush failed.\n");                      /* uart flush failed */
        
        return 1;                                                                /* return error */
    }
    res = handle->uart_write(output, 6);                                         /* uart write */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("ba111: uart write failed.\n");                      /* uart write failed */
        
        return 1;                                                                /* return error */
    }
    handle->delay_ms(500);                                                       /* delay 500ms */
    len = handle->uart_read(input, 6);                                           /* uart read */
    if (len != 6)                                                                /* check length */
    {
        handle->debug_print("ba111: uart read failed.\n");                       /* uart read failed */
        
        return 1;                                                                /* return error */
    }
    res = a_ba111_parse_frame(handle, 0, input, &data);                          /* parse data */
    if (res != 0)                                                                /* check result */
    {
        handle->debug_print("ba111: frame error.\n");                            /* frame error */
        
        return 4;                                                                /* return error */
    }
    handle->last_status = (data >> 24) & 0xFF;                                   /* save last status */
    if (handle->last_status != 0)                                                /* check last status */
    {
        handle->debug_print("ba111: response error.\n");                         /* response error */
        
        return 5;                                                                /* return error */
    }
    
    return 0;                                                                    /* success return 0 */ 
}

/**
 * @brief     set ntc resistance
 * @param[in] *handle pointer to a ba111 handle structure
 * @param[in] ohm resistance ohm
 * @return    status code
 *            - 0 success
 *            - 1 set ntc resistance failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 frame error
 *            - 5 response error
 * @note      none
 */
uint8_t ba111_set_ntc_resistance(ba111_handle_t *handle, uint32_t ohm)
{
    uint8_t res;
    uint8_t output[6];
    uint8_t input[6];
    uint16_t len;
    uint32_t data;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }
    
    (void)a_ba111_make_frame(BA111_COMMAND_NTC_RES, ohm, output);        /* make frame */
    res = handle->uart_flush();                                          /* uart flush */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("ba111: uart flush failed.\n");              /* uart flush failed */
        
        return 1;                                                        /* return error */
    }
    res = handle->uart_write(output, 6);                                 /* uart write */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("ba111: uart write failed.\n");              /* uart write failed */
        
        return 1;                                                        /* return error */
    }
    handle->delay_ms(500);                                               /* delay 500ms */
    len = handle->uart_read(input, 6);                                   /* uart read */
    if (len != 6)                                                        /* check length */
    {
        handle->debug_print("ba111: uart read failed.\n");               /* uart read failed */
        
        return 1;                                                        /* return error */
    }
    res = a_ba111_parse_frame(handle, 0, input, &data);                  /* parse data */
    if (res != 0)                                                        /* check result */
    {
        handle->debug_print("ba111: frame error.\n");                    /* frame error */
        
        return 4;                                                        /* return error */
    }
    handle->last_status = (data >> 24) & 0xFF;                           /* save last status */
    if (handle->last_status != 0)                                        /* check last status */
    {
        handle->debug_print("ba111: response error.\n");                 /* response error */
        
        return 5;                                                        /* return error */
    }
    
    return 0;                                                            /* success return 0 */ 
}

/**
 * @brief     set ntc b
 * @param[in] *handle pointer to a ba111 handle structure
 * @param[in] value ntc value
 * @return    status code
 *            - 0 success
 *            - 1 set ntc b failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 frame error
 *            - 5 response error
 * @note      none
 */
uint8_t ba111_set_ntc_b(ba111_handle_t *handle, uint16_t value)
{
    uint8_t res;
    uint8_t output[6];
    uint8_t input[6];
    uint16_t len;
    uint32_t data;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    (void)a_ba111_make_frame(BA111_COMMAND_NTC_B, value << 16, output);        /* make frame */
    res = handle->uart_flush();                                                /* uart flush */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ba111: uart flush failed.\n");                    /* uart flush failed */
        
        return 1;                                                              /* return error */
    }
    res = handle->uart_write(output, 6);                                       /* uart write */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ba111: uart write failed.\n");                    /* uart write failed */
        
        return 1;                                                              /* return error */
    }
    handle->delay_ms(500);                                                     /* delay 500ms */
    len = handle->uart_read(input, 6);                                         /* uart read */
    if (len != 6)                                                              /* check length */
    {
        handle->debug_print("ba111: uart read failed.\n");                     /* uart read failed */
        
        return 1;                                                              /* return error */
    }
    res = a_ba111_parse_frame(handle, 0, input, &data);                        /* parse data */
    if (res != 0)                                                              /* check result */
    {
        handle->debug_print("ba111: frame error.\n");                          /* frame error */
        
        return 4;                                                              /* return error */
    }
    handle->last_status = (data >> 24) & 0xFF;                                 /* save last status */
    if (handle->last_status != 0)                                              /* check last status */
    {
        handle->debug_print("ba111: response error.\n");                       /* response error */
        
        return 5;                                                              /* return error */
    }
    
    return 0;                                                                  /* success return 0 */ 
}

/**
 * @brief      get last status
 * @param[in]  *handle pointer to a ba111 handle structure
 * @param[out] *status pointer to a status buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ba111_get_last_status(ba111_handle_t *handle, ba111_status_t *status)
{
    if (handle == NULL)                                     /* check handle */
    {
        return 2;                                           /* return error */
    }
    if (handle->inited != 1)                                /* check handle initialization */
    {
        return 3;                                           /* return error */
    }
    *status = (ba111_status_t)(handle->last_status);        /* set status */
    
    return 0;                                               /* success return 0 */
}

/**
 * @brief     set buffer
 * @param[in] *handle pointer to a ba111 handle structure
 * @param[in] *buf pointer to a data buffer
 * @param[in] len buffer length
 * @return    status code
 *            - 0 success
 *            - 1 set buffer failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t ba111_set_buffer(ba111_handle_t *handle, uint8_t *buf, uint16_t len)
{
    uint8_t res;

    if (handle == NULL)                                            /* check handle */
    {
        return 2;                                                  /* return error */
    }
    if (handle->inited != 1)                                       /* check handle initialization */
    {
        return 3;                                                  /* return error */
    }
    
    res = handle->uart_flush();                                    /* uart flush */
    if (res != 0)                                                  /* check result */
    {
        handle->debug_print("ba111: uart flush failed.\n");        /* uart flush failed */
        
        return 1;                                                  /* return error */
    }
    res = handle->uart_write(buf, len);                            /* uart write */
    if (res != 0)                                                  /* check result */
    {
        handle->debug_print("ba111: uart write failed.\n");        /* uart write failed */
        
        return 1;                                                  /* return error */
    }
    
    return 0;                                                      /* success return 0 */ 
}

/**
 * @brief      get buffer
 * @param[in]  *handle pointer to a ba111 handle structure
 * @param[out] *buf pointer to a data buffer
 * @param[in]  len buffer length
 * @return     status code
 *             - 0 success
 *             - 1 get buffer failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t ba111_get_buffer(ba111_handle_t *handle, uint8_t *buf, uint16_t len)
{
    uint16_t l;

    if (handle == NULL)                                           /* check handle */
    {
        return 2;                                                 /* return error */
    }
    if (handle->inited != 1)                                      /* check handle initialization */
    {
        return 3;                                                 /* return error */
    }
    
    l = handle->uart_read((uint8_t *)buf, len);                   /* uart read */
    if (l != len)                                                 /* check result */
    {
        handle->debug_print("ba111: uart read failed.\n");        /* uart read failed */
       
        return 1;                                                 /* return error */
    }
    
    return 0;                                                     /* success return 0 */ 
}

/**
 * @brief      get chip's information
 * @param[out] *info pointer to a ba111 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t ba111_info(ba111_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(ba111_info_t));                          /* initialize ba111 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "UART", 8);                            /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
