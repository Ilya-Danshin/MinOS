#include <game/arkanoid.h>

using namespace myos;
using namespace myos::gui;
using namespace myos::game;

ArkanoidGame::ArkanoidGame(gui::Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b)
: CompositeWidget(parent, x, y, w, h, r, g, b)
{

}

ArkanoidGame::~ArkanoidGame() 
{

}

void ArkanoidGame::OnKeyDown(char c)
{
    switch(c)
    {
        case 'a':
            r = ~r;
            g = ~g;
            b = ~b; 
            break;
        case 'd':
            r = 0xA8;
            g = 0x00;
            b = 0x00;
            break;
        default:
            break;
    }
}