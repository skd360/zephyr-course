#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

#define DT_DRV_COMPAT our_driver

#define LED_NODE DT_ALIAS(led0)
//can only register one channel

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);


//if u had another compilation in the same driver then declare it, LOG_MODULE_DECLARE(our_driver, LOG_LEVEL_INF);
//bool led_state = true;


//go to definition of sensor_driver_api and then go to see the definition of channel get
// static int channel_get_my_impl(const struct device *dev,
// 				    enum sensor_channel chan,
// 				    struct sensor_value *val){
//     LOG_INF("HELLO FROM CHANNEL GET , CHANNEL = %d", chan);
//     return 0;
// }
static int our_driver_sample_fetch(const struct device *dev, enum sensor_channel chan){
//    LOG_INF("HELLO FROM SAMPLE FETCH , CHANNEL = %d", chan);
    gpio_pin_set_dt(&led, 1);
    return 0;
}
static int our_driver_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val){
    gpio_pin_set_dt(&led, 0);
    val ->val1 = 0;
    val ->val2 = 0;
    //LOG_INF("HELLO FROM CHANNEL GET , CHANNEL = %d", chan);
    return 0;
}
static DEVICE_API(sensor, api_iomico_lesture) = {
    .sample_fetch = our_driver_sample_fetch,
    .channel_get = our_driver_channel_get,
    //.channel_get = channel_get_my_impl,
};

//init function
static int init(const struct device *dev){
    LOG_INF("HELLO FROM INIT");
    //check if gpio controller is available and ready
    if(!gpio_is_ready_dt(&led)){
        LOG_ERR("GPIO controller is not ready");
        return -ENODEV;
    }
    //configure led as output and start with led off
    int ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_INACTIVE);
    if(ret < 0){
        LOG_ERR("Failed to configure LED pin");
        return ret;
    }
    return 0;
}
//U have to change the number 0 and redefine the init function for each instance of the driver. If you have 2 instances of the driver, then you will have to define 2 init functions and 2 DEVICE_DT_INST_DEFINE macros. The number 0 is the instance number of the driver
//DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_iomico_lesture);
 
//instead of that
#define DEV_INST(inst) DEVICE_DT_INST_DEFINE(inst, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_iomico_lesture);
DT_INST_FOREACH_STATUS_OKAY(DEV_INST);

//POST KERNEL is a init level, CONFIG_SENSOR_INIT_PRIORITY is a init priority, &api_iomico_lesture is a pointer to the api struct
//higher level means later like 80 is priority later

// //u can send function pointers to the driver api struct here. If you don't want to implement a function, you can set it to NULL. The functions are defined in sensor.h
// struct sensor_driver_api api_iomico_lesture = {
//     .attr_set = NULL,
//     .attr_get = NULL,
//     .trigger_set = NULL,
//     .sample_fetch = NULL,
//     .channel_get = NULL,
//     .get_decoder = NULL,
//     .submit = NULL,
// };