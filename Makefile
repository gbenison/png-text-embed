CFLAGS=`pkg-config --cflags libpng` -g
LIBS=`pkg-config --libs libpng`

.PHONY: all
all: png-text-append png-text-dump

png-text-append: png-text-append.o crc.o
	$(CC) -o$@ $^

png-text-dump: png-text-dump.o
	$(CC) $(LIBS) -o$@ $^


