.PHONY: all clean run

all:
	riscv64-unknown-elf-as -o main.o test_cache.S
	riscv64-unknown-elf-ld -Map=main.map -T link.ld -o main main.o

clean:
	rm *.o *.map main

run: all
	spike main
