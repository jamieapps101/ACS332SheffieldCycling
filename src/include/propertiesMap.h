#ifndef MAP_HEADER
  #define MAP_HEADER
  #include <vector>
  #include <string>
  class propertiesMap
  {
  private:
    std::vector<std::vector<int>> mapElevations;
    std::vector<std::vector<int>> mapResolutions;
    int string2int(std::string input);
  public:
    propertiesMap();
    ~propertiesMap();
    void readMap(std::string inputFile);
    int getElement(int x, int y);
    std::vector<int> getDimentsions();
    void printMap();
  };
#endif
