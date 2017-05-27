//Author: Li Ning
//Date: 2017-04-09
//Brief: read file by mmap.

#ifndef READ_FILE_MMAP_H
#define READ_FILE_MMAP_H

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

class ReadFileMmap {
protected:
  int file_descriptor_;
  void* start_;
  struct stat stat_block_;

public:
  ReadFileMmap(const char* file_path);
  virtual ~ReadFileMmap();
  const void* GetDataPointer();
  off_t GetFileSize();
};

#endif //READ_FILE_MMAP_H
