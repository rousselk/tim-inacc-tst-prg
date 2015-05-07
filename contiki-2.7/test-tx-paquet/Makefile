CONTIKI = ../..

NETSTACK_CONF_RDC=nullrdc_driver
NETSTACK_CONF_MAC=nullmac_driver

ifndef TARGET
TARGET=z1
endif

CONTIKI_PROJECT = test-tx-paquet
CONTIKI_SOURCEFILES += cc2420-arch.c ### sensors.c sht11.c
#PROJECT_SOURCEFILES =
all: $(CONTIKI_PROJECT)

include $(CONTIKI)/Makefile.include


