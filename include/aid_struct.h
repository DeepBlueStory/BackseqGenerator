/* -- utf_8 --*/
//Author: Li Ning

#ifndef AID_STRUCT_H
#define AID_STRUCT_H


#include <vector>
#include <string>
#include <limits>

namespace blueseq {

// Some pre-declare. 
typedef std::string::size_type StrPosType;
const double DOUBLEMAX = std::numeric_limits<double>::max()


// For every posssible subsequence, we need to now the position in chrome,
// and the score of similarirty between input sequence's markov and its.
// 
// Potentail negative subsequnce.
struct NegPotSeq {
  // Position in chrome.
  const StrPosType position_; 
  // Score of similarity of inputseq.
  const double score_;

  // constructor
  NegPotSeq(StrPosType pos, double score) explicit;
  NegPotSeq() = delete;
};
// For one input sequence, we can get a max heap. It means most similar
// negative subsequence. It will be implemet by a structure of max heap.
// The most NOT similar is on the top of max heap.
//
// Negative alternative subsequnce.
class NegAltSeq {
public:
  // Only one constructer, user must set the size of max heap.
  NegAltSeq(int heap_size);
  // Get the top subsequence's score for compared with.
  double GetMaxScore() const;
  // Push a potentail negative subsequence in the max heap.
  void PushPotSeq(NegPotSeq&& entity);

private:
  // function object for std::make_heap;
  static bool CompareScore(const NegPotSeq& left, const NegPotSeq& right);

  // The size of max heap.
  const std::vector<NegPotSeq>::size_type heap_size_;
  // The base structure of max heap is vector.
  std::vector<NegPotSeq> max_heap_;
};

} // namesapce blueseq

#endif //AID_STRUCT_H
