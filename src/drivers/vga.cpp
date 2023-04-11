#include <drivers/vga.h>
#include <drivers/vincent.h>

using namespace myos::common;
using namespace myos::drivers;

VideoGraphicsArray::VideoGraphicsArray() : 
    miscPort(0x3c2),
    crtcIndexPort(0x3d4),
    crtcDataPort(0x3d5),
    sequencerIndexPort(0x3c4),
    sequencerDataPort(0x3c5),
    graphicsControllerIndexPort(0x3ce),
    graphicsControllerDataPort(0x3cf),
    attributeControllerIndexPort(0x3c0),
    attributeControllerReadPort(0x3c1),
    attributeControllerWritePort(0x3c0),
    attributeControllerResetPort(0x3da)
{
}
  
VideoGraphicsArray::~VideoGraphicsArray()
{
}


            
void VideoGraphicsArray::WriteRegisters(uint8_t* registers)
{
    //  misc
    miscPort.Write(*(registers++));
    
    // sequencer
    for(uint8_t i = 0; i < 5; i++)
    {
        sequencerIndexPort.Write(i);
        sequencerDataPort.Write(*(registers++));
    }
    
    // cathode ray tube controller
    crtcIndexPort.Write(0x03);
    crtcDataPort.Write(crtcDataPort.Read() | 0x80);
    crtcIndexPort.Write(0x11);
    crtcDataPort.Write(crtcDataPort.Read() & ~0x80);
    
    registers[0x03] = registers[0x03] | 0x80;
    registers[0x11] = registers[0x11] & ~0x80;
    
    for(uint8_t i = 0; i < 25; i++)
    {
        crtcIndexPort.Write(i);
        crtcDataPort.Write(*(registers++));
    }
    
    // graphics controller
    for(uint8_t i = 0; i < 9; i++)
    {
        graphicsControllerIndexPort.Write(i);
        graphicsControllerDataPort.Write(*(registers++));
    }
    
    // attribute controller
    for(uint8_t i = 0; i < 21; i++)
    {
        attributeControllerResetPort.Read();
        attributeControllerIndexPort.Write(i);
        attributeControllerWritePort.Write(*(registers++));
    }
    
    attributeControllerResetPort.Read();
    attributeControllerIndexPort.Write(0x20);
    
}

bool VideoGraphicsArray::SupportsMode(uint32_t width, uint32_t height, uint32_t colordepth)
{
    return width == 320 && height == 200 && colordepth == 8;
}

bool VideoGraphicsArray::SetMode(uint32_t width, uint32_t height, uint32_t colordepth)
{
    if(!SupportsMode(width, height, colordepth))
        return false;
    
    unsigned char g_320x200x256[] =
    {
        /* MISC */
            0x63,
        /* SEQ */
            0x03, 0x01, 0x0F, 0x00, 0x0E,
        /* CRTC */
            0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
            0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
            0x9C, 0x0E, 0x8F, 0x28, 0x40, 0x96, 0xB9, 0xA3,
            0xFF,
        /* GC */
            0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
            0xFF,
        /* AC */
            0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
            0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
            0x41, 0x00, 0x0F, 0x00, 0x00
    };
    
    WriteRegisters(g_320x200x256);
    return true;
}


uint8_t* VideoGraphicsArray::GetFrameBufferSegment()
{
    graphicsControllerIndexPort.Write(0x06);
    uint8_t segmentNumber = graphicsControllerDataPort.Read() & (3<<2);
    switch(segmentNumber)
    {
        default:
        case 0<<2: return (uint8_t*)0x00000;
        case 1<<2: return (uint8_t*)0xA0000;
        case 2<<2: return (uint8_t*)0xB0000;
        case 3<<2: return (uint8_t*)0xB8000;
    }
}
            
void VideoGraphicsArray::PutPixel(int32_t x, int32_t y,  uint8_t colorIndex)
{
    if(x < 0 || 320 <= x
    || y < 0 || 200 <= y)
        return;
    
    uint8_t* pixelAddress = GetFrameBufferSegment() + 320*y + x;
    *pixelAddress = colorIndex;
}

uint8_t VideoGraphicsArray::GetColorIndex(uint8_t r, uint8_t g, uint8_t b)
{
    if(r == 0x00 && g == 0x00 && b == 0x00) return 0x00; // black
    if(r == 0x00 && g == 0x00 && b == 0xA8) return 0x01; // blue
    if(r == 0x00 && g == 0xA8 && b == 0x00) return 0x02; // green
    if(r == 0xA8 && g == 0x00 && b == 0x00) return 0x04; // red
    if(r == 0xFF && g == 0xFF && b == 0xFF) return 0x3F; // white
    return 0x00;
}
           
void VideoGraphicsArray::PutPixel(int32_t x, int32_t y,  uint8_t r, uint8_t g, uint8_t b)
{
    PutPixel(x,y, GetColorIndex(r,g,b));
}

void VideoGraphicsArray::FillRectangle(uint32_t x, uint32_t y, uint32_t w, uint32_t h,   uint8_t r, uint8_t g, uint8_t b)
{
    for(int32_t Y = y; Y < y+h; Y++)
        for(int32_t X = x; X < x+w; X++)
            PutPixel(X, Y, r, g, b);
}

 void VideoGraphicsArray::DrawRectangle(common::uint32_t x, common::uint32_t y, common::uint32_t w, common::uint32_t h,  
                                           common::uint8_t r, common::uint8_t g, common::uint8_t b)
{
    // Draw top line
    int32_t Y = y;
    for(int32_t X = x; X < x+w; X++) {
        PutPixel(X, Y, r, g, b);
    }

    // Draw bottom line
    Y += h;
    for(int32_t X = x; X < x+w; X++) {
        PutPixel(X, Y, r, g, b);
    }

    // Draw left line
    int32_t X = x;
    for(int32_t Y = y; Y < y+h; Y++) {
        PutPixel(X, Y, r, g, b);
    }

    // Draw right line
    X += w;
        for(int32_t Y = y; Y < y+h; Y++) {
        PutPixel(X, Y, r, g, b);
    }
}

void VideoGraphicsArray::DrawString(common::string str, common::uint32_t x, common::uint32_t y, common::uint8_t r, common::uint8_t g, common::uint8_t b)
{
    for (int32_t i = 0; str[i] != '\0'; i++) {
        DrawLetter(str[i], x + 8*i, y, r, g, b);
    }
}
void VideoGraphicsArray::DrawLetter(char c, common::uint32_t x, common::uint32_t y, common::uint8_t r, common::uint8_t g, common::uint8_t b)
{
    uint8_t* bitmap = vincent_data[c];

    // font 8x8 so borders are (y + 8) and (x + 8)
    for (int8_t i = 0; i < 8; i++) {
        
        for (int8_t j = 7; j >= 0; j--) {
            if((bitmap[i] >> j) & 0x01) {
                PutPixel(x + 7 - j, y + i, r, g, b);
            }
        }
    }

}