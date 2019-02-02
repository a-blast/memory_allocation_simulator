# ifndef FRAME_ALLOCATOR_H
# define FRAME_ALLOCATOR_H


# include <vector>
# include <stdint.h>
# include <string>

// NO I/O FROM THIS CLASS
class FrameAllocator {
public:
  FrameAllocator(uint64_t numPageFrames);

  virtual ~FrameAllocator();
  
  FrameAllocator(const FrameAllocator &orig) = delete;
  FrameAllocator(FrameAllocator &&orig) = delete;
  FrameAllocator operator=(const FrameAllocator &orig) = delete;
  FrameAllocator operator=(FrameAllocator && orig) = delete;

  bool Allocate(uint32_t count, std::vector<uint32_t> &page_frames);
  bool Release(uint32_t count, std::vector<uint32_t> &page_frames);
  uint32_t get_available() const;
  std::string get_available_list_string() const;

private:
  // first 4 bits: address to next open page in memory (linked list)
  // second 4 bits:
  std::vector<uint8_t> memory;
};

# endif
