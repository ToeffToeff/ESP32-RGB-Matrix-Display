#include "Framebuffer.h"

Framebuffer::Framebuffer(uint8_t bufferSize, uint32_t width, uint32_t height) : bufferSize(bufferSize), myWidth(width), myHeight(height)
{
    if (bufferSize < 2)
        bufferSize = 2;

    frameSize = width * height;

    buffer = new struct color *[bufferSize];
    for (int i = 0; i < bufferSize; i++)
    {
        buffer[i] = new struct color[frameSize]();
    }
}

Framebuffer::Framebuffer() : bufferSize(4), myWidth(64), myHeight(32)
{
    frameSize = myWidth * myHeight;

    buffer = new struct color *[bufferSize];
    for (int i = 0; i < bufferSize; i++)
    {
        buffer[i] = new struct color[frameSize];
    }
}

Framebuffer::~Framebuffer()
{
    //#TODO: delete arrays
}
/*
struct color *Framebuffer::getWriteP()
{
    struct color *temp = buffer[writePos];
    inc(readPos);
    return temp;
}
*/
void Framebuffer::setPixel(int16_t x, int16_t y, uint16_t c)
{
    struct color *pixel = buffer[writePos];
    pixel[myWidth * y + x].r = ((c & rmask)) << 4;
    pixel[myWidth * y + x].g = ((c & gmask));
    pixel[myWidth * y + x].b = ((c & bmask) >> 4);
}

void Framebuffer::frameWriteComplete()
{
    setNextWritePos();
}

void Framebuffer::frameReadComplete()
{
    inc(readPos);
}

struct color *Framebuffer::getPixel(uint16_t y, uint16_t x)
{
    return &buffer[writePos][myWidth * y + x];
}

void Framebuffer::inc(uint8_t &a)
{
    a++;
    if (a >= bufferSize)
        a = 0;
    if (a = readPos)
        a++;
}

void Framebuffer::setNextWritePos()
{
    if ((readPos > 0) && (readPos < bufferSize))
    {
        nextWritePos = readPos - 1;
    }
    else if (readPos = 0)
    {
        nextWritePos = bufferSize - 1;
    }
}
