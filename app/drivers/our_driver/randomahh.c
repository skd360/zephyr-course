#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT our_driver

#define LED_NODE DT_ALIAS(led0)

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

/* ---------------------------------------------------
 * LED FROM DEVICETREE
 * ---------------------------------------------------
 */
static const struct gpio_dt_spec led =
    GPIO_DT_SPEC_GET(LED_NODE, gpios);


/* ---------------------------------------------------
 * DYNAMIC DRIVER DATA
 *
 * This lives in RAM and can change while program runs.
 * Task 2 asks us to modify something inside this struct.
 * ---------------------------------------------------
 */
struct our_driver_data {
    int custom_value;
};


/* ---------------------------------------------------
 * CUSTOM DRIVER API
 *
 * We are extending the normal sensor API.
 * ---------------------------------------------------
 */

/* Function pointer type for our custom function */
typedef int (*our_driver_set_value_t)(
    const struct device *dev,
    int value
);


/*
 * Our custom API contains:
 *
 * 1. Normal sensor API
 * 2. Our extra function
 */
__subsystem struct our_driver_api {

    /* MUST be first because we extend sensor API */
    struct sensor_driver_api sensor_api;

    /* Our custom function */
    our_driver_set_value_t set_custom_value;
};


/*
 * Tell Zephyr:
 *
 * our_driver API extends sensor API
 */
DEVICE_API_EXTENDS(our_driver, sensor, sensor_api);


/* ---------------------------------------------------
 * SAMPLE FETCH
 *
 * Task 1:
 * sensor_sample_fetch() -> LED ON
 * ---------------------------------------------------
 */
static int our_driver_sample_fetch(
    const struct device *dev,
    enum sensor_channel chan)
{
    LOG_INF("sample_fetch called");

    gpio_pin_set_dt(&led, 1);

    return 0;
}


/* ---------------------------------------------------
 * CHANNEL GET
 *
 * Task 1:
 * sensor_channel_get() -> LED OFF
 * ---------------------------------------------------
 */
static int our_driver_channel_get(
    const struct device *dev,
    enum sensor_channel chan,
    struct sensor_value *val)
{
    LOG_INF("channel_get called");

    gpio_pin_set_dt(&led, 0);

    val->val1 = 0;
    val->val2 = 0;

    return 0;
}


/* ---------------------------------------------------
 * CUSTOM FUNCTION IMPLEMENTATION
 *
 * Task 2:
 * Change a parameter inside dynamic data.
 * ---------------------------------------------------
 */
static int our_driver_set_custom_value(
    const struct device *dev,
    int value)
{
    /*
     * dev->data points to our_driver_data
     */
    struct our_driver_data *data = dev->data;

    /*
     * Change dynamic data
     */
    data->custom_value = value;

    LOG_INF("custom_value changed to %d",
            data->custom_value);

    return 0;
}


/* ---------------------------------------------------
 * API STRUCT
 *
 * Connect Zephyr API + our custom API
 * to our actual functions.
 * ---------------------------------------------------
 */
static DEVICE_API(our_driver, api_iomico_lecture) = {

    /*
     * Standard Sensor API
     */
    .sensor_api = {
        .sample_fetch = our_driver_sample_fetch,
        .channel_get = our_driver_channel_get,
    },

    /*
     * Custom extension API
     */
    .set_custom_value = our_driver_set_custom_value,
};


/* ---------------------------------------------------
 * DRIVER INITIALIZATION
 * ---------------------------------------------------
 */
static int init(const struct device *dev)
{
    LOG_INF("HELLO FROM INIT");

    /*
     * Check GPIO controller
     */
    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("LED GPIO not ready");
        return -ENODEV;
    }


    /*
     * Configure LED as output.
     * Initially OFF.
     */
    int ret =
        gpio_pin_configure_dt(
            &led,
            GPIO_OUTPUT_INACTIVE
        );

    if (ret < 0) {
        LOG_ERR("Failed to configure LED");
        return ret;
    }


    /*
     * Initialize dynamic data
     */
    struct our_driver_data *data = dev->data;

    data->custom_value = 0;

    LOG_INF("Initial custom_value = %d",
            data->custom_value);

    return 0;
}


/* ---------------------------------------------------
 * CREATE DRIVER INSTANCES
 * ---------------------------------------------------
 */

#define DEV_INST(inst)                                   \
                                                        \
    static struct our_driver_data                        \
        our_driver_data_##inst;                          \
                                                        \
    DEVICE_DT_INST_DEFINE(                               \
        inst,                                            \
        init,                                            \
        NULL,                                            \
        &our_driver_data_##inst,                         \
        NULL,                                            \
        POST_KERNEL,                                     \
        80,                                              \
        &api_iomico_lecture                              \
    );


DT_INST_FOREACH_STATUS_OKAY(DEV_INST)