# include "FrameAllocator.h"
# include <fstream>
# include <string>
# include <iostream>


std::vector<std::string> tokenizer(std::string line){

  int pos = 0;
  std::string token;
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

  std::ifstream fileStream(argv[1]);
  if(!fileStream.is_open()){
    throw std::runtime_error("Failed to open file, does it exist?");
  }

  auto printCommandLine = [](std::string line){std::cout << "|" << line <<"\n";};
  auto printAllocResult = [](FrameAllocator &alloc, bool result){
                            uint32_t pagesAvail = alloc.get_available();
                            std::cout << (result ? " T ":" F ")
                                      << std::hex << pagesAvail << "\n";
                          };

  std::string line;
  std::getline(fileStream, line);
  uint32_t pages = std::stoi(line);
  FrameAllocator alloc(pages);

  std::vector<std::vector<uint32_t>> processes;

  printCommandLine(line);

  for(std::string line; std::getline(fileStream, line);){
    std::vector<std::string> tokens = tokenizer(line);
    printCommandLine(line);

    if(tokens[0] == "P"){
      std::cout << alloc.get_available_list_string() <<"\n";
      continue;
    }

    uint32_t processNum = std::stoi(tokens[1]);
    uint32_t pages = std::stoi(tokens[2]);


    if(processes.size() < processNum+1){
      processes.push_back(std::vector<uint32_t>());
    }

    bool result;

    if(tokens[0] == "A"){
      result = alloc.Allocate(pages, processes[processNum]);
    }else if(tokens[0] == "R"){
      result = alloc.Release(pages, processes[processNum]);
    }
    printAllocResult(alloc,result);
  }

  return(0);
}
