#ifndef TEST_LIB_H_
#define TEST_LIB_H_

#include <vector>

//A mix-in class for test fixtures that provides some useful constants
class JAGSFixture
{
  public:
    JAGSFixture();

  protected:
    //tolerance for equality tests
    double tol; 
    
    //Logical vectors of length 1
    std::vector<bool> F; //False
    std::vector<bool> T; //True
    
    //Logical vectors of length 2
    std::vector<bool> FF; //False, False
    std::vector<bool> FT; 
    std::vector<bool> TF; 
    std::vector<bool> TT; 
    
    //Logical vectors of length 3
    std::vector<bool> FFF; //False, False, False
    std::vector<bool> FFT; 
    std::vector<bool> FTF; 
    std::vector<bool> FTT; 
    std::vector<bool> TFF; 
    std::vector<bool> TFT; 
    std::vector<bool> TTF; 
    std::vector<bool> TTT; 
};

#endif /* TEST_LIB_H */
