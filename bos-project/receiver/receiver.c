#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "packetbuf.h"
#include <string.h>
#include <stdio.h>

#include "dev/serial-line.h"
#include "dev/leds.h"

#include "sys/log.h"
#define LOG_MODULE "Receiver"
#define LOG_LEVEL LOG_LEVEL_INFO

PROCESS(receiver_node, "Receiver");
AUTOSTART_PROCESSES(&receiver_node);

void input_callback(const void *data, uint16_t len,
                    const linkaddr_t *src, const linkaddr_t *dest)
{
    if (len % sizeof(unsigned int) == 0) {
        size_t num_count = len / sizeof(unsigned int);
        unsigned int numbers[num_count];
        memcpy(numbers, data, len);

        LOG_INFO_("Received %u numbers from ", num_count);
        LOG_INFO_LLADDR(src);
        LOG_INFO_(": ");
        for (size_t i = 0; i < num_count; i++) {
            LOG_INFO_("%u ", numbers[i]);
        }
        LOG_INFO_("\n");
    } else {
        LOG_ERR("Invalid packet received from ");
        LOG_ERR_LLADDR(src);
        LOG_ERR_("\n");
    }
}

/*--------------------------------------------------------------------------------------------------------------------*/
PROCESS_THREAD(receiver_node, ev, data) {

    PROCESS_BEGIN();

    leds_on(LEDS_RED);
    leds_off(LEDS_YELLOW);

    nullnet_set_input_callback(input_callback);

    while (1) {
        PROCESS_WAIT_EVENT();
    }
    PROCESS_END();
}