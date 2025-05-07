CC = gcc
CFLAGS = -I../include
LIBS = -L../build -lpkcs11_crypto \
       -lulfius -lyder -ljansson -lgnutls -lssl -lcrypto -lpthread -ldl

TARGET = server
SRC = server.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) -Wl,-rpath=../build $(LIBS)

clean:
	rm -f $(TARGET)
