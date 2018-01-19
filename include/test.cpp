/***************************************************************************
 *
 * Copyright (c) 2017 Baidu.com, Inc. All Rights Reserved
 *
 **************************************************************************/

/**
 * @file test.cpp
 * @author lining(lining21@baidu.com)
 * @date 2018-01-11 14:57:55
 * @brief 
 *
 **/

#include <iostream>
using namespace std;

int main() {
    off_t v1 = 99999999;

    const char* str = "chr1_length_249250623";

    sscanf(str, "chr1_length_%ld", &v1);

    printf("%d", v1);

    return 0;
}
