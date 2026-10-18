#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

//#define SLEEP_TIME_MS 1000 //removing this for testing kconfig

/* The devicetree node identifier for the "led0" alias. */
//#define LED_NODE DT_ALIAS(led0)
///////////////////#define LED_NODE DT_ALIAS(appled)

//static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

//#define driver_node DT_NODELABEL(our_driver0)

// namespace {
//     void test(){
//         const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
//         struct sensor_value val;
//         auto ret = sensor_channel_get(driver,SENSOR_CHAN_AMBIENT_TEMP,&val);

//         LOG_INF("sensor_channel_ret %d", ret);
//         //k_sleep(K_SECOND(2));


//     }
// }

int main(void)
{
    const struct device* driver = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
    if(!device_is_ready(driver)){
        LOG_ERR("Driver device is not ready");
        return 0;
    }
    else{
        LOG_INF("Driver device is ready");
    }
//    struct sensor_value val;
    // test();
    //bool led_state = true;

    //if (!gpio_is_ready_dt(&led)) return 0;

    //if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    return 0;
}
