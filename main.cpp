# include "FrameAllocator.h"
# include <fstream>
# include <string.h>
# include <iostream>
# include <sstream>
# include <map>

// breaks input lines into  token vectors
std::vector<std::string> tokenizer(std::string line){

  int                      pos = 0;
  std::string              token;
  std::vector<std::string> tokensOut;

  if(line.size() == 1){
    tokensOut.push_back(line);
    return tokensOut;
  }

  while ((pos = line.find(" ")) != std::string::npos) {
    token = line.substr(0, pos);
    tokensOut.push_back(token);
    line.erase(0, pos + 1);
  }

  tokensOut.push_back(line);
  return tokensOut;
}

int main(int argc, char* argv[]){

  // open the stream
  std::ifstream fileStream(argv[1]);
  if(!fileStream.is_open()){
    throw std::runtime_error("Failed to open file, does it exist?");
  }

  // get hex number from a hex string 
  auto getHex = [](std::string hexString){
                  std::stringstream hexStream;
                  hexStream << std::hex << hexString;
                  uint32_t val;
                  hexStream >> val;
                  return val;
                };
  // print a command to the appropriate format
  auto printCommandLine = [](std::string line){std::cout << "|" << line <<"\n";};
  // print the result of the allocation/release
  auto printAllocResult = [](FrameAllocator &alloc, bool result){
                            uint32_t pagesAvail = alloc.get_available();
                            std::cout << (result ? " T ":" F ")
                                      << std::hex << pagesAvail << "\n";
                          };

  // interpret the first line of the file & create the allocator
  std::string    line;
  std::getline(fileStream, line);
  uint32_t       pages = getHex(line);
  FrameAllocator alloc(pages);
  std::map<uint32_t,std::vector<uint32_t> > processes;
  printCommandLine(line);

  // Iterate over the rest of the file
  for(std::string line; std::getline(fileStream, line);){

    // tokenize the input
    std::vector<std::string> tokens = tokenizer(line);
    printCommandLine(line);

    // handle print statements
    if(tokens[0] == "P"){
      std::cout << alloc.get_available_list_string() <<"\n";
      continue;
    }

    uint32_t processNum = getHex(tokens[1]);
    uint32_t pages      = getHex(tokens[2]);

    if(processes.count(processNum)){
      processes.insert(std::pair<uint32_t,std::vector<uint32_t> >(processNum,
                                                                  std::vector<uint32_t>()
                                                                  ));
    }

    bool result;

    (tokens[0] == "A")?
      result = alloc.Allocate(pages, processes[processNum]):
      result = alloc.Release(pages, processes[processNum]);

    printAllocResult(alloc,result);
  }

  return(0);
}
