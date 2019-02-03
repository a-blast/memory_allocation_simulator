# include "FrameAllocator.h"

# include <stdint.h>
# include <algorithm>
# include <iostream>
# include <bitset>
# include <string>

void FrameAllocator::set_mem_from_uint32(uint32_t index, uint32_t valToSet){
  for(uint8_t uint8Index = 0; uint8Index < 4; uint8Index++){
    uint32_t leftShift = valToSet << (uint8Index*8);
    uint8_t out = leftShift >> 24;
    this->memory[index+uint8Index] = out;

    //prints intermediate bit shifted values
    // std::cout << unsigned(uint8Index) << "+" << unsigned(index) << "\n"
    //           << "i: " << std::bitset<32>(index) << "\n"
    //           << "l: " << std::bitset<32>(leftShift) << "\n"
    //           << "O: " << std::bitset<8>(out) << "\n";
  }
}

uint32_t FrameAllocator::get_uint32_from_mem(uint32_t start) const {
  uint32_t val(0);
  for(int index=0; index<4;index++){

    val |= (this->memory[start+index] << ((3-index)*8));

    // std::cout << std::bitset<8>(memory[start+index]) << " ";

  }
  // std::cout << "\n";
  // std::cout << std::bitset<32>(val) << "?\n";
  return val;
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
  //set_mem_from_uint32(index, ~(uint32_t(0)));
  // set the bits in the tail ref

  uint32_t tailLinkAddr  = (numPageFrames-1)*8192;
  memory[tailLinkAddr]   = ~(uint8_t(0));
  memory[tailLinkAddr+1] = ~(uint8_t(0));
  memory[tailLinkAddr+2] = ~(uint8_t(0));
  memory[tailLinkAddr+3] = ~(uint8_t(0));

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
  return get_uint32_from_mem(page_frames_available_offset);
}


// returns false if there arent enough pages or the page count
// goes over the page_frames_total
bool FrameAllocator::update_avail_page_count(int changeInPageCount){
  uint32_t numPages = get_uint32_from_mem(page_frames_available_offset);
  uint32_t maxPages = get_uint32_from_mem(page_frames_total_offset);

  // cant have negative pages, cant have more than maxPages
  if((0 > numPages+changeInPageCount)
     ||((numPages+changeInPageCount) > maxPages)){return false;};

  set_mem_from_uint32(page_frames_available_offset, (numPages+changeInPageCount));
  return true;
}

bool FrameAllocator::Allocate(uint32_t count, std::vector<uint32_t> &page_frames){
  uint32_t availablePageFrames = get_uint32_from_mem(page_frames_available_offset);
  if(count > availablePageFrames){return false;};
  uint32_t pageAddr = get_uint32_from_mem(available_list_head_offset);

  update_avail_page_count(-count);

  while(count>0){
    page_frames.push_back(pageAddr);
    //std::cout << std::bitset<32>(pageAddr) << "\n";
    //std::cout << std::hex << pageAddr << "!!!\n";
    pageAddr = get_uint32_from_mem(pageAddr);
    count--;
  }
  // Set the head to point at the next available pageAddr
  set_mem_from_uint32(available_list_head_offset, pageAddr);
  return true;
}

bool FrameAllocator::Release(uint32_t count, std::vector<uint32_t> &page_frames){
  if(count>page_frames.size()){return false;};
  update_avail_page_count(count);
  while(count>0){
    uint32_t addrToFree = page_frames.back();
    page_frames.pop_back();
    // insert the newly opened page between the link of the head & the first open page
    uint32_t firstOpenPage = get_uint32_from_mem(available_list_head_offset);
    set_mem_from_uint32(addrToFree, firstOpenPage);
    set_mem_from_uint32(available_list_head_offset, addrToFree);
    count--;
  }
  return true;
}

