#ifndef SDHelper_h
#define SDHelper_h


#include <inttypes.h>


class SDHelper
{
  public:
    SDHelper();

    void begin(void);
    bool isCard();
    String cardType();
    bool writeFile(const char * path, String message);
    bool appendFile(const char * path, String message);
    String fileToString(const char * path);
    bool createDir(const char * path);
    bool removeDir(const char * path);
    bool renameFile(const char * path1, const char * path2);
    bool deleteFile(const char * path);
  private:
    bool isCardIn;
};

#endif //SDHelper_h
