// NOTE you will have to install google test to make this work.
# include "gtest/gtest.h"
# include <vector>

# include "../FrameAllocator.h"


TEST(FrameAllocator, Construction){
  // Test constructor
  FrameAllocator allocator(5);

  // Test linked list addresses:
  std::vector<uint32_t> llAdr = {8192,16384,24576,32768};
  EXPECT_EQ(llAdr[0],
            allocator.get_uint32_from_mem(allocator.available_list_head_offset));



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

  // test the Allocator
  std::vector<uint32_t> page_frames_out;
  std::vector<uint32_t> page_frames_expected = {8192,16384,24576};
  uint32_t numPages = 3;
  std::cout << page_frames_expected[0]<<"\n";

  allocator.Allocate(numPages, page_frames_out);

}

int main(int argc, char* argv[]){
  ::testing::InitGoogleTest(&argc,argv);
  return RUN_ALL_TESTS();
}
