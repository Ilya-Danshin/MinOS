#include <gui/desktop.h>

using namespace myos;
using namespace myos::common;
using namespace myos::gui;


Desktop::Desktop(common::int32_t w, common::int32_t h,
                 common::uint8_t r, common::uint8_t g, common::uint8_t b)
:   CompositeWidget(0,0,0, w,h,r,g,b),
    MouseEventHandler()
{
    // Create obj menu
    // GetFocus(menu) 
}

Desktop::~Desktop()
{   
}

void Desktop::Draw(common::GraphicsContext* gc)
{
    CompositeWidget::Draw(gc);
}
            
void Desktop::OnKeyDown(char c)
{
    CompositeWidget::OnKeyDown(c);    
}
