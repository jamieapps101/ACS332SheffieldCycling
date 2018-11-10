#ifndef MAP_HEADER
  #define MAP_HEADER
  #include <vector>
  #include <string>
  class map
  {
  private:
    std::vector<std::vector<int>> mapElevations;
    std::vector<std::vector<int>> mapResolutions;
    int string2int(std::string input);
  public:
    map(std::string inputFile);
    ~map();
    void readMap(std::string inputFile);
    int getElevation(int x, int y);
    std::vector<int> getDimentsions();
    void printMap();
  };
#endif

//Matrix.assign(rows, vector < int >(columns, 0));
