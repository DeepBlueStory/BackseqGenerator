// Author: LiNing
//

#include <vector>
#include <utility>
#include <stdio.h>

#include "mmap.h"

using namespace blue;

void CalculateChromLength(const char* dp, off_t file_size);

int main(int argc, char* argv[]) {
    Mmap f;
    if (argc != 2) {
      printf("usage: %s Hg38\n", argv[0]);
      return 0;
    }
    if (!f.Open(argv[1])) {
      printf("please check the hg file\n");
      return 1;
    }
    printf("---> Begin to preprocess the BSgenome file\n");
    CalculateChromLength(static_cast<const char*> (f.GetData()), f.GetFileSize());
    return 0;
}

void CalculateChromLength(const char* dp, off_t file_size) {
  FILE* fp;
  if ((fp=fopen("conf", "w")) == NULL) {
    printf("Can't write at current directory\n");
    return ;
  }
  off_t disp = 0;
  while(*(dp+disp) != '>') {
    disp++;
  }

  int chr_name = 0;
  while (disp < file_size) {
    off_t lf_dis = 0;
    while(*(dp+disp+lf_dis) != '\n') {
      lf_dis ++;
    }
    char str[100] = {0};
    strncpy(str, dp+disp, lf_dis);
    int chr_name_cur = 0;
    printf("---> %s\n", str);
    if (strstr(str, "_") != NULL) {
      disp += lf_dis +1;
      while (*(dp+disp) != '\n') {
        disp++;
      }
      disp++;
      continue;
    }
    if (sscanf(str, ">chr%d", &chr_name_cur) == 1) {
      chr_name = chr_name_cur - 1;
    } else if (strcmp(str, ">chrX") == 0) {
      chr_name = 22;
    } else {
      disp += lf_dis +1;
      while (*(dp+disp) != '\n') {
        disp++;
      }
      disp++;
      continue;
    }
    disp += lf_dis + 1;
    off_t length = 0;
    off_t start = 0;
    off_t end =0;
    std::vector<std::pair<off_t, off_t>> subseq;
    bool has_head = false;
    while (*(dp+disp+length) != '\n') {
      if (*(dp+disp+length) != 'N') {
        if (!has_head) {
          has_head = true;
          start = length;
        }
      } else {
        if (has_head) {
          has_head = false;
          end = length;
          if ((end-start) > 100) {
            subseq.emplace_back(std::make_pair(start, end));
          }
        }
      }
      length++;
    }
    if (has_head) {
      has_head = false;
      end = length;
      if ((end-start) > 100) {
        subseq.emplace_back(std::make_pair(start, end));
      }
    }
      
    printf("---> Write configuration ... %.2f%%\n", chr_name*100.0/23);
    fprintf(fp, "c%d_%llu\n", chr_name, length);
    for (auto ss : subseq) {
      fprintf(fp, "%llu_%llu\n", ss.first, ss.second);
    }
    disp += length + 1;
  }
}
