#include "aid_struct.h"
#include "read_fasta.h"
#include "markov_cal.h"

#include <vector>
#include <malloc.h>

std::vector<Kmer> FilterInStep(ReadFasta& chr,
    int length_positive,
    int* markov_positive,
    int step,
    int order);

int main() {
  const char* file_path = "/home/breeze/BSgenome.Hsapiens.UCSC.hg19/chr21.fasta";
  ReadFasta chr21(file_path);
  char positive_seq[300];
  const char* str_chr21 = chr21.c_str();
  long length_chr21 = chr21.GetChromosomeLength();
  int start = 13000;
  int end = 13300;
  int i = 0;
  int j = 0;
  for(i = start; i < end; i++) {
    positive_seq[j++] = str_chr21[i];
  }


  int step = 500;
  int markov_order = 2;
  int seq_num_needed = 10;
  int length_positive = 300;

  int length_markov = 1;
  length_markov <<= 2 * markov_order;
  int* markov_positive = (int*)calloc(length_markov, sizeof(int));
  if(!SeqToMarkov(positive_seq, length_positive, markov_order, markov_positive)) {
    printf("error in convert positive sequence to markov \n");
    return 0;
  }

  // filter in step.
  std::vector<Kmer> first_layer = FilterInStep(chr21,
    length_positive, markov_positive, step, markov_order);
  printf("how many first layer: %d\n", (int)first_layer.size());
  double min = DOUBLEMAX;
  double max = 0;
  for(auto x: first_layer) {
    printf("score: %f\n", x.score_);
  }
  printf("max : %f, min: %f\n", max, min);
  return 0;
}

std::vector<Kmer> FilterInStep(ReadFasta& chr,
    int length_positive,
    int* markov_positive,
    int step,
    int order) {
  std::vector<Kmer> res;
  const char* seq = chr.c_str();
  int length_markov = 1;
  length_markov <<= 2 * order;
  int* markov_vec = (int*)calloc(length_markov, sizeof(int));

  for(unsigned long i = 0; i < chr.GetChromosomeLength() - length_positive; i += step) {
    SeqToMarkov(seq+i, length_positive, order, markov_vec);
    double score = EuclideanDistance(markov_vec, markov_positive, length_markov);
    res.push_back(Kmer(i, score));
  }
  return res;
}
