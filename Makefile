CC = g++
CFLAGS = -Wall -Wextra -O2 -std=c++20
SRC = app.cpp
OUT = app

$(OUT): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

.PHONY: clean run
clean:
	rm -f $(OUT)

run: $(OUT)
	./$(OUT)