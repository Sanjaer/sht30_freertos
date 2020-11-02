/* SHT30 example

    MIT Licensed

    Copyright 2020 Pablo San José Burgos

    Permission is hereby granted, free of charge, to any person obtaining a copy 
    of this software and associated documentation files (the "Software"), 
    to deal in the Software without restriction, including without limitation the rights to 
    use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
    of the Software, and to permit persons to whom the Software is furnished to do so, 
    subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all copies 
    or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
    INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS 
    OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include "sht30.h"

/**         
 * TEST CODE BRIEF
 *
 * This example will show you how to use the SHT30 via I2C:
 *
 * - read external i2c SHT30 sensor
 * - TODO: Use one I2C port(master mode) to configure different modes of the SHT30
 *
 * Pin assignment via menuconfig, defaults are:
 *
 * - master:
 *    GPIO4 (probably D1) is assigned as the data signal of i2c master port
 *    GPIO5 (probably D2) is assigned as the clock signal of i2c master port
 *
 * Connection:
 *
 * - connect sda/scl of sensor with GPIO5/GPIO4
 * - no need to add external pull-up resistors, driver will enable internal pull-up resistors.
 *
 * Test items:
 *
 * - read the sensor data, if connected.
 */



static void i2c_task_example(void *arg)
{
    uint8_t sensor_data[DATA_MSG_SIZE];
    float temp=0, hum = 0;
    static uint32_t count = 0;

    vTaskDelay(100 / portTICK_RATE_MS);
    
    // Init i2c master
    i2c_master_init();
    
    while (1) {

        // Counter num of iterations, no practical use
        count++;
 
        // Set to zeroes all data variables
        memset(sensor_data, 0, DATA_MSG_SIZE);
        temp = 0;
        hum = 0;

        // Single shot data acquisition mode, clock stretching
        i2c_master_sht30_read(I2C_EXAMPLE_MASTER_NUM, SHT30_CMD_START_MSB, SHT30_CMD_START_LSB, sensor_data, DATA_MSG_SIZE);

        // Convert raw data to true values
        temp = convert_raw_to_celsius(sensor_data);
        hum = convert_raw_to_humidity(sensor_data);

        // Print values
        printf("count: %d\n", count);
        printf("temp=%f, hum=%f\n", temp, hum);

        vTaskDelay(1000 / portTICK_RATE_MS);
    }

    i2c_driver_delete(I2C_EXAMPLE_MASTER_NUM);
}

void app_main(void)
{
    //start i2c task
    xTaskCreate(i2c_task_example, "i2c_task_example", 2048, NULL, 10, NULL);
}