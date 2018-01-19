//Author: Li Ning
//Date: 2017-04-10
//Brief: define some function associate with markov.

#ifndef MARKOV_CAL_H
#define MARKOV_CAL_H

namespace blueseq {
// JumpMarkov.
class JumpMarkov {
public:
  // set positive sequence set.
  // In there, every positive sequences will be convert to markov represent.
  JumpMarkov& SetPositiveSeqs(std::vector<std::string> pos_seqs);
  // Set chromosome data.
  JumpMarkov& SetChromoData(const char *file_path);
  // Get every max heap of every positive sequence.
  std::vector<blueseq::NegAltSeq>& GetAltNegSeq();

private:
  // postive sequence markov.
  std::vector<int *> pos_markov_;
  // ReadFasta object. Get the chromosome data.
  ReadFasta chr_;
  // The max heap for every positive seq.
  std::vector<blueseq::NegAltSeq> pos_maxheap_;

  
};
}
//euclidean distance with no sqrt.
double EuclideanDistance(const int* left, const int* right, int size);

// convert DNA sequence to markov vector.
bool SeqToMarkov(const char* seq, int length, int order, int* markov_vec);

// convert DNA alphabet to 0123.
inline int DNATo0123(char c);

#endif // MARKOV_CAL_H
