//Author: Li Ning
//Date: 2017-04-09

#include <algorithm>
#include <queue>
#include <stack>
#include <stdio.h>
#include <math.h>
#include "practical_hg.h"

blue::PracticalHG::PracticalHG(int rank, int back_size, int length):
    rank_(rank), mask_(~((~0)<<(2*rank))), 
    markov_(std::vector<double>(1<<(2*rank), 0)), 
    back_size_(back_size), length_(length)
  {}

void blue::PracticalHG::AddPosFile(const char* file_path) {
  auto pos_chrom = ReadPosFile(file_path);
  GeneratePosVec(pos_chrom);
  RepBack(pos_chrom);
}

std::vector<std::vector<blue::SubSeq>> ReadPosFile(const char* file_path) {
  std::vector<std::vector<blue::SubSeq>> pos_chrom(23);
  FILE* fp;
  if ((fp = fopen(file_path, "r")) == NULL) {
    printf("Can't open positive file %s", file_path);
    return pos_chrom;
  }

  char str[200];
  int chr_name = 0;
  off_t v1 = 0;
  off_t v2 = 0;
  while(fgets(str, 200, fp)) {
    int chr_name_cur = 0;
    if (sscanf(str, "chr%d\t%llu\t%llu%*s", &chr_name_cur, &v1, &v2) == 3) {
      chr_name = chr_name_cur;
    } else if (sscanf(str, "chrX\t%llu%llu%*s", &v1, &v2) == 2) {
      chr_name = 22;
    } else {
      continue;
    }
    pos_chrom[chr_name].emplace_back(blue::SubSeq(chr_name, 0, v1, v2));
  }
  return pos_chrom;
}
  

void blue::PracticalHG::GeneratePosVec(std::vector<std::vector<blue::SubSeq>> pos_chrom) {
  for (int i = 0; i < 23; i++) {
    const char* chrom_i_p = hg_[i][0].chr_p;
    for (auto seq : pos_chrom[i]) {
      off_t start = seq.start;
      off_t end = seq.end;
      int dimension = 0;
      for (off_t j = 0; j < rank_-1; j++) {
        dimension <<= 2;
        dimension += *(chrom_i_p + start + j) & 6;
      }
      for (off_t j = start+rank_-1; j < end; j++) {
        dimension <<= 2;
        dimension += *(chrom_i_p + j) & 6;
        markov_[dimension>>1&mask_] += 1;
      }
    }
  }
  double sum = 0;
  for (auto i : markov_) {
    sum += i;
  }
  for (auto& i : markov_) {
    i = i / sum;
  }
}


void blue::PracticalHG::RepBack(std::vector<std::vector<blue::SubSeq>> pos_chrom) {
  for (int i = 0; i < 23; i++) {
    const char* chrom_i_p = hg_[i][0].chr_p;
    std::vector<off_t> point;
    for (auto seq : hg_[i]) {
      point.push_back(seq.start);
      point.push_back(seq.end);
    }
    for (auto seq : pos_chrom[i]) {
      point.push_back(seq.start);
      point.push_back(seq.end);
    }
    std::sort(point.begin(), point.end());
    for (int j = 0; j < point.size(); j+=2) {
      if (point[j+1] - point[j] < length_) {
        continue;
      }
      all_seqs_.emplace_back(blue::SubSeq(i, chrom_i_p, point[j], point[j+1]));
    }
  }
}


void blue::PracticalHG::FindBackSeqs(int step) {
  if (step < 1) {
    step = 1;
  }
  for (auto subseq : all_seqs_) {
    int chr_name = subseq.chr_name;
    const char* chrom_p = subseq.chr_p;
    off_t start = subseq.start;
    off_t end = subseq.end;

    std::stack<int> markov_stack;
    std::vector<double> markov_cur(1<<2*rank_, 0);

    int dimension = 0;
    for (off_t i = 0; i < rank_-1; i++) {
      dimension <<= 2;
      dimension += *(chrom_p + start + i) & 6;
    }
    for (off_t i = start+rank_-1; i < start-length_; i++) {
      dimension <<= 2;
      dimension += *(chrom_p + i) & 6;
      markov_cur[dimension>>1&mask_] += 1;
      markov_stack.push(dimension>>1&mask_);
    }
    AddNegSeq(SubSeq(chr_name, 0, start, start+length_), markov_cur);
    
    for (off_t i = start+length_+step; i < end; ) {
      for (off_t j = 0; j < step; j++) {
        dimension <<= 2;
        dimension += *(chrom_p + i + j) & 6;
        markov_cur[dimension>>1&mask_] += 1;
        markov_stack.push(dimension>>1&mask_);
      }
      for (int j = 0; j < step; j++) {
        int dim = markov_stack.top();
        markov_cur[dim] -= 1;
        markov_stack.pop();
      }
      AddNegSeq(SubSeq(chr_name, 0, i-length_, i), markov_cur);
      i += step;
    }
  }
}
      

void blue::PracticalHG::WriteNegFile(const char* file_path) {
  FILE* fp;
  if ((fp = fopen(file_path, "w")) == NULL) {
    printf("Can't write background files at current directory\n");
    return ;
  }
  while (!minheap_.empty()) {
    SubSeq seq = minheap_.top();
    int chr_name = seq.chr_name;
    off_t start = seq.start;
    off_t end = seq.end;
    if (chr_name == 22) {
      fprintf(fp, "chrX\t%llu\t%llu\n", start, end);
    } else {
      fprintf(fp, "chr%d\t%llu\t%llu\n", chr_name, start, end);
    }
    minheap_.pop();
  }
}


void blue::PracticalHG::AddNegSeq(blue::SubSeq seq, std::vector<double> markov_cur) {
  double sum = 0;
  for (auto i : markov_cur) {
    sum += i;
  }
  for (auto& i : markov_cur) {
    i = i / sum;
  }

  double score = 0;
  for (int i = 0; i < (1<<2*rank_); i++) {
    score += fabs(markov_cur[i] - markov_[i]);
  }
  seq.score = score;
  
  minheap_.push(seq);
  if (minheap_.size() > back_size_) {
    minheap_.pop();
  }
}
