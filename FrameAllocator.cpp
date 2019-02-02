# include "FrameAllocator.h"

# include <stdint.h>
# include <algorithm>
# include <iostream>
# include <bitset>

FrameAllocator::FrameAllocator(uint64_t numPageFrames)
  : memory(numPageFrames*8192) {
  for(uint32_t index = 8192; index <= (numPageFrames-1)*8192; index += 8192){
    uint32_t link = 8192+index;
    for(uint8_t uint8Index = 0; uint8Index < 2; uint8Index++){
      uint32_t leftShift = link << (uint8Index*8);
      uint8_t out = leftShift >> 8;
      memory[index+uint8Index] = out;

      // prints intermediate bit shifted values
      // std::cout << unsigned(uint8Index) << "\n"
      //           << "i: " << std::bitset<32>(index) << "\n"
      //           << "l: " << std::bitset<32>(leftShift) << "\n"
      //           << "O: " << std::bitset<8>(out) << "???\n";
    }
    // prints index bits
    // std::cout << std::bitset<8>(memory[index]) << "\n "
    //           << std::bitset<8>(memory[index+1]) << "\n ";
  }
  // set the bits in the tail ref
  uint16_t tailLinkAddr  = numPageFrames*8192;
  memory[tailLinkAddr]   = ~(uint8_t(0));
  memory[tailLinkAddr+1] = ~(uint8_t(0));

  // std::cout << std::bitset<8>(memory[tailLinkAddr]) << "\n"
  //           << std::bitset<8>(memory[tailLinkAddr]) << "\n";
}

uint32_t FrameAllocator::get_available() const {



}
