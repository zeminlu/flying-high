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
CFLAGS = -Wall -Wuninitialized -errors -fno-builtin -fno-stack-protector -c -o
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
OBJECTS1 = kstart.o libc.o stdio.o libasm.o kernel.o unistd.o string.o syscall.o io.o video_driver.o keyboard_driver.o int_handlers.o queue.o shell.o system.o int80.o process.o memModule.o sysProcess.o kMalloc.o sysMalloc.o uMalloc.o exceptions.o kernelDepth.o schedule.o ttys.o bin.o semaphore.o shMemory.o rand.o bttlship.o cleanup.o attch.o semop.o game.o
# mouse_driver.o
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

libc.o: libc.c libc.h

unistd.o: unistd.asm sys.inc

syscall.o: syscall.c syscall.h

kernel.o: kernel.c kernel.h

file.o: file.c file.h

io.o: io.asm io.h 

video_driver.o: video_driver.c video_driver.h 

int_handlers.o: int_handlers.asm int_handlers.h

mouse_driver.o: mouse_driver.c mouse_driver.h

keyboard_driver.o: keyboard_driver.c keyboard_driver.h

queue.o: queue.c queue.h

video_adapter.o: video_adapter.c video_adapter.h

stdio.o: stdio.c stdio.h

keyboard_adapter.o: keyboard_adapter.c keyboard_adapter.h

shell.o: shell.c shell.h

screensaver.o: screensaver.c screensaver.h 

system.o: system.c system.h

timerTick.o: timerTick.c timerTick.h

memModule.o: memModule.c memModule.h

process.o: process.c process.h

sysProcess.o: sysProcess.c sysProcess.h

int80.o: int80.asm syscall.h

kMalloc.o: kMalloc.c kMalloc.h

sysMalloc.o: sysMalloc.c sysMalloc.h

uMalloc.o: uMalloc.c uMalloc.h

exceptions.o: exceptions.c exceptions.h

kernelDepth.o: kernelDepth.c kernelDepth.h

schedule.o: schedule.c schedule.h

ttys.o: ttys.c ttys.h

bin.o: bin.c bin.h

semaphore.o: semaphore.c semaphore.h

shMemory.o: shMemory.c shMemory.h	

attch.o: attch.c bttlship.h

semop.o: semop.c bttlship.h

game.o: game.c bttlship.h

cleanup.o: cleanup.c bttlship.h

bttlship.o: bttlship.c bttlship.h

rand.o: rand.h rand.c

clean:
	@echo "Clearing" $(OUTPUT_DIR) "directory..."
	@rm -f *.o
	@rm -f $(OUTPUT_DIR)*
	@echo "Done."
