//Author: Li Ning
//Date: 2017-04-09
//Brief: read file by mmap.

#ifndef READ_FILE_MMAP_H
#define READ_FILE_MMAP_H

#include <sys/type.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

namespace blue {
// read file by the mmap function.
// Use the mmap function and munmap function to map a external storage to 
// internal memory.
class ReadFileMmap {
public:
  // Use the munmap function in linux.
  virtual ~ReadFileMmap();
  // Open the file and map the memory.
  bool Open(const char *file_path);
  // Return the start pointer of data. 
  const void* GetDataPointer();
  // Return the file size.
  off_t GetFileSize();

private:
  // file descriptor, get from the open function.
  int file_descriptor_ = 0;
  // the start pointer of data.
  void *data_pointer_ = nullptr;

  struct stat stat_block_;
}; // class ReadFileMmap

} // namespace blue

#endif //READ_FILE_MMAP_H
