#include <time.h>
#include "markov_cal.h"

double EuclideanDistance(const int* left, const int* right, int size) {
  double res = 0;
  for(int i = 0; i < size; i++) {
    res += (left[i] - right[i]) * (left[i] - right[i]);
  }
  return res;
}

bool SeqToMarkov(const char* seq, int length, int order, int* markov_vec) {
  if(order > length) {
    return false;
  }
  int length_markov = 1;
  length_markov <<= 2 * order;
  int i = 0;
  int num = 0;
  for(; i < order; i++) {
    num = (num << 2) + DNATo0123(seq[i]);
  }
  markov_vec[num]++;
  for(; i < length; i++) {
    num = (num << 2) + DNATo0123(seq[i]);
    num = num % length_markov;
    markov_vec[num]++;
  }
  return true;
}

inline int DNATo0123(char c) {
  if(c & 8) {
    return clock() & 2;
  }
  else {
    return (c & 6) >> 1;
  }
}

