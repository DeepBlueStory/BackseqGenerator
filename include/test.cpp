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
#include "fasta.h"
using namespace std;

int main(int argc, char* argv[]) {
  blue::Fasta a;
  a.Open(argv[1]);
  return 0;
}
