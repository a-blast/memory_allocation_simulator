# include "FrameAllocator.h"

# include <stdint.h>
# include <algorithm>
# include <iostream>
# include <iomanip>
# include <sstream>
# include <bitset>
# include <string>

void FrameAllocator::set_mem_from_uint32(uint32_t index, uint32_t valToSet){
  for(uint8_t uint8Index = 0; uint8Index < 4; uint8Index++){
    uint32_t leftShift = valToSet << (uint8Index*8);
    uint8_t out = leftShift >> 24;
    this->memory[index+uint8Index] = out;
  }
}

uint32_t FrameAllocator::get_uint32_from_mem(uint32_t start) const {
  uint32_t val(0);
  for(int index=0; index<4;index++){
    val |= (this->memory[start+index] << ((3-index)*8));
  }
  return val;
}

FrameAllocator::FrameAllocator(uint32_t numPageFrames)
  : memory(numPageFrames*16384) {
  for(uint32_t index = 16384; index <= (numPageFrames-1)*16384; index += 16384){
    uint32_t link = 16384+index;
    this->set_mem_from_uint32(index, link);
  }
  uint32_t tailLinkAddr  = (numPageFrames-1)*16384;
  set_mem_from_uint32(tailLinkAddr, ~(uint32_t(0)));

  set_page_zero(numPageFrames, numPageFrames-1, 16384);
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

std::string FrameAllocator::get_available_list_string() const {
  uint32_t nextPage = get_uint32_from_mem(available_list_head_offset);
  std::stringstream out;
  while(nextPage != ~(uint32_t(0))){
    out << " " << std::hex << nextPage;
    nextPage = get_uint32_from_mem(nextPage);
  }
  return out.str();
}

// returns false if there arent enough pages or the page count
// goes over the page_frames_total
bool FrameAllocator::update_avail_page_count(signed int changeInPageCount){
  uint32_t numPages = get_uint32_from_mem(page_frames_available_offset);
  uint32_t maxPages = get_uint32_from_mem(page_frames_total_offset);

  // cant have negative pages, cant have more than maxPages
  if((0 > signed(numPages)+changeInPageCount)
     ||((numPages+changeInPageCount) > maxPages)){return false;};

  set_mem_from_uint32(page_frames_available_offset, (numPages+changeInPageCount));
  return true;
}

bool FrameAllocator::Allocate(uint32_t count, std::vector<uint32_t> &page_frames){
  uint32_t availablePageFrames = get_uint32_from_mem(page_frames_available_offset);
  if(count > availablePageFrames){return false;};
  uint32_t pageAddr = get_uint32_from_mem(available_list_head_offset);

  update_avail_page_count(-signed(count));

  while(count>0){
    page_frames.push_back(pageAddr);
    pageAddr = get_uint32_from_mem(pageAddr);
    count--;
  }
  // Set the head to point at the next available pageAddr
  set_mem_from_uint32(available_list_head_offset, pageAddr);
  return true;
}

bool FrameAllocator::Release(uint32_t count, std::vector<uint32_t> &page_frames){

  if(count>page_frames.size()){return false;};
  update_avail_page_count(signed(count));
  while(count>0){
    uint32_t addrToFree = page_frames.back();
    page_frames.pop_back();
    uint32_t firstOpenPage = get_uint32_from_mem(available_list_head_offset);
    set_mem_from_uint32(addrToFree, firstOpenPage);
    set_mem_from_uint32(available_list_head_offset, addrToFree);
    count--;
  }
  return true;
}

