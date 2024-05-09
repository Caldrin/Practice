CC      = gcc
CFLAGS  = -Wall -Werror -std=c99

OBJ = reverse.o
BIN = reverse.exe

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
	rm -rf $(BIN) $(OBJ)
