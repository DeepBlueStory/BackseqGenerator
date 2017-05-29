//Author: Li Ning
//Date: 2017-04-09

#include <stdio.h>

#include "read_fasta.h"

bool blueseq::ReadFasta::Open(const char *file_path) {
  if (!mmap_object_.Open(file_path)) {
    return false;
  }
  const void* file_pointer = mmap_object_.GetDataPointer();
  data_pointer_ = static_cast<const char*> file_pointer;
  PreProcess();
  return true;
}

const char& blueseq::ReadFasta::operator[](off_t i) {
  static const char *data_pointer = (const char *)mmap_object_.GetDataPointer();
  return data_pointer_[i];
}

const off_t blueseq::ReadFasta::GetLength() {
  return seq_length_;
}

void blueseq::ReadFasta::PreProcess() {
  sscanf(data_pointer_, "%*[^_]_length_%ld[^\r\n]", &seq_length_);
  off_t i = 0;
  for(; i < mmap_object_.GetFileSize(); i++) {
    if (data_pointer_[i] == '\n') {
      break;
    }
  }
  ++data_pointer_;
}