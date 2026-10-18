#include <zephyr/kernel.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>
//#include <zephyr/logging/log.h>
#include<zephyr/shell/shell.h>

static const struct device *sensor_dev = DEVICE_DT_GET(DT_NODELABEL(our_driver0));
//#define SENSOR_NODE DT_NODELABEL(our_driver0)
struct sensor_value val;
static int cmd_sensor_info(const struct shell *shell, size_t argc, char **argv){
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    shell_print(shell, "Device Name: %s", sensor_dev->name);
    shell_print(shell, "Device Status: %s", device_is_ready(sensor_dev) ? "ready" : "not ready");
    return 0;
}
static int cmd_sample_fetch(const struct shell *shell, size_t argc, char **argv){
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    int ret = sensor_sample_fetch(sensor_dev);
    if(ret < 0){
        shell_error(shell, "sensor_sample_fetch failed with error %d", ret); 
        return ret;
        }
    return 0;
}
static int cmd_sample_get(const struct shell *shell, size_t argc, char **argv){
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);
    int ret = sensor_channel_get(sensor_dev, SENSOR_CHAN_AMBIENT_TEMP, &val);
    if(ret < 0){
            shell_error(shell, "sensor_channel_get failed with error %d", ret);
        } 
    shell_print(shell, "Ambient Temperature: %d.%06d C", val.val1, val.val2);
    return 0;
}
// static int cmd_channel_get_handler(const struct shell *shell, size_t argc, char **argv){
//     ARG_UNUSED(argc);
//     ARG_UNUSED(argv);
//     shell_info(shell, "Hello from channel get command");
//     return 0;
// }

SHELL_STATIC_SUBCMD_SET_CREATE(sub_demo, 
    //SHELL_CMD_ARG(channel_get, NULL, "Get channel value", cmd_channel_get_handler, 1, 0),
    SHELL_CMD(info, NULL, "Get sensor info", cmd_sensor_info),
    SHELL_CMD(sample_fetch, NULL, "Fetch sensor sample", cmd_sample_fetch),
    SHELL_CMD(sample_get, NULL, "Get sensor sample", cmd_sample_get),
    SHELL_SUBCMD_SET_END
);
SHELL_CMD_REGISTER(demo, &sub_demo, "Our driver commands", NULL);
