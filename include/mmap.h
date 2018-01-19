//Author: Li Ning
//Date: 2017-04-09
//Brief: read file by mmap.

#ifndef MMAP_H
#define MMAP_H

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

namespace blue {
// read file by the mmap function.
// Use the mmap function and munmap function to map a external storage to 
// internal memory.
class Mmap {
public:
  // Use the munmap function in linux.
  virtual ~Mmap();
  // Open the file and map the memory.
  bool Open(const char *file_path);
  // Return the start pointer of data. 
  const void* GetData();
  // Return the file size.
  off_t GetFileSize();

private:
  // file descriptor, get from the open function.
  int file_descriptor_ = 0;
  // the start pointer of data.
  void *data_pointer_ = nullptr;

  struct stat stat_block_;
}; // class Mmap

} // namespace blue

#endif //MMAP_H
