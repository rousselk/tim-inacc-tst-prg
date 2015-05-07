/**
 * \file
 *         Test noeud simple émetteur sous ContikiMAC
 * \author
 *         K. Roussel
 */

#include "contiki.h"
#include "net/netstack.h"

#include <stdio.h>
#include <stdlib.h>

/*---------------------------------------------------------------------------*/

#define ROUTER_NODE_ADDR 0x0001   // address of the router in the PAN
#define DATA_SIZE 110
#define DELAY_mS 500
#define TOTAL_PACKETS_NUMBER 50U

/*---------------------------------------------------------------------------*/

extern unsigned short node_id;   /* ID de noeud attribué par Cooja */


static unsigned char pkt_payload[DATA_SIZE];

volatile unsigned int pkt_num = 0;

/*---------------------------------------------------------------------------*/
static struct etimer et;
/*---------------------------------------------------------------------------*/
PROCESS(test_contikimac_node_process, "Test ContikiMAC emitter node");
AUTOSTART_PROCESSES(&test_contikimac_node_process);
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(test_contikimac_node_process, ev, data)
{
  PROCESS_BEGIN();

  printf("node_id == %d.\n", node_id);
  unsigned rseed = node_id + 12345U;
  srand(rseed);

  NETSTACK_RADIO.init();
  NETSTACK_RADIO.on();


  while (pkt_num <= TOTAL_PACKETS_NUMBER) {
    int res;

    pkt_num++;

    unsigned long delay = rand();
    delay %= DELAY_mS;
    delay += (DELAY_mS / 2);
    etimer_set(&et, CLOCK_SECOND * delay / 1000);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    sprintf((char *) pkt_payload,
            "[node %2u] S-CoSenS test, packet #%03u.",
            node_id, pkt_num);

    rtimer_clock_t debut = RTIMER_NOW();

    res = NETSTACK_RADIO.prepare(pkt_payload, DATA_SIZE);
    if (res != RADIO_TX_OK) {
        printf("*** ERREUR : impossible de préparer le packet n° %d!\n",
               pkt_num);
        printf("*** Passage au paquet suivant...\n\n");
    }

    rtimer_clock_t fin = RTIMER_NOW();

    printf("%u\n", (fin - debut));

    NETSTACK_RADIO.transmit(DATA_SIZE);
  }

  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
