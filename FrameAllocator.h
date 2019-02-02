# ifndef FRAME_ALLOCATOR_H
# define FRAME_ALLOCATOR_H


# include <vector>
# include <stdint.h>
# include <string>

// NO I/O FROM THIS CLASS
class FrameAllocator {
public:
  FrameAllocator(uint32_t numPageFrames);

  //virtual ~FrameAllocator();
  
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
  static const int page_frames_total_offset = 0;
  static const int page_frames_available_offset = 4;
  static const int available_list_head_offset = 8;

  // helper functions
  // accessible by gtest unit testing class
  friend class FrameAllocator_Construction_Test;

  void set_page_zero(uint32_t page_frames_total,
                     uint32_t page_frames_available,
                     uint32_t available_list_head
                     );


  void set_mem_from_uint32(uint32_t index, uint32_t valToSet);
  uint32_t get_uint32_from_mem(uint32_t index) const;

};

# endif
