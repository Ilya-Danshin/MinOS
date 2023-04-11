#ifndef __MYOS__GUI__DESKTOP_H
#define __MYOS__GUI__DESKTOP_H

#include <gui/widget.h>
#include <drivers/mouse.h>

namespace myos
{
    namespace gui
    {
        
        class Desktop : public CompositeWidget, public myos::drivers::MouseEventHandler
        {
        protected:
            CompositeWidget* menu;
        public:
            Desktop(common::int32_t w, common::int32_t h,
                    common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~Desktop();
            
            void Draw(common::GraphicsContext* gc);
            
            void OnKeyDown(char c);
        };

    }
}


#endif