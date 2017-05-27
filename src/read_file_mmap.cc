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

#include "read_file_mmap.h"

ReadFileMmap::ReadFileMmap(const char* file_path) :
  file_descriptor_(0),
  start_(NULL) {
  file_descriptor_ = open(file_path, O_RDONLY);
  if(file_descriptor_ == -1) {
    printf("error in open file %s, errno: %d\n", file_path, errno);
  }
  else {
    fstat(file_descriptor_, &stat_block_);
    start_ = mmap(NULL, stat_block_.st_size, PROT_READ, MAP_PRIVATE,
                  file_descriptor_, 0);
    if(start_ == MAP_FAILED) {
      printf("error in mmap file %s, errno: %d\n", file_path, errno);
      start_ = NULL;
    }
  }
}

ReadFileMmap::~ReadFileMmap() {
  munmap(start_, stat_block_.st_size);
  close(file_descriptor_);
}

const void* ReadFileMmap::GetDataPointer() {
  return start_;
}

off_t ReadFileMmap::GetFileSize() {
  return stat_block_.st_size;
}
