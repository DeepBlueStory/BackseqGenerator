//Author: Li Ning
//Date: 2017-04-09
//Brief: read fasta file based on mmap;

#ifndef READ_FASTA_H
#define READ_FASTA_H

#include "read_file_mmap.h"

class ReadFasta: public ReadFileMmap{
private:
  const char* string_;
  off_t length_;
public:
  ReadFasta(const char* file_path);
  const char& operator[](off_t i);
  const char* c_str();
  off_t GetChromosomeLength();
};

#endif //READ_FASTA_H
