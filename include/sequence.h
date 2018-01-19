/***************************************************************************
 *
 * Copyright (c) 2017 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

/**
 * @file sequence.h
 * @author lining(lining21@baidu.com)
 * @date 2018-01-17 18:07:17
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
};
}

#endif // SEQUENCE_H
