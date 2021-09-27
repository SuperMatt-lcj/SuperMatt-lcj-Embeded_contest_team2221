#include "netdev_ipaddr.h"
#include "netdev.h"
#include <rtdevice.h>
#include "drv_common.h"
#include "time.h"
#include "ntp.h"
#include <rtdbg.h>
#include <board.h>
#define ADC_DEV_NAME        "adc1"      /* ADC �豸���� */
#define ADC_DEV_CHANNEL     1           /* ADC ͨ�� */
#define REFER_VOLTAGE       500         /* �ο���ѹ 5.0v,���ݾ��ȳ���100����2λС��*/
#define CONVERT_BITS        (1 << 16)   /* ת��λ��Ϊ16λ */

rt_thread_t Adc_thread = RT_NULL;


static void Adc_entry(void* paremeter);

void adc_detect(){

Adc_thread = rt_thread_create("adc1",
            Adc_entry,
            RT_NULL,
            1024,
            25,
            5);
    if(Adc_thread != RT_NULL)
        rt_thread_startup(Adc_thread);

}
static void Adc_entry(void* paremeter)
    {
        rt_adc_device_t adc_dev;
        rt_uint32_t value,vol;
        rt_err_t ret = RT_EOK;

        adc_dev = (rt_adc_device_t)rt_device_find(ADC_DEV_NAME);
        if (adc_dev == RT_NULL)
        {
            rt_kprintf("adc sample run failed! can't find %s device!\n", ADC_DEV_NAME);
        }
        /* ʹ���豸 */
        ret = rt_adc_enable(adc_dev, ADC_DEV_CHANNEL);
        if(ret == RT_EOK)
        {
            rt_kprintf("adc sample run success!  find %s device!\n", ADC_DEV_NAME);
        }
        while(1)
        {
            /* ��ȡ����ֵ */
            value = rt_adc_read(adc_dev, ADC_DEV_CHANNEL);
            rt_kprintf("the value is :%d \n", value);
            /* ת��Ϊ��Ӧ��ѹֵ */
            vol = value * REFER_VOLTAGE / CONVERT_BITS;
            rt_kprintf("the voltage is :%d.%02d \n", vol / 100, vol % 100);

            rt_thread_delay(500);
        }
    }
