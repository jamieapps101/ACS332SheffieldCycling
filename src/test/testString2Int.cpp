#include <stdio.h>
#include <string>
#include <iostream>

float string2float(std::string input);

int main(void)
{
  std::string input = "0.43";
  float output = string2float(input);
  std::cout << "output 1 is " << output << std::endl;
  input = "0394.43";
  output = string2float(input);
  std::cout << "output 2 is " << output << std::endl;
  input = "2394.43";
  output = string2float(input);
  std::cout << "output 3 is " << output << std::endl;
  input = "39t4.43";
  output = string2float(input);
  std::cout << "output 4 is " << output << std::endl;
  input = "34";
  output = string2float(input);
  std::cout << "output 5 is " << output << std::endl;
}

float string2float(std::string input)
{
  int prePointValue = 0;
  int postPointValue = 0;
  bool postPoint = false;
  for(int a = 0; a < input.size(); a++)
  {
    char character = input.at(a);
    if(character >= 48 && character <= 57)
    {
      if(postPoint == false)
      {
        prePointValue *= 10;
        prePointValue += (int)character - 48;
      }
      else
      {
        postPointValue *= 10;
        postPointValue += (int)character - 48;
      }
    }
    else
    {
      if(character == '.') // aka are we at the decimal point.
      {
        postPoint = true;
      }
      else
      {
        return (-1); // else this must be an error
      }
    }
  }
  float value = (float)prePointValue;
  float decimal = (float)postPointValue;
  while(decimal > 1)
  {
    decimal = decimal/10;
  }
  value += decimal;
  return value;
}
