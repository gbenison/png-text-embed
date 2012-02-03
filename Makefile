CFLAGS=`pkg-config --cflags libpng` -g
LIBS=`pkg-config --libs libpng`

.PHONY: all
all: png-text-append png-text-dump png-dump-chunks

png-text-append: png-text-append.o
	$(CC) -o$@ $^

png-text-dump: png-text-dump.o
	$(CC) $(LIBS) -o$@ $^

png-dump-chunks: png-dump-chunks.o
	$(CC) -o$@ $^

