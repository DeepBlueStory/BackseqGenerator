//Author: Li Ning
//Brief: read fasta file based on mmap;

#ifndef FASTA_H
#define FASTA_H

#include <functional>
#include <string>
#include <vector>

#include "mmap.h"
#include "sequence.h"

namespace blue {
// Read Fasta file based on ReadFileMmap.
class Fasta {
public:
  // Open one file.
  bool Open(const char* file_path);
  // Get pointer in chrom i
  std::vector<blue::SubSeq>  operator[](int chr_name);
private:
  // Read configuration file.
  const std::vector<std::vector<std::pair<off_t, off_t>>>  ReadConf();
  // Pre-process. What if there has one line declare the file.
  bool PreProcess(const char*, off_t);

private:
  // ReadFileMmap object.
  Mmap mmap_object_;
  // chrom pointers.
  std::vector<blue::SubSeq> chrom_seqs_[23];
}; // class ReadFasta.

} // namespace blue.
#endif //FASTA_H
