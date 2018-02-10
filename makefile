CPPFLAGS = -std=c++11
CC = g++
vpath %.h include
vpath %.cpp src
backseqgenerator: main.o practical_hg.o fasta.o mmap.o
