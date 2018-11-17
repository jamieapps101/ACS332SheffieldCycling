#ifndef MAP_HEADER
  #define MAP_HEADER
  #include <vector>
  #include <string>
  class propertiesMap
  {
  private:
    std::vector<std::vector<int>> mapElementsMatrix;
    int string2int(std::string input);
    float string2float(std::string input);
  public:
    propertiesMap();
    ~propertiesMap();
    void readMap(std::string inputFile);
    void readMap(std::string inputFile,std::string keyFile);
    int getElement(int x, int y);
    std::vector<int> getDimensions();
    void printMap();
  };
#endif
