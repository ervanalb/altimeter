# Makefile for STM32F373 using cmsis and GNU toolchain.

# Files to include
C_SRC  = $(wildcard src/*.c)
C_SRC += $(wildcard lib/STM32F0xx_StdPeriph_Driver/src/*.c)
C_SRC += $(wildcard lib/lux/src/*.c)

OBJECTS = $(patsubst %.c,%.o,$(C_SRC)) src/startup.o

LIBRARIES = 

INC  = -Iinc
INC += -Ilib/CMSIS/Include -Ilib/CMSIS/Device/ST/STM32F0xx/Include
INC += -Ilib/STM32F0xx_StdPeriph_Driver/inc

# Assembler, compiler, and linker flags
AFLAGS  = -mlittle-endian -mthumb -mcpu=cortex-m0 -g
CFLAGS  = $(AFLAGS) -O0 $(INC) -std=c99 -DUSE_STDPERIPH_DRIVER -Wall
CFLAGS += -DSTM32F031
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -fdata-sections -ffunction-sections
LFLAGS  = $(CFLAGS) -nostartfiles -Wl,--gc-sections

# Targets
all: main.elf
clean:
	-rm -f $(OBJECTS) *.elf *.bin *.map *.hex
main.hex: main.elf
	arm-none-eabi-objcopy -R .config main.elf -O ihex main.hex
main.bin: main.elf
	arm-none-eabi-objcopy -R .config -O binary main.elf main.bin
main.elf: $(OBJECTS)
	arm-none-eabi-gcc $(LFLAGS) -Tmain.ld -Wl,-Map=main.map -o main.elf $(OBJECTS) $(LIBRARIES)
src/startup.o: src/startup.s
	arm-none-eabi-as $(AFLAGS) src/startup.s -o src/startup.o
lib/%.o: lib/%.c
	arm-none-eabi-gcc $(CFLAGS) -c -o $@ $<
%.o: %.c
	arm-none-eabi-gcc $(CFLAGS) -c -o $@ $<
gdb: main.elf
	arm-none-eabi-gdb main.elf -x init.gdb
load: main.elf
	arm-none-eabi-gdb main.elf -x init.gdb -ex load
loadquit: main.elf
	arm-none-eabi-gdb main.elf -batch -x init.gdb -ex load -ex kill -ex quit
stlink:
	st-util -p 4244
stload: main.bin
	st-flash write main.bin 0x8000000
sterase:
	st-flash erase
