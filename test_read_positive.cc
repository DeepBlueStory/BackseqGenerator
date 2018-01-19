#include <iostream>
#include <vector>
#include <string>

typedef std::string::size_type StrPos;
typedef long StrLen;

struct SubSeq{
  char chr[6] = {0};
  std::string::size_type pos = 0;
  StrLen len = 0;
  int fd = 0;
};

