#ifndef __MYOS__GUI__MENU_H
#define __MYOS__GUI__MENU_H

#include <gui/widget.h>

#define MAX_PUNKTS 8

namespace myos
{
    namespace gui
    {

        class MenuPunkt : public Widget 
        {
            public:
            MenuPunkt(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~MenuPunkt();

            virtual void Run() = 0;
        };

        class ShutdownPunkt : public MenuPunkt 
        {
            public:
            ShutdownPunkt(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~ShutdownPunkt() ;

            void Draw(common::GraphicsContext* gc);
            void Run() override;
        };

        class GamePunkt : public MenuPunkt 
        {
        public:
            GamePunkt(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~GamePunkt();

            void Draw(common::GraphicsContext* gc);
            void Run() override;
        };

        class Cursor : public Widget 
        {
        
        private:
            common::int8_t prev_punkt = 0;
        
        public:
            Cursor(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~Cursor();

            void Draw(common::GraphicsContext* gc);
            void MoveCursor(common::int8_t current_punkt);
        };
        
        class Menu : public CompositeWidget 
        {
        private:
            Widget* title; 
            MenuPunkt* punkts[MAX_PUNKTS];
            Cursor* cursor;

            common::int8_t num_of_punkts = 0;
            common::int8_t cur_punkt = 0;
            
        public:
            Menu(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~Menu();

            void Draw(common::GraphicsContext* gc);
            void OnKeyDown(char c);
        };

        class Title : public Widget {
            public:
            Title(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~Title();

            void Draw(common::GraphicsContext* gc);
        };
    

    }
}



#endif