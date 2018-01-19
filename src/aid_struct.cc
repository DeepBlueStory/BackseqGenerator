/* -- utf_8 -- */
//Author: Li Ning

#include <algorithm>
#include <vector>
#include "aid_struct.h"


blueseq::NegPotSeq::NegPotSeq(StrPosType pos, double score):
  position_(pos),
  score_(socre) {}

blueseq::NegAltSeq::NegAltSeq(int size):
  heap_size_(size) {}

double blueseq::NegAltSeq::GetMaxScore() {
  if (max_heap_.size() < heap_size_) {
    return DOUBLEMAX;
  }
  return max_heap_.front().score_;
}

blueseq::NegAltSeq::PushPotSeq(NegPotSeq&& entity) {
  if (max_heap_.size() < heap_size_) {
    max_heap_.emplace_back(entity);
    if(max_heap_.size() == heap_size_) {
      std::make_heap(max_heap_.begin(), max_heap_.end(), CompareScore);
    }
  } else {
    max_heap_.emplace_back(entity);
    std::push_heap(max_heap_.begin(), max_heap_.end(), CompareScore);
    std::pop_heap(max_heap_.begin(), max_heap_.end(), CompareScore);
    max_heap_.pop_back();
  }
}

bool blueseq::NegAltSeq::CompareScore(const NegPotSeq& left, const NegPotSeq& right) {
  return left.score_ > right.score_;
}

const std::vector<NegPotSeq>& blueseq::NegAltSeq::GetAltSeq() {
  return max_heap_;
}