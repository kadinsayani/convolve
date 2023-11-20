CC = g++
CFLAGS = -g
RM = rm -f

default: all

all: convolve

convolve: convolve.cpp
	$(CC) $(CFLAGS) -o convolve convolve.cpp
	./convolve ./star_wars.wav ./big_hall_mono.wav ./output.wav

clean veryclean:
	$(RM) convolve