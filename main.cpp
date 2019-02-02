# include "FrameAllocator.h"



int main(int argc, char* argv[]){

  // TODO open input from file specified by command line arg

  // TODO Read the first line from the file, containing the page frame count.

  // TODO create FrameAllocator object w/ the number of page frames

  // TODO map over the input file lines and parse the commands
  // ----> A to alloate page frames
  // ----> R to release page frames
  // ----> p to print the contents of the available list

  FrameAllocator alloc(3);
  

  return(0);
}
