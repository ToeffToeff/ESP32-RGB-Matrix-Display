#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define rmask 0b0000000000001111
#define gmask 0b0000000011110000
#define bmask 0b0000111100000000

struct color
{
  public:
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

class Framebuffer
{
  private:
    struct color **buffer;
    uint8_t readPos = 0;
    uint8_t writePos = 0;
    uint8_t nextWritePos = 0;
    const uint8_t bufferSize;
    const uint32_t myWidth;
    const uint32_t myHeight;
    uint32_t frameSize;

    void inc(uint8_t &a);
    void setNextWritePos();

  public:
    Framebuffer(uint8_t bufferSize, uint32_t width, uint32_t height);
    Framebuffer();
    ~Framebuffer();

    void setPixel(int16_t x, int16_t y, uint16_t c);
    void frameWriteComplete();
    struct color* getPixel(uint16_t y, uint16_t x);
    void frameReadComplete();
};