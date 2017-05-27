//Author: Li Ning
//Date: 2017-04-10
//Brief: define some function associate with markov.

#ifndef MARKOV_CAL_H
#define MARKOV_CAL_H

namespace blueseq {
// Calculate two 
}
//euclidean distance with no sqrt.
double EuclideanDistance(const int* left, const int* right, int size);

// convert DNA sequence to markov vector.
bool SeqToMarkov(const char* seq, int length, int order, int* markov_vec);

// convert DNA alphabet to 0123.
inline int DNATo0123(char c);

#endif // MARKOV_CAL_H
