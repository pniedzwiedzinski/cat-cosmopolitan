CC=gcc
CC_OPTS=-g -Os -static -nostdlib -nostdinc -fno-pie -no-pie -mno-red-zone -fno-omit-frame-pointer -pg -mnop-mcount
INCLUDE=-fuse-ld=bfd -Wl,-T,ape/ape.lds -include ape/cosmopolitan.h ape/crt.o ape/ape.o ape/cosmopolitan.a

all: cat.com

cat.com.dbg: cat.c
	$(CC) $(CC_OPTS) cat.c -o cat.com.dbg $(INCLUDE)

cat.com: cat.com.dbg
	objcopy -S -O binary cat.com.dbg cat.com

clean:
	rm cat.com cat.com.dbg
