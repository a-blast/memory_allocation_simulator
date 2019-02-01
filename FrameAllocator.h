# ifndef FRAME_ALLOCATOR_H
# define FRAME_ALLOCATOR_H

# include <vector>
# include <stdint.h>


// NO I/O FROM THIS CLASS
class FrameAllocator {
public:
  FrameAllocator(uint64_t numPageFrames);
  bool Allocate(uint32_t count, std::vector<uint32_t> &page_frames);
  bool Release(uint32_t count, std::vector<uint32_t> &page_frames);
  uint32_t get_available() const;

private:
  std::vector<uint8_t> memory;
};

# endif
