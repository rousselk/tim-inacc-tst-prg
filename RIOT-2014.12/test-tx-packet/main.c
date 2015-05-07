/**
 * Simple 802.15.4 transmitter node test application under RIOT OS.
 *
 * Copyright (C) 2015, Kévin Roussel <Kevin.Roussel@inria.fr>
 *
 * This file is subject to the terms and conditions of the GNU Lesser General
 * Public License. See the file LICENSE in the top level directory for more
 * details.
 */


#include <stdio.h>
#include <stdbool.h>

#include "hwtimer.h"
#include "crash.h"
#include "random.h"

#define ENABLE_DEBUG 0
#include "debug.h"

#include "cc2420.h"


#define MY_PAN_ID 0x1234          // current PAN ID
#define ROUTER_NODE_ADDR 0x0001   // address of the router in the PAN
#define DATA_SIZE 110
#define DELAY_uS 500000
#define TOTAL_PACKETS_NUMBER 50U

#define BOARD_RADIO_DRIVER  (cc2420_radio_driver)


unsigned short node_id = 0;   /* node ID (re-initialized by Cooja) */


static unsigned char pkt_payload[DATA_SIZE];

volatile unsigned int pkt_num = 0;


int main(void)
{
    hwtimer_init();

    printf("node_id == %d.\n", node_id);
    uint32_t rseed = node_id + 1234567890UL;
    genrand_init(rseed);

    /* directly init the radio, and keep it on */
    BOARD_RADIO_DRIVER.init();
    BOARD_RADIO_DRIVER.set_promiscuous_mode(false);
    BOARD_RADIO_DRIVER.set_pan_id(MY_PAN_ID);
    BOARD_RADIO_DRIVER.set_address(node_id);
    bool ok = BOARD_RADIO_DRIVER.on();
    if (!ok) {
        core_panic(0xFFFF,
                   "COULD NOT START RADIO TRANSCEIVER");
    }

    ieee802154_node_addr_t dest = {
        .pan.addr = ROUTER_NODE_ADDR,
        .pan.id = MY_PAN_ID
    };

    while (pkt_num < TOTAL_PACKETS_NUMBER) {
        pkt_num++;

        unsigned long delay = genrand_uint32();
        delay %= DELAY_uS;
        delay += (DELAY_uS / 2);
        hwtimer_wait(HWTIMER_TICKS(delay));

        sprintf((char *) pkt_payload,
                "[node %2u] transmission test, packet #%03u.",
                node_id, pkt_num);

        unsigned long debut = hwtimer_now();

        int res = BOARD_RADIO_DRIVER.load_tx(PACKET_KIND_DATA,
                                             dest,
                                             false,
                                             false,
                                             (void*)pkt_payload,
                                             DATA_SIZE);
        if (res != RADIO_TX_OK) {
            printf("*** ERREUR : impossible de préparer le packet n° %d!\n",
                   pkt_num);
            printf("*** Passage au paquet suivant...\n\n");
            continue;
        }

        unsigned long fin = hwtimer_now();
        printf("%lu\n", (fin - debut));

        BOARD_RADIO_DRIVER.transmit();
    }

    return 0;
}

