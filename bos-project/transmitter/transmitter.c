#include "contiki.h"
#include "net/netstack.h"
#include "net/nullnet/nullnet.h"
#include "packetbuf.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "dev/serial-line.h"
#include "dev/leds.h"

#include "sys/log.h"

#define LOG_MODULE "Transmitter"
#define LOG_LEVEL LOG_LEVEL_INFO

/* Configuration */
#define SEND_INTERVAL (CLOCK_SECOND * 2)
#define MAX_NUMBERS 4
#define MAX_BUFFER_SIZE 32

PROCESS(transmitter, "Transmitter");
AUTOSTART_PROCESSES(&transmitter);

static void parse_and_transmit(const char *input) {
    unsigned int numbers[MAX_NUMBERS];
    size_t num_count = 0;
    char *token;
    char buffer[MAX_BUFFER_SIZE];

    // Copy the input string to a buffer
    strncpy(buffer, input, MAX_BUFFER_SIZE);

    // Tokenize the input string
    token = strtok(buffer, ",");
    while(token != NULL && num_count < MAX_NUMBERS) {
        numbers[num_count++] = atoi(token); // Convert the token to an integer
        token = strtok(NULL, ",");
    }

    // Transmit the numbers
    if(num_count > 0) {
        nullnet_buf = (uint8_t *)numbers;
        nullnet_len = num_count * sizeof(unsigned int);

        NETSTACK_NETWORK.output(NULL);
        LOG_INFO_("Sent %u numbers -> ", num_count);
        for (size_t i = 0; i < num_count; i++) {
            LOG_INFO_("%u ", numbers[i]);
        }
        LOG_INFO_("\n");
    } else {
        LOG_ERR("Invalid input, please enter a valid list of numbers.\n");
    }
}

/*---------------------------------------------------------------------------*/
PROCESS_THREAD(transmitter, ev, data) {
    PROCESS_BEGIN();
    leds_off(LEDS_GREEN);

    while (1) {
        PROCESS_YIELD();
        if(ev == serial_line_event_message) {
            char *input = (char *)data;
            LOG_INFO("Received input: %s\n", input);
            parse_and_transmit(input);
        }
    }
    // static unsigned int number_to_send;
    // static char *end_ptr;

    // PROCESS_BEGIN();
    // leds_off(LEDS_GREEN);

    // while (1) {
    //     PROCESS_WAIT_EVENT();
    //     if(ev == serial_line_event_message) {
    //         // Parse the incoming string as an unsigned integer
    //         number_to_send = strtoul((char *)data, &end_ptr, 10);

    //         if(end_ptr != (char *)data) {  // Check if the conversion was successful
    //             LOG_INFO("Received number: %u\n", number_to_send);

    //             // Prepare data to be sent
    //             nullnet_buf = (uint8_t *)&number_to_send;
    //             nullnet_len = sizeof(number_to_send);

    //             // Transmit the number
    //             NETSTACK_NETWORK.output(NULL);

    //             LOG_INFO("Sent number: %u\n", number_to_send);
    //         } else {
    //             LOG_ERR("Invalid input, please enter a valid number.\n");
    //         }
    //     }
    // }
    PROCESS_END();
}