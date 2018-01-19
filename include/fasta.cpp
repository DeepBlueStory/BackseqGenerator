//Author: Li Ning
//Date: 2017-04-09

#include <stdio.h>
#include "fasta.h"

const char* CONF_FILE = "conf";
namespace blue {

// chromosome size
off_t g_chrom_size[23];

}


bool blue::Fasta::Open(const char *file_path) {
  if (!mmap_object_.Open(file_path)) {
    return false;
  }
  const char* data_pointer = static_cast<const char*> (mmap_object_.GetData());
  if (!PreProcess(data_pointer, mmap_object_.GetFileSize())) {
    return false;
  }
  return true;
}

std::vector<blue::SubSeq> blue::Fasta::operator[](int chr_name) {
  return chrom_seqs_[chr_name];
}

const std::vector<std::vector<std::pair<off_t, off_t>>> blue::Fasta::ReadConf() {
  std::vector<std::vector<std::pair<off_t, off_t>>> ret(23);
  char str[100] = {0};
  FILE* fp;
  if ((fp=fopen(CONF_FILE, "r")) == NULL) {
    printf("ERROR!!! erro in read conf\n");
    return ret;
  }
  
  int chr_name = 0;
  while (fgets(str, 100, fp) != NULL) {
    off_t v1 = 0;
    off_t v2 = 0;
    if (sscanf(str, "%llu_%llu", &v1, &v2) == 2) {
      ret[chr_name].emplace_back(std::make_pair(v1, v2));
    } else {
      sscanf(str, "c%d_%llu", &chr_name, &v2);
      blue::g_chrom_size[chr_name] = v2;
    }
  }
  return ret;
}


bool blue::Fasta::PreProcess(const char* dp, off_t file_size) {
    // read configuration file to get lenght and calculable position
    //
    // name of configuration file is fixed.
    // 'conf'
    // if full genome sequences changed, the 'conf' file need regenerate.
  auto conf_seqs = ReadConf();
  off_t disp = 0;
  while(*(dp+disp) != '>') {
    disp++;
  }

  int chr_name = 0;
  while (disp < file_size) {
    off_t lf_dis = 0;
    while (*(dp+disp+lf_dis) != '\n') {
      lf_dis ++;
    }
    char str[100] = {0};
    strncpy(str, dp+disp, lf_dis);
    int chr_name_cur = 0;
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
    for (auto seq : conf_seqs[chr_name]) {
      chrom_seqs_[chr_name].emplace_back(
              SubSeq(chr_name, dp+disp, seq.first, seq.second));
    }
    disp += g_chrom_size[chr_name] + 1;
  }

  return true;
}
