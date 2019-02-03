// NOTE you will have to install google test to make this work.
# include "gtest/gtest.h"
# include <vector>

# include "../FrameAllocator.h"


TEST(FrameAllocator, Construction){
  // Test constructor
  uint32_t pageNumber = 5;
  FrameAllocator allocator(pageNumber);

  // Test linked list addresses:
  std::vector<uint32_t> llAdr = {8192,16384,24576,32768};
  EXPECT_EQ(llAdr[0],
            allocator.get_uint32_from_mem(allocator.available_list_head_offset));

  // Test pageCountUpdate
  // By default, class should update page count on construction
  EXPECT_EQ(pageNumber-1,
            allocator.get_uint32_from_mem(allocator.page_frames_available_offset));
  // Test updating the page number manually,
  // should not update to page count greater than maxPages or less than 0
  EXPECT_EQ(false, allocator.update_avail_page_count(pageNumber+10));
  EXPECT_EQ(false, allocator.update_avail_page_count(-pageNumber-10));

  EXPECT_EQ(true, allocator.update_avail_page_count(+1));
  EXPECT_EQ(true, allocator.update_avail_page_count(-3));
  EXPECT_EQ(2,allocator.get_uint32_from_mem(allocator.page_frames_available_offset));

  // Test that the memory vector is properly initialized
  // & can be checked via get_available

  // test setting memory from a uint32_t
  uint32_t valToSet = ~(uint32_t(0));
  uint8_t valToTest = ~(uint8_t(0));
  uint32_t index = 0;
  allocator.set_mem_from_uint32(index, valToSet);

  EXPECT_EQ(valToTest,allocator.memory[0]);
  EXPECT_EQ(valToTest,allocator.memory[1]);
  EXPECT_EQ(valToTest,allocator.memory[2]);
  EXPECT_EQ(valToTest,allocator.memory[3]);

  // test page zero initialization
  allocator.set_page_zero(valToSet,valToSet,valToSet);
  for(int i = 0; i<12; i++){
    EXPECT_EQ(valToTest, allocator.memory[i]);
  };

  // test get_uint32 from memory
  EXPECT_EQ(valToSet, allocator.get_uint32_from_mem(0));

  // Define a new FrameAllocator after messing with the old one
  FrameAllocator allocator2(8);
  // test the Allocator
  std::vector<uint32_t> page_frames_out;
  std::vector<uint32_t> page_frames_expected = {8192,16384,24576};
  uint32_t numPagesAllocate = 3;

  EXPECT_EQ(true, allocator2.Allocate(numPagesAllocate, page_frames_out));
  EXPECT_EQ(page_frames_expected, page_frames_out);

  // test the Releaser
  uint32_t numPagesRelease = 1;
  page_frames_expected.pop_back();
  EXPECT_EQ(true, allocator2.Release(numPagesRelease, page_frames_out));
  EXPECT_EQ(page_frames_expected, page_frames_out);

  // test the get_available_list_string function post release
  std::string expectedString = " 6000 8000 a000 c000 e000";
  EXPECT_EQ(expectedString, allocator2.get_available_list_string());




}

int main(int argc, char* argv[]){
  ::testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
