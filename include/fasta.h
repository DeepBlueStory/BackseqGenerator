//Author: Li Ning
//Date: 2017-04-09
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
  std::vector<subseq>  operator[](int chr_name);
private:
  // Pre-process. What if there has one line declare the file.
  bool PreProcess(const char*);

private:
  // ReadFileMmap object.
  Mmap mmap_object_;
  // chrom pointers.
  std::vector<subseq> chrom_pointers[23];
}; // class ReadFasta.

// chromosome size
off_t g_chrom_size[23];

} // namespace blue.
#endif //FASTA_H
