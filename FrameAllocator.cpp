# include "FrameAllocator.h"

# include <stdint.h>
# include <algorithm>
# include <iostream>
# include <bitset>

void FrameAllocator::set_mem_from_uint32(uint32_t index, uint32_t valToSet){
  for(uint8_t uint8Index = 0; uint8Index < 4; uint8Index++){
    uint32_t leftShift = valToSet << (uint8Index*8);
    uint8_t out = leftShift >> 24;
    this->memory[index+uint8Index] = out;

    //prints intermediate bit shifted values
    // std::cout << unsigned(uint8Index) << "\n"
    //           << "i: " << std::bitset<32>(index) << "\n"
    //           << "l: " << std::bitset<32>(leftShift) << "\n"
    //           << "O: " << std::bitset<8>(out) << "\n";
  }
}



FrameAllocator::FrameAllocator(uint32_t numPageFrames)
  : memory(numPageFrames*8192) {
  for(uint32_t index = 8192; index <= (numPageFrames-1)*8192; index += 8192){
    uint32_t link = 8192+index;
    this->set_mem_from_uint32(index, link);
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
  set_page_zero(numPageFrames, numPageFrames-1, 8192);


  // for(int index = 0; index <= 16; index++){
  //   std::cout << std::bitset<8>(memory[index]);
  //   if(((index+1)%4)==0){ std::cout << "\n";};
  // };

}

void FrameAllocator::set_page_zero(uint32_t page_frames_total,
                                   uint32_t page_frames_available,
                                   uint32_t available_list_head){
  set_mem_from_uint32(page_frames_total_offset, page_frames_total);
  set_mem_from_uint32(page_frames_available_offset, page_frames_available);
  set_mem_from_uint32(available_list_head_offset, available_list_head);
}

uint32_t FrameAllocator::get_available() const {

  

}
