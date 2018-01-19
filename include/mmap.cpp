//Author: Li Ning
//Date: 2017-04-09
//Brief: read file by mmap.

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>

#include "mmap.h"


bool blue::Mmap::Open(const char *file_path) {
  file_descriptor_ = open(file_path, O_RDONLY);
  if (file_descriptor_ == -1) {
    printf("error in open file %s, errno: %d\n", file_path, errno);
    return false;
  } else {
    fstat(file_descriptor_, &stat_block_);
    data_pointer_ = mmap(
      NULL, 
      stat_block_.st_size, 
      PROT_READ, 
      MAP_PRIVATE,
      file_descriptor_,
      0
    );
    if (data_pointer_ == MAP_FAILED) {
      printf("error in mmap, errno: %d\n", errno);
      data_pointer_ = nullptr;
      return false;
    }    
  }
  return true;
}

blue::Mmap::~Mmap() {
  munmap(data_pointer_, stat_block_.st_size);
  close(file_descriptor_);
}

const void* blue::Mmap::GetData() {
  return data_pointer_;
}

off_t blue::Mmap::GetFileSize() {
  return stat_block_.st_size;
}
