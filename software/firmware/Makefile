ROOT_DIR=../..
include ../software.mk

#additional compiler flags
CFLAGS+=--specs=nosys.specs -Wl,-Bstatic,-T,../template.lds,-Map,firmware.map,--strip-debug

#additional linker flag
LFLAGS+=-Wl,-Map,firmware.map

#cache
ifeq ($(USE_DDR),1)
include $(CACHE_DIR)/software/software.mk
endif

#uart
include $(UART_DIR)/software/embedded/embedded.mk

#define
ifeq ($(DEBUG),1)
DEFINE+=-DDEBUG
endif

#HEADERS
HDR+=periphs.h \
*.h

#SOURCES
SRC+= firmware.S $(UART_DIR)/software/printf.c \
*.c

#RULES

build: firmware.elf

firmware.elf: ../template.lds $(HDR) $(SRC)
	$(TOOLCHAIN_PREFIX)gcc -o $@ $(CFLAGS) $(LFLAGS) $(DEFINE) $(INCLUDE) $(SRC) $(LLIBS)
	$(TOOLCHAIN_PREFIX)objcopy -O binary firmware.elf firmware.bin

clean:
	@rm -rf firmware.bin firmware.elf firmware.map *.hex periphs.h *swreg.h \
		*swreg_emb.c

.PHONY: build clean
