# include "FrameAllocator.h"
# include <fstream>
# include <string>
# include <iostream>


std::vector<std::string> parser(std::string line){
  int pos = 0;
  std::string token;
  std::vector<std::string> tokensOut;
  while ((pos = line.find(" ")) != std::string::npos) {
    token = line.substr(0, pos);
    tokensOut.push_back(token);
    line.erase(0, pos + 1);
  }
  return tokensOut;
}

int main(int argc, char* argv[]){

  // TODO open input from file specified by command line arg
  std::ifstream fileStream(argv[1]);
  if(!fileStream.is_open()){
    throw std::runtime_error("Failed to open file, does it exist?");
  }
  // TODO Read the first line from the file, containing the page frame count.

  std::string line;
  std::getline(fileStream, line);
  uint32_t pages = std::stoi(line);
  FrameAllocator alloc(pages);


  for(std::string line; std::getline(fileStream, line);){
    std::vector<std::string> tokens = parser(line);
    // TODO use tokens to define allocation calls & manage processes (make structs?)
  }

  return(0);
}
