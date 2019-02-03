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
  return tokensOut;
}

int main(int argc, char* argv[]){

  std::ifstream fileStream(argv[1]);
  if(!fileStream.is_open()){
    throw std::runtime_error("Failed to open file, does it exist?");
  }

  auto printLine = [](std::string line){std::cout << "|" << line <<"\n";};
  std::string line;
  std::getline(fileStream, line);
  uint32_t pages = std::stoi(line);
  FrameAllocator alloc(pages);

  std::vector<std::vector<uint32_t>> processes;

  printLine(line);

  for(std::string line; std::getline(fileStream, line);){
    std::vector<std::string> tokens = tokenizer(line);
    printLine(line);
    std::cout << tokens[0] << "\n";
    //std::cout << tokens.size() <<"\n";
    if(tokens[0] == "P"){
      std::cout << alloc.get_available_list_string() <<"\n";
    }

  }

  return(0);
}
