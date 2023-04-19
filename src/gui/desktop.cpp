#include <gui/desktop.h>
#include <gui/menu.h>
#include <drivers/vga.h>
#include <memorymanagement.h>

using namespace myos;
using namespace myos::common;
using namespace myos::gui;

Desktop::Desktop(common::int32_t w, common::int32_t h,
                 common::uint8_t r, common::uint8_t g, common::uint8_t b)
:   CompositeWidget(0, 0, 0, w, h, r, g, b),
    MouseEventHandler()
{
    // Create obj menu
    menu = new Menu(this, (w-200)/2, (h-100)/2, 200, 100, 0x00, 0x00, 0xA8);
    this->AddChild(menu);
    GetFocus(menu);
}

Desktop::~Desktop()
{   
    delete (void*)menu;
}

void Desktop::Draw(common::GraphicsContext* gc)
{
    CompositeWidget::Draw(gc);
}
            
void Desktop::OnKeyDown(char c)
{
    CompositeWidget::OnKeyDown(c);    
}
