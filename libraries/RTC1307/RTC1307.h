#ifndef RTC1307_h
#define RTC1307_h


#include <inttypes.h>


class RTC1307
{
  public:
    RTC1307();

    void begin(void);
    bool isRtc();
    void start(void);
    void stop(void);
    void get(int *sec, int *min, int *hour, int *day, int *month, int *year);
    String dateTimeFile();
    String dateYYYMMDD();
    String date();
    String time();
    void set(uint8_t sec, uint8_t min, uint8_t hour, uint8_t day, uint8_t month, uint16_t year);
    void set(int sec, int min, int hour, int day, int month, int year);

  private:
    String LeftPadZero(String s,int len);
    String Repeat(String s,int n);
    uint8_t bin2bcd(uint8_t val);
    uint8_t bcd2bin(uint8_t val);
};


#endif //RTC1307_h
