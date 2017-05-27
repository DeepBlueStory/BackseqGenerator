//Author: Li Ning
//Date: 2017-04-09

#include <stdio.h>

#include "read_fasta.h"

ReadFasta::ReadFasta(const char* file_path):
  ReadFileMmap(file_path),
  string_(NULL),
  length_(0) {
  sscanf((const char*)start_, "%*[^_]_length_%ld[^\r\n]", &length_);
  off_t i = 0;
  for(; i < stat_block_.st_size; i++) {
    if(*((char*)start_+i) == '\n') {
      break;
    }
  }
  string_ = (const char*) start_ + i + 1;
}

const char& ReadFasta::operator[](off_t i) {
  return string_[i];
}

off_t ReadFasta::GetChromosomeLength() {
  return length_;
}

const char* ReadFasta::c_str() {
  return string_;
}
