#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

int foo(const struct device *dev);

#ifdef __cplusplus
}
#endif

#endif