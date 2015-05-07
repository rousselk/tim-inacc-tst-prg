export APPLICATION = test_tx_packet

BOARD_WHITELIST := z1 telosb

ifeq ($(strip $(BOARD)),)
	export BOARD = z1
endif

# this has to be the absolute path of the RIOT-base dir
export RIOTBASE = $(CURDIR)/../../

QUIET = 1

## Modules to include.

USEMODULE += random
USEMODULE += cc2420

INCLUDES += -I$(RIOTBASE)/sys/include \
            -I$(RIOTBASE)/drivers/include \
            -I$(RIOTBASE)/drivers/cc2420/include

include $(RIOTBASE)/Makefile.include

