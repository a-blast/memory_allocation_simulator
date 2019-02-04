# memory_allocation_simulator

### Code to run the main file (on the second test file)

g++ -g -Wall -Wextra -std=c++14 main.cpp FrameAllocator.* -o out  && ./out tests/data/allocator2.txt

### Code to run the gtest commands: (from the gtest folder)

g++ -g -Wall -Wextra -std=c++14 main_test.cpp ../FrameAllocator.* -o out -lgtest -lpthread && ./out
