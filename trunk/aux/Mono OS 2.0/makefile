############################### Variables ######################################
#
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
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -nostartfiles -c -o 
#CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra  -nostartfiles -nodefaultlibs -DDEBUG_LEVEL=6  -c -o
#CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra  -nostartfiles -nodefaultlibs -DDEBUG_LEVEL=3  -c -o
#CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra  -nostartfiles -nodefaultlibs -DDEBUG_PAUSE=3 -c -o
#CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -Wall -Wextra  -nostartfiles -nodefaultlibs -DDEBUG_LEVEL=3 -DDEBUG_PAUSE=3 -c -o
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
#LDFLAGS = -melf_i386 -T $(SRC_DIR)link.ld --print-map -o
LDFLAGS = -melf_i386 -T $(SRC_DIR)link.ld -o
#
TARGET1 = $(OUTPUT_DIR)kernel.bin
OBJECTS1 = kernel.o kstart.o libc.o libasm.o fdTable.o primitives.o systemCalls.o intHandlers.o convertions.o mouseInit.o mouse.o portsHandler.o stdio.o string.o controllers.o videoDriver.o KBDriver.o mouseControler.o screenSaver.o clipboardManager.o stdlib.o shell.o cmdDispatch.o memmanager.o exceptions.o genShellFunc.o dirShellFunc.o sysfilesystem.o debug.o filesystem.o callint80.o process.o sysprocess.o fileShellFunc.o miscShellFunc.o newsyscall.o basicShellFunc.o tty.o kernelBridge.o baseMalloc.o bin.o alloc.o pQueue.o
###############################################################################
.SILENT:
.PHONY: clean mcopy configure

all: $(TARGET1)

$(TARGET1): $(addprefix $(OUTPUT_DIR), $(OBJECTS1))
	@echo "Linking" $@"..."
	$(LD) $(LDFLAGS) $@ $^ 
	@echo "Done."
	@echo "Copying binary file to a:boot..."
	@mcd a:
	@mcopy -D o $(TARGET1) a:boot
	@echo "Done."
	
mcopy:	
	@echo "Copying binary file to a:boot..."
	@mcd a:
	@mcopy -D o $(TARGET1) a:boot
	@echo "Done."

configure:
	@echo "Copying mtools.conf to /etc/..."
	@cp ./mtools.conf /etc/
	@echo "Done."

$(OUTPUT_DIR)%.o: %.c
	@echo "Compiling" $< "into" $@...
	$(COMPILE.c) $@ $<
	@echo "Done."

$(OUTPUT_DIR)%.o: %.asm
	@echo "Compiling" $< "into" $@...
	$(COMPILE.asm) $@ $<
	@echo "Done."

clean:
	@echo "Clearing" $(OUTPUT_DIR) "directory..."
	@rm -f *.o
	@rm -f $(OUTPUT_DIR)*
	@echo "Done."

kernel.o: kernel.h

debug.o: debug.h stdio.h

memmanager.o: memmanager.h stdio.h string.h debug.h

exceptions.h: exceptions.h kasm.h idt.h stdio.h 

kernel.o: intHandlers.h videoDriver.h mouseInit.h shell.h screenSaver.h memmanager.h idt.h exceptions.h

sysprocess.o: process.h sysprocess.h memmanager.h string.h sysfilesystem.h debug.h 

process.o: process.h unistd.h

dirFileFunc.o: dirFileFunc.h
