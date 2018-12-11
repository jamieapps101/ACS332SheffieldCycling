#include <fstream>

int main()
{
  std::vector<std::vector<double>> runInitData;
  std::ifstream inputRefFile("outputReference.csv");
  int lineNumber = -1;
  std::string line;
  while(std::getline(inputRefFile, line))
  {
    lineNumber++;
    if(lineNumber != 0) // first line is full of headers
    {
      while(line.length() > 2)
      {
        float inputNumber;
        std::size_t commaPos = line.find_first_of(",");
        std::string stringNum = line.substr(0,commaPos-1);
        line = line.substr(commaPos+1,line.length());
        std::size_t nextChar;
        inputNumber = std::stof(stringNum, &nextChar);
      }
    }
  }
}
