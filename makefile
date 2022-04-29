# Makefile
all: diskimage bootloader kernel shell utils

# Recipes
diskimage:
	dd if=/dev/zero of=out/system.img bs=512 count=2880

bootloader:
	nasm src/asm/bootloader.asm -o out/bootloader
	dd if=out/bootloader of=out/system.img bs=512 count=1 conv=notrunc

kernel:
	bcc -ansi -c -o out/kernel.o src/c/kernel.c
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c
	bcc -ansi -c -o out/fileio.o src/c/fileio.c
	bcc -ansi -c -o out/textio.o src/c/textio.c
	bcc -ansi -c -o out/string.o src/c/string.c
	bcc -ansi -c -o out/message.o src/c/message.c
	nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	nasm -f as86 src/asm/utils.asm -o out/lib_utils.o
	ld86 -o out/kernel -d out/kernel.o out/kernel_asm.o out/lib_interrupt.o out/lib_utils.o out/std_lib.o out/fileio.o out/textio.o out/string.o out/message.o
	dd if=out/kernel of=out/system.img bs=512 conv=notrunc seek=1

shell:
	bcc -ansi -c -o out/shell.o src/c/shell.c
	bcc -ansi -c -o out/textio.o src/c/textio.c
	bcc -ansi -c -o out/fileio.o src/c/fileio.c
	bcc -ansi -c -o out/string.o src/c/string.c
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c
	bcc -ansi -c -o out/message.o src/c/message.c
	bcc -ansi -c -o out/program.o src/c/program.c
	nasm -f as86 src/asm/interrupt.asm -o out/lib_interrupt.o
	nasm -f as86 src/asm/utils.asm -o out/lib_utils.o
	ld86 -o out/shell -d out/shell.o out/lib_interrupt.o out/lib_utils.o out/std_lib.o out/fileio.o out/textio.o out/string.o out/message.o out/program.o

utils: ls cat mkdir mv cd cp

ls:
	bcc -ansi -c -o out/ls.o src/c/ls.c
	bcc -ansi -c -o out/textio.o src/c/textio.c
	bcc -ansi -c -o out/fileio.o src/c/fileio.c
	bcc -ansi -c -o out/string.o src/c/string.c
	bcc -ansi -c -o out/message.o src/c/message.c
	bcc -ansi -c -o out/program.o src/c/program.c
	nasm -f as86 src/asm/utils.asm -o out/lib_utils.o
	ld86 -o out/ls -d out/ls.o out/lib_interrupt.o out/lib_utils.o out/std_lib.o out/fileio.o out/textio.o out/string.o out/message.o out/program.o

cat:
	bcc -ansi -c -o out/cat.o src/c/cat.c
	bcc -ansi -c -o out/textio.o src/c/textio.c
	bcc -ansi -c -o out/fileio.o src/c/fileio.c
	bcc -ansi -c -o out/string.o src/c/string.c
	bcc -ansi -c -o out/message.o src/c/message.c
	bcc -ansi -c -o out/program.o src/c/program.c
	nasm -f as86 src/asm/utils.asm -o out/lib_utils.o
	ld86 -o out/cat -d out/cat.o out/lib_interrupt.o out/lib_utils.o out/std_lib.o out/fileio.o out/textio.o out/string.o out/message.o out/program.o

mkdir:
	bcc -ansi -c -o out/mkdir.o src/c/mkdir.c
	bcc -ansi -c -o out/textio.o src/c/textio.c
	bcc -ansi -c -o out/fileio.o src/c/fileio.c
	bcc -ansi -c -o out/string.o src/c/string.c
	bcc -ansi -c -o out/message.o src/c/message.c
	bcc -ansi -c -o out/program.o src/c/program.c
	nasm -f as86 src/asm/utils.asm -o out/lib_utils.o
	ld86 -o out/mkdir -d out/mkdir.o out/lib_interrupt.o out/lib_utils.o out/std_lib.o out/fileio.o out/textio.o out/string.o out/message.o out/program.o

cd:
	bcc -ansi -c -o out/cd.o src/c/cd.c
	bcc -ansi -c -o out/textio.o src/c/textio.c
	bcc -ansi -c -o out/fileio.o src/c/fileio.c
	bcc -ansi -c -o out/string.o src/c/string.c
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c
	bcc -ansi -c -o out/message.o src/c/message.c
	bcc -ansi -c -o out/program.o src/c/program.c
	nasm -f as86 src/asm/utils.asm -o out/lib_utils.o
	ld86 -o out/cd -d out/cd.o out/lib_interrupt.o out/lib_utils.o out/std_lib.o out/fileio.o out/textio.o out/string.o out/message.o out/program.o

mv:
	bcc -ansi -c -o out/mv.o src/c/mv.c
	bcc -ansi -c -o out/textio.o src/c/textio.c
	bcc -ansi -c -o out/fileio.o src/c/fileio.c
	bcc -ansi -c -o out/string.o src/c/string.c
	bcc -ansi -c -o out/message.o src/c/message.c
	bcc -ansi -c -o out/program.o src/c/program.c
	nasm -f as86 src/asm/utils.asm -o out/lib_utils.o
	ld86 -o out/mv -d out/mv.o out/lib_interrupt.o out/lib_utils.o out/std_lib.o out/fileio.o out/textio.o out/string.o out/message.o out/program.o
	
cp:
	bcc -ansi -c -o out/cp.o src/c/cp.c
	bcc -ansi -c -o out/textio.o src/c/textio.c
	bcc -ansi -c -o out/fileio.o src/c/fileio.c
	bcc -ansi -c -o out/string.o src/c/string.c
	bcc -ansi -c -o out/std_lib.o src/c/std_lib.c
	bcc -ansi -c -o out/message.o src/c/message.c
	bcc -ansi -c -o out/program.o src/c/program.c
	nasm -f as86 src/asm/utils.asm -o out/lib_utils.o
	ld86 -o out/cp -d out/cp.o out/lib_interrupt.o out/lib_utils.o out/std_lib.o out/fileio.o out/textio.o out/string.o out/message.o out/program.o

run:
	bochs -f src/config/if2230.config

build-run: all run

tc:
	cd out && ./tc_gen S

tc-2:
	cd out && ./tc_gen B

tc-3:
	cd out && ./tc_gen C

tc-4:
	cd out && ./tc_gen D

tcA: tc run

tcB: tc-2 run

tcC: tc-3 run

tcD: tc-4 run
