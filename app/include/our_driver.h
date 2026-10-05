#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

/*
 * Our custom API structure.
 *
 * IMPORTANT:
 * sensor_driver_api MUST be first.
 */
struct our_driver_api {
    struct sensor_driver_api sensor_api;

    int (*set_custom_value)(
        const struct device *dev,
        int value
    );
};


/*
 * Function main.cpp will call.
 */
static inline int our_driver_set_custom_value(
    const struct device *dev,
    int value)
{
    const struct our_driver_api *api =
        (const struct our_driver_api *)dev->api;

    return api->set_custom_value(dev, value);
}

#endif
// #pragma once

// #include <zephyr/device.h>
// #include <zephyr/drivers/sensor.h>

// typedef int (*our_driver_set_value_t)(
//     const struct device *dev,
//     int value
// );

// __subsystem struct our_driver_api {
//     struct sensor_driver_api sensor_api;

//     our_driver_set_value_t set_custom_value;
// };

// static inline int our_driver_set_value(
//     const struct device *dev,
//     int value)
// {
//     const struct our_driver_api *api =
//         DEVICE_API_GET(our_driver, dev);

//     return api->set_custom_value(dev, value);
// }