//Author: Li Ning
//Date: 2017-04-09
//Brief: practical hg

#ifndef PRACTICAL_HG_H
#define PRACTICAL_HG_H

#include <vector>
#include <queue>
#include "sequence.h"
#include "fasta.h"

namespace blue {


class BackSSCmp {
public:
    bool operator() (const SubSeq& left, const SubSeq& right) {
        return left.score < right.score;
    }
};


class PracticalHG {
public:
  PracticalHG(int rank, int back_size, int length);
  void AddPosFile(const char* file_path);
  void FindBackSeqs(int step=1);
  void WriteNegFile(const char* file_path);

private:
  std::vector<std::vector<SubSeq>> ReadPosFile(const char* file_path);
  void GeneratePosVec(std::vector<std::vector<SubSeq>>);
  void RepBack(std::vector<std::vector<SubSeq>>);
  void AddNegSeq(SubSeq, std::vector<double>);
  
private:
  std::vector<double> markov_;
  int rank_;
  int mask_;
  int back_size_;
  int length_;
  std::vector<SubSeq> all_seqs_;
  std::priority_queue<SubSeq, std::vector<SubSeq>, BackSSCmp> minheap_;
  static Fasta hg_;
};  // class PracticalHG

} // namespace blue.

#endif // PRACTICAL_HG_H
