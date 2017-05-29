#include "read_fasta.h"

int main() {
  const char* file_path = "/home/breeze/BSgenome.Hsapiens.UCSC.hg19/chr21.fasta";
  blueseq::ReadFasta chr21;
  chr21.Open(file_path);
  off_t length = chr21.GetLength();
  for(off_t i = 0; i < length; i++) {
    printf("%c", chr21[i]);
  }
  return 0;
} 