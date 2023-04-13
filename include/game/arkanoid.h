#ifndef __MYOS__GAME__ARKANOID_H
#define __MYOS__GAME__ARKANOID_H

#include <gui/widget.h>

namespace myos
{
    namespace game
    {
        
        class ArkanoidGame : public gui::CompositeWidget
        {
        public:
            ArkanoidGame(gui::Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~ArkanoidGame();

            void OnKeyDown(char c);

        private:

        };
    }
}


#endif