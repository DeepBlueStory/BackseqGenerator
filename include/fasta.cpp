//Author: Li Ning
//Date: 2017-04-09

#include <iostream>

#include "fasta.h"

const char* CONF_FILE = "conf";

bool blue::Fasta::Open(const char *file_path) {
  if (!mmap_object_.Open(file_path)) {
    return false;
  }
  const char* data_pointer = static_cast<const char*> (mmap_object_.GetData());
  PreProcess(data_pointer);
  return true;
}

const char* blue::Fasta::operator[](int chr_name) {
  return chrom_pointers[chr_name-1];
}

void blue::Fasta::PreProcess(const char* data_pointer) {
    // read configuration file to get lenght and calculable position
    //
    // name of configuration file is fixed.
    // 'conf'
    // if full genome sequences changed, the 'conf' file need regenerate.

    off_t displacement = 0;
    char str[50] = 0;
    int chr_name = 0;
    FILE* fp;
    
    if ((fp=fopen(CONF_FILE, "r")) == NULL) {
        return false;
    }
    while (fgets(str, 50, fp)) {
        off_t v1 = 0;
        off_t v2 = 0;
        if (sscanf(str, "%d_%d", &v1, &v2) == 2) {
            chrom_pointers



    while(*(data_pointer + displacement) == '#') {
        while(*(data_pointer + displacement) != '\n') {
            displacement ++;
        }
        displacement ++;
    }
    for(int i = 0; i < 23; i++) {
        chrom_pointers[i] = data_pointer + displacement;
        displacement += g_chrom_size[i] + 1;
        while(*(data_pointer + displacement) != '\n') {
            displacement ++;
        }
        displacement ++;
    }
}
