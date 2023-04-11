#ifndef __MYOS__GUI__MENU_H
#define __MYOS__GUI__MENU_H

#include <gui/widget.h>

namespace myos
{
    namespace gui
    {

        class Menu : public CompositeWidget 
        {
        private:
            Widget* title; 
            
        public:
            Menu(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~Menu();

            void Draw(common::GraphicsContext* gc);

        };

        class Title : public Widget {
            public:
            Title(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~Title();

            void Draw(common::GraphicsContext* gc);
        };
        
        class MenuPunkt : public Widget 
        {
            public:
            MenuPunkt(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~MenuPunkt();

            void Run();
        };



    }
}



#endif