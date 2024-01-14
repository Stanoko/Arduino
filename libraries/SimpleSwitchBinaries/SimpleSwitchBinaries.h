#ifndef SimpleSwitchBinaries_h
#define SimpleSwitchBinaries_h


#include <inttypes.h>


class SimpleSwitchBinaries
{
  public:
    SimpleSwitchBinaries();
    void updateFromSD(String returnBinary);
    void changeSketch(String binFile);
  private:
    void performUpdate(Stream &updateSource, size_t updateSize);
    bool loadFromSD(String binFile);
};


#endif //SimpleSwitchBinaries_h
