############################### Variables ######################################
#
#Sets working directories
INC_DIR = ./inc/
SRC_DIR = ./src/
OUTPUT_DIR = ./bin/
#
#- Global Variables
#Sets the location of the header files for make
vpath %.h $(INC_DIR)
vpath %.inc $(INC_DIR)
#Set the location of the source files for make
vpath %.c $(SRC_DIR)
vpath %.asm $(SRC_DIR)
#
#
#- Compiler
#Sets the C compiler used to create object files from source code.
CC = gcc
#Sets compiler options.
CFLAGS = -O -Wall -Wuninitialized -pedantic -errors -fno-builtin -c -o
#Sets the location of the header files.
INCLUDES = -I $(INC_DIR)
#Sets the C compiler set up
COMPILE.c = $(CC) $(INCLUDES) $(CFLAGS) 
#
#Sets the x86 Assembler compiler used to create object files from source code.
ASMC = nasm
#Sets compiler options
ASMCFLAGS = -f aout -o
#Sets the Assembler compiler set up
COMPILE.asm = $(ASMC) $(INCLUDES) $(ASMCFLAGS)
#
#Sets the linker for the project
LD = ld
#
LDFLAGS = -T $(SRC_DIR)link.ld -o
#
TARGET1 = kernel.bin
OBJECTS1 = kstart.o libc.o stdio.o libasm.o kernel.o unistd.o string.o syscall.o io.o video_driver.o mouse_driver.o keyboard_driver.o keyboard_adapter.o int_handlers.o queue.o video_adapter.o file.o shell.o system.o screensaver.o timerTick.o int80.o process.o memModule.o sysProcess.o
###############################################################################
.SILENT:
.PHONY: clean mcopy

all: $(TARGET1)

$(TARGET1): $(OBJECTS1)
	@echo "Linking" $@"..."
	$(LD) $(LDFLAGS) $(OUTPUT_DIR)$@ $^ 
	@echo "Done."
	
mcopy:	
	@echo "Copying binary file to a:boot..."
	@mcd a:
	@mcopy -D o $(OUTPUT_DIR)$(TARGET1) a:boot
	@echo "Done."

%.o: %.c
	@echo "Compiling" $< "into" $@...
	$(COMPILE.c) $@ $<
	@echo "Done."

%.o: %.asm
	@echo "Compiling" $< "into" $@...
	$(COMPILE.asm) $@ $<
	@echo "Done."

libc.o: libc.c kc.h

unistd.o: unistd.asm sys.inc

syscall.o: syscall.c file.h system.h sysProcess.h memModule.h

kernel.o: kernel.c kc.h kasm.h system.h int_handlers.h unistd.h video_driver.h mouse_driver.h keyboard_driver.h config.h stdio.h string.h shell.h memModule.h 

file.o: file.c file.h system.h types.h config.h

io.o: io.asm io.h 

video_driver.o: video_driver.c video_driver.h config.h io.h string.h 

int_handlers.o: int_handlers.asm int_handlers.h

mouse_driver.o: mouse_driver.c mouse_driver.h video_driver.h config.h

keyboard_driver.o: keyboard_driver.c keyboard_driver.h io.h queue.h keyboard_buffer.h config.h

queue.o: queue.c queue.h keyboard_buffer.h

video_adapter.o: video_adapter.c video_adapter.h video_driver.h stdio.h

stdio.o: stdio.c stdio.h

keyboard_adapter.o: keyboard_adapter.c keyboard_adapter.h stdio.h video_driver.h queue.h

shell.o: shell.c shell.h video_driver.h system.h stdio.h config.h unistd.h string.h colors.h mouse_driver.h

screensaver.o: screensaver.c screensaver.h config.h video_driver.h io.h mouse_driver.h system.h 

system.o: system.c system.h stdio.h io.h shell.h screensaver.h

timerTick.o: timerTick.c timerTick.h system.h video_adapter.h keyboard_adapter.h keyboard_driver.h mouse_driver.h screensaver.h

memModule.o: memModule.c memModule.h

process.o: process.c process.h int80.h syscall.h string.h types.h sysProcess.h

sysProcess.o: sysProcess.c sysProcess.h keyboard_driver.h config.h

int80.o: int80.asm syscall.h


clean:
	@echo "Clearing" $(OUTPUT_DIR) "directory..."
	@rm -f *.o
	@rm -f $(OUTPUT_DIR)*
	@echo "Done."
