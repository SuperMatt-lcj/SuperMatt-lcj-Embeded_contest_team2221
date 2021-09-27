/*
 * Copyright (c) 2006-2019, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-02-20     Mculover666  the first version
 */

#include <rtthread.h>
#include <board.h>
#include <sht3x.h>

#define THREAD_PRIORITY         31
#define THREAD_STACK_SIZE       1024
#define THREAD_TIMESLICE        5

static rt_thread_t tid1 = RT_NULL;

/* ��ں��� */
static void sht31_collect_thread_entry(void *parameter)
{
    sht3x_device_t  sht3x_device;
    sht3x_device = sht3x_init("i2c1", 0x44);
    //�˴���Ҫ���ݵ���ʱ����������������ȡʧ�ܵ����⣬��л���ѵ�����
     rt_thread_mdelay(200);
    while (1)
    {
        if(RT_EOK == sht3x_read_singleshot(sht3x_device))
        {
            rt_kprintf("sht31 humidity   : %d.%d  ", (int)sht3x_device->humidity, (int)(sht3x_device->humidity * 10) % 10);
            rt_kprintf("temperature: %d.%d\n", (int)sht3x_device->temperature, (int)(sht3x_device->temperature * 10) % 10);
            lcd_show_string(66, 354, 16, "Current Environment:");
            lcd_show_string(50, 370, 16, "%d.%d Celcius %d.%d %",(int)sht3x_device->temperature, (int)(sht3x_device->temperature * 10) % 10,
                           (int)sht3x_device->humidity, (int)(sht3x_device->humidity * 10) % 10);
        }
        else
        {
            rt_kprintf("read sht3x fail.\r\n");
          //  break;
        }
        rt_thread_mdelay(2000);
    }
}

/* �����߳� */
int sht31_collect(void)
{
    /* �����߳�*/
    tid1 = rt_thread_create("sht31_collect_thread",
            sht31_collect_thread_entry, RT_NULL,
                            THREAD_STACK_SIZE,
                            THREAD_PRIORITY, THREAD_TIMESLICE);

    /* �������߳̿��ƿ飬��������߳� */
    if (tid1 != RT_NULL)
        rt_thread_startup(tid1);

    return 0;
}

