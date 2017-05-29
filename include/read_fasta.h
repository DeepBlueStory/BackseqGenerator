//Author: Li Ning
//Date: 2017-04-09
//Brief: read fasta file based on mmap;

#ifndef READ_FASTA_H
#define READ_FASTA_H

#include <functional>

#include "read_file_mmap.h"


namespace blueseq {
// Read Fasta file based on ReadFileMmap.
class ReadFasta {
public:
  // Open one file.
  bool Open(const char* file_path);
  // Get data in i position.
  const char& operator[](off_t i);
  // Get the length of chromosome.
  const off_t GetLength();
private:
  // Pre-process. What if there has one line declare the file.
  //void PreProcess(std::function<void(const char*)> func);
  void PreProcess();

private:
  // ReadFileMmap object.
  ReadFileMmap mmap_object_;
  // data pointer.
  const char *data_pointer_;
  // sequence length.
  off_t seq_length_;
}; // class ReadFasta.

} // namespace blueseq.
#endif //READ_FASTA_H
