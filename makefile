CPPFLAGS = -std=c++11
CC = g++

all: backseqgenerator preprocess
.PHONY : all

backseqgenerator: main.o practical_hg.o fasta.o mmap.o
	$(CC) $(CPPFLAGS) -o backseqgenerator main.o practical_hg.o fasta.o mmap.o

preprocess: preprocess.o mmap.o
	$(CC) $(CPPFLAGS) -o preprocess preprocess.o mmap.o


clean:
	rm -f *.o backseqgenerator preprocess
