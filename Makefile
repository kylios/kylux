# Makefile for kylux

DIRS			=	. lib lib/kernel lib/user kernel arch arch/x86 test test/kernel util

# ./
SRC				=	src/main.o \
					src/boot.o \
					src/debug.o

# lib/
LIB_SRC			=	src/lib/stdio.o \
					src/lib/stdlib.o \
					src/lib/string.o

# lib/user/
LIB_USER_SRC	=

# lib/kernel/
LIB_KERNEL_SRC	=	src/lib/kernel/framebuf.o \
                    src/lib/kernel/bitmap.o \
					src/lib/kernel/list.o \
					src/lib/kernel/hash.o \
                    src/lib/kernel/kmalloc.o    \
                    src/lib/kernel/test.o

# arch/
ARCH_SRC		=

# arch/x86/
ARCH_X86_SRC	=	src/arch/x86/framebuf.o \
					src/arch/x86/io_bus.o	\
					src/arch/x86/idt.o	\
					src/arch/x86/isr.o	\
					src/arch/x86/gdt.o	\
					src/arch/x86/gdt_asm.o	\
					src/arch/x86/interrupt.o \
                    src/arch/x86/atomic.o \
					src/arch/x86/timer.o \
					src/arch/x86/pagedir.o \
                    src/arch/x86/thread.o

# kernel/
KERNEL_SRC		=   src/kernel/spinlock.o \
					src/kernel/semaphore.o \
					src/kernel/lock.o \
					src/kernel/page_mgr.o \
                    src/kernel/frame_mgr.o \
                    src/kernel/pagemap.o    \
					src/kernel/paging.o \
                    src/kernel/process.o \
                    src/kernel/thread.o \
                    src/kernel/interrupt.o

# test/
TEST_SRC        =   src/test/test.o

# test/kernel/
TEST_KERNEL_SRC	=	src/test/kernel/hash.o  \
					src/test/kernel/list.o

# All of our source files
SOURCES			= 	$(SRC)	\
					$(LIB_SRC)	\
					$(LIB_KERNEL_SRC)	\
					$(ARCH_X86_SRC)		\
                    $(KERNEL_SRC)   \
					$(TEST_SRC)		\
					$(TEST_KERNEL_SRC)	

 #    -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align   \
#    -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
#    -Wredundant-decls -Wnested-externs -Winline -Wno-long-long  \
#    -Wconversion -Wstrict-prototypes    \

CFLAGS= -Wall   \
	-Iinclude/ -Linclude/lib/kernel/ \
    -nostdlib -nostdinc -fno-builtin -fno-stack-protector   \
    -Winline
# These flags are enabled when `make test' is invoked
TESTFLAGS=-D WITH_ASSERTS -g
LDFLAGS=-Tlink.ld
ASFLAGS=-felf


all: $(SOURCES) link 

test:
	CFLAGS += $(TESTFLAGS)
	clean 
	all 

clean:
	for d in $(DIRS); do -rm src/$$d/*.o 2>/dev/null; -rm src/$$d/.*.swo 2>/dev/null; -rm src/$$d/.*.swp 2>/dev/null; done
	for d in $(DIRS); do rm -include/$$d/.*.swo 2>/dev/null; -rm include/$$d/.*.swp 2>/dev/null; done
	-rm .*.swp 2>/dev/null
	-rm .*.swo 2>/dev/null
	-rm kernel 2>/dev/null
	-rm *.i 2>/dev/null
	-rm *.s 2>/dev/null

kylux:		FORCE
	sudo make reset && make && make install && make run

# Creates a loopback, causing all writes to /dev/loop0 to go to 
# floppy.img.  Then /dev/loop0 is mounted to install/ where the binary
# is written.
install:	FORCE# reset
	sudo losetup /dev/loop0 floppy.img
	sudo mount /dev/loop0 install
	sudo cp kernel install/kernel
	sudo umount /dev/loop0
	sudo losetup -d /dev/loop0

# Force something to make
FORCE:

# Sometimes the loop fails to get destroyed due to sudo timing out after
# the VM has been running for a while.  If this happens, execute this
# command to reset it.
reset:
	-sudo umount install
	-sudo losetup -d /dev/loop0

# Runs the kernel by creating a loop on /dev/loop0 with floppy.img.  Our
# bochsrc.txt file indicates that bochs should read from /dev/loop0 as a
# floppy disk to find the kernel.
run:
	sudo losetup /dev/loop0 floppy.img
	sudo bochs -q -f ./bochsrc.txt
#	qemu -fda floppy.img
	sudo losetup -d /dev/loop0

link:
	ld $(LDFLAGS) -o kernel $(SOURCES)

.s.o:
	nasm $(ASFLAGS) $< 

