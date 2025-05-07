CC = gcc
CFLAGS = -Wall -I../include
LDFLAGS = ../build/libpkcs11_crypto.a -lcmocka -ldl

TARGET = test_crypto
SRC = test_crypto.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET)
