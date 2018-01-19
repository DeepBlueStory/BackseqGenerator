/* -- utf_8 --*/
//Author: Li Ning

#ifndef AID_STRUCT_H
#define AID_STRUCT_H


#include <vector>
#include <string>
#include <limits>
#include <map>

namespace blueseq {

// Some pre-declare. 
typedef std::string::size_type StrPosType;
const double DOUBLEMAX = std::numeric_limits<double>::max()

struct SubSeq {
  
};

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
  // Get all the seq in max heap.
  const std::vector<NegPotSeq>& GetAltSeq();

private:
  // function object for std::make_heap;
  static bool CompareScore(const NegPotSeq& left, const NegPotSeq& right);

private:
  // The size of max heap.
  const std::vector<NegPotSeq>::size_type heap_size_;
  // The base structure of max heap is vector.
  std::vector<NegPotSeq> max_heap_;
};

// For every opened files, we need to recognize its file descriptor with the file name.
// 
// File decriptor to file name.
class FDtoFN {
public:
  // Push a file descriptor and file name into map.
  void push_back(std::pair<int, std::string> file);
  // Get the file name
  const std::string& operator[](int fd);
private:
  std::map<int, std::string> fd_fn_;
};


} // namesapce blueseq

#endif //AID_STRUCT_H
