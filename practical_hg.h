//Author: Li Ning
//Date: 2017-04-09
//Brief: practical hg

#ifndef PRACTICAL_HG_H
#define PRACTICAL_HG_H

#include <vector>
#include <queue>
#include <set>
#include "sequence.h"
#include "fasta.h"

namespace blue {

class PracticalHG {
public:
  PracticalHG(int rank, int back_size, Fasta& hg);
  void AddPosFile(const char* file_path);
  void FindBackSeqs(int step=1);

private:
  void ReadPosFile(const char* file_path);
  void RepPos();
  void OpenNegFile(const char* file_path);
  std::vector<double> GenerateMarkovProperty(const SubSeq&);
  std::vector<SubSeq> FindNegSeqs(int step, int chr_name, int length, std::vector<double>& markov);
  void WriteNegFile(std::vector<SubSeq> min_neg);
  void AddNegSeq(blue::SubSeq seq, std::vector<double> markov_cur,
        std::vector<double>& markov, std::vector<SubSeq>& min_neg);

private:
  int rank_;
  int mask_;
  int back_size_;
  FILE * neg_fp_;
  std::vector<std::vector<SubSeq>> pos_subs_;
  std::vector<std::multiset<off_t>> all_seqs_;
  Fasta& hg_;
};  // class PracticalHG

} // namespace blue.

#endif // PRACTICAL_HG_H
