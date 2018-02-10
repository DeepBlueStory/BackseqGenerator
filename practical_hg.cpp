//Author: Li Ning
//Date: 2017-04-09

#include <algorithm>
#include <stack>
#include <set>
#include <string>
#include <stdio.h>
#include <math.h>
#include "practical_hg.h"

blue::PracticalHG::PracticalHG(int rank, int back_size, Fasta& hg):
    rank_(rank), mask_(~((~0)<<(2*rank))),
    back_size_(back_size), hg_(hg),
    all_seqs_(std::vector<std::multiset<off_t>>(23)),
    pos_subs_(std::vector<std::vector<SubSeq>>(23))
  {}


void blue::PracticalHG::AddPosFile(const char* file_path) {
    ReadPosFile(file_path);
    RepPos();
    OpenNegFile(file_path);
    for (int i = 0; i < 23; i++) {
        printf("size:%lu\t", pos_subs_[i].size());
    }
    printf("\nRead positive file success.\n");
}


void blue::PracticalHG::OpenNegFile(const char* file_path) {
    neg_fp_ = NULL;
    std::string neg_file_name(file_path);
    std::size_t found = neg_file_name.find_last_of(".");
    if (found == std::string::npos) {
        neg_file_name += "neg.bed";
    }else {
        neg_file_name.insert(found, "_neg");
    }
    if ((neg_fp_ = fopen(neg_file_name.c_str(), "w")) == NULL) {
        printf("Can't write background files at current directory\n");
        return ;
    }
}


void blue::PracticalHG::ReadPosFile(const char* file_path) {
  FILE* fp;
  if ((fp = fopen(file_path, "r")) == NULL) {
    printf("Can't open positive file %s", file_path);
    return;
  }

  char str[200];
  int chr_name = 0;
  off_t v1 = 0;
  off_t v2 = 0;
  while(fgets(str, 200, fp)) {
    int chr_name_cur = 0;
    if (sscanf(str, "chr%d\t%llu\t%llu%*s", &chr_name_cur, &v1, &v2) == 3) {
      chr_name = chr_name_cur-1;
    } else if (sscanf(str, "chrX\t%llu%llu%*s", &v1, &v2) == 2) {
      chr_name = 22;
    } else {
      continue;
    }
    pos_subs_[chr_name].emplace_back(blue::SubSeq(chr_name, 0, v1, v2));
  }
}


void blue::PracticalHG::RepPos() {
  for (int i = 0; i < 23; i++) {
    for (auto seq : hg_[i]) {
      all_seqs_[i].insert(seq.start);
      all_seqs_[i].insert(seq.end);
    }
    for (auto seq : pos_subs_[i]) {
      all_seqs_[i].insert(seq.start);
      all_seqs_[i].insert(seq.end);
    }
  }
}


void blue::PracticalHG::FindBackSeqs(int step) {
    printf("Generate nagtive sequences...\n");
  if (step < 1) {
    step = 1;
  }
  for (int i = 0; i < 23; i++) {
      printf("chromosome %d:\n", i+1);
      for (auto pos_sub : pos_subs_[i]) {
          printf("positive sequences:%llu  --- %llu\n", pos_sub.start, pos_sub.end);
          auto markov = GenerateMarkovProperty(pos_sub);
          auto min_neg = FindNegSeqs(step, i, pos_sub.end-pos_sub.start, markov);
          WriteNegFile(min_neg);
      }
  }
}


std::vector<double>
blue::PracticalHG::GenerateMarkovProperty(const blue::SubSeq& pos_sub) {
    std::vector<double> markov(1<<(2*rank_),0);
    off_t start = pos_sub.start;
    off_t end = pos_sub.end;
    int chr_name = pos_sub.chr_name;
    const char* chrom_i_p = hg_[chr_name][0].chr_p;
    int dimension = 0;
      for (off_t j = 0; j < rank_-1; j++) {
        dimension <<= 2;
        dimension += *(chrom_i_p + start + j) & 6;
      }
      for (off_t j = start+rank_-1; j < end; j++) {
        dimension <<= 2;
        dimension += *(chrom_i_p + j) & 6;
        markov[dimension>>1&mask_] += 1;
      }
  double sum = 0;
  for (auto i : markov) {
    sum += i;
  }
  for (auto& i : markov) {
    i = i / sum;
  }
  return markov;
}


std::vector<blue::SubSeq>
blue::PracticalHG::FindNegSeqs(int step, int chr_name, int length, std::vector<double>& markov) {
    std::vector<SubSeq> min_neg;
    auto all_seq = all_seqs_[chr_name];
    for (auto sub_ite = all_seq.begin(); sub_ite != all_seq.end(); sub_ite++) {
        const char* chrom_p = hg_[chr_name][0].chr_p;
        off_t start = *(sub_ite++);
        off_t end = *sub_ite;
        printf("subseq: %llu, %llu\n", start, end);

        std::stack<int> markov_stack;
        std::vector<double> markov_cur(1<<2*rank_, 0);

        int dimension = 0;
        for (off_t i = 0; i < rank_-1; i++) {
          dimension <<= 2;
          dimension += *(chrom_p + start + i) & 6;
        }

        for (off_t i = start+rank_-1; i < start+length; i++) {
          dimension <<= 2;
          dimension += *(chrom_p + i) & 6;
          markov_cur[dimension>>1&mask_] += 1;
          markov_stack.push(dimension>>1&mask_);
        }
        AddNegSeq(SubSeq(chr_name, 0, start, start+length),
                markov_cur, markov, min_neg);

        for (off_t i = start+length; i < end; ) {
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
          i += step;
          AddNegSeq(SubSeq(chr_name, 0, i-length, i),
                  markov_cur, markov, min_neg);
        }
    }
    return min_neg;
}


void blue::PracticalHG::WriteNegFile(std::vector<SubSeq> min_neg) {
    std::sort(min_neg.begin(), min_neg.end(), [](const SubSeq a, const SubSeq b) {return a.score < b.score;});
    int i = 0;
    auto iter = min_neg.begin();
    std::vector<std::pair<off_t, off_t>> dum;
    while (i < back_size_) {
        off_t start = iter->start;
        off_t end = iter->end;
        int chr_name = iter->chr_name;
        const char* chrom_p = hg_[chr_name][0].chr_p;
        bool in_flag = false;
        for (auto d : dum) {
            if((start >= d.first && start <= d.second) ||
                (end >= d.first && end <= d.second)) {
                in_flag = true;
                break;
            }
        }
        if(!in_flag) {
            if(chr_name == 22) {
                fprintf(neg_fp_, ">chrX:%llu-%llu\n", start, end);
            }else {
                fprintf(neg_fp_, ">chr%d:%llu-%llu\n", chr_name, start, end);
            }
            for(off_t j = start; j < end; j++) {
                fprintf(neg_fp_, "%c", *(chrom_p+j));
            }
            fprintf(neg_fp_, "\n");
            dum.push_back(std::make_pair(start, end));
            all_seqs_[chr_name].insert(start);
            all_seqs_[chr_name].insert(end);
            i++;
        }
        iter++;
    }
}


void blue::PracticalHG::AddNegSeq(blue::SubSeq seq, std::vector<double> markov_cur,
        std::vector<double>& markov, std::vector<SubSeq>& min_neg) {
  double sum = 0;
  for (auto i : markov_cur) {
    sum += i;
  }
  for (auto& i : markov_cur) {
    i = i / sum;
  }

  double score = 0;
  for (int i = 0; i < (1<<2*rank_); i++) {
    score += fabs(markov_cur[i] - markov[i]);
  }
  seq.score = score;

  min_neg.push_back(seq);
}
