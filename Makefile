CC = gcc
CFLAGS = $(shell pkg-config --cflags libinput libudev)
LDFLAGS = $(shell pkg-config --libs libinput libudev)
SRC = main.c
OUT = executable

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT) $(LDFLAGS)

clean:
	rm -f $(OUT)
