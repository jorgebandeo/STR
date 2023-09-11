CC = gcc
CFLAGS =  -lm

TARGET = fda
SRCS = pgmtolist.c pgmfiles.c diff2d.c main.c

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: clean
