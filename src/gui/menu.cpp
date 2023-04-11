
#include <gui/menu.h>
#include <drivers/vga.h>
#include <memorymanagement.h>

using namespace myos;
using namespace myos::common;
using namespace myos::gui;


Menu::Menu(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b)
: CompositeWidget(parent, x, y, w, h, r, g, b)
{
    int32_t X = 0;
    int32_t Y = 0;
    this->ModelToScreen(X, Y);
    title = new Title(this, (X+w)/2, Y, w, 20, 0xFF, 0xFF, 0xFF);
    this->AddChild(title);
}

Menu::~Menu() 
{
    delete (void*)title;
}

void Menu::Draw(common::GraphicsContext* gc){
    CompositeWidget::Draw(gc);
    gc->DrawRectangle(x, y, w, h, 0xFF, 0xFF, 0xFF);
}

Title::Title(Widget* parent,
            common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
            common::uint8_t r, common::uint8_t g, common::uint8_t b) 
: Widget(parent, x, y, w, h, r, g, b)
{
}

Title::~Title()
{
}

void Title::Draw(common::GraphicsContext* gc) 
{
    gc->DrawString("Menu", x, y+h/2, r, g, b);
}

MenuPunkt::MenuPunkt(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b)
:Widget(parent, x, y, w, h, r, g, b)
{
}

MenuPunkt::~MenuPunkt()
{
}
