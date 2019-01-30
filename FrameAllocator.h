# ifndef FRAME_ALLOCATOR_H
# define FRAME_ALLOCATOR_H

# include <vector>
# include <stdint.h>


// NO I/O FROM THIS CLASS
class FrameAllocator {
public:
  FrameAllocator();
private:
  std::vector<uint8_t> memory;
};

# endif
