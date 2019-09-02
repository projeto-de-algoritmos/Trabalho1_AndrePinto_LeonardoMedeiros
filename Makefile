CC := g++
CFLAGS := -W -Wall -std=c++14
SFMLFLAGS := -lsfml-graphics -lsfml-window -lsfml-system
speed = 4

all:
	$(CC) $(CFLAGS) -c main.cpp -I ./SFML-2.5.1/include/
	$(CC) main.o -o prog -L ./SFML-2.5.1/lib/ $(SFMLFLAGS)

clean:
	rm prog
	rm main.o

run:
	export LD_LIBRARY_PATH=./SFML-2.5.1/lib/ && ./prog $(speed)
