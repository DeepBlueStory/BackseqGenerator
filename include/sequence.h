/**
 * @file sequence.h
 * @brief
 *
 **/

#ifndef SEQUENCE_H
#define SEQUENCE_H

namespace blue {

struct SubSeq {
    int chr_name;
    const char* chr_p;
    off_t start;
    off_t end;
    double score;

    SubSeq(int chr_name, const char* chr_p, off_t start, off_t end):
            chr_name(chr_name), chr_p(chr_p), start(start), end(end), score(0)
        {}
    bool operator<(const struct SubSeq & right) const {
        if (start < right.start) {
            return true;
        }else {
            return false;
        }
    }
};
}

#endif // SEQUENCE_H
