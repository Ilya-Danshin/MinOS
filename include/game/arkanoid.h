#ifndef __MYOS__GAME__ARKANOID_H
#define __MYOS__GAME__ARKANOID_H

#include <gui/widget.h>
#include <common/graphicscontext.h>

namespace myos
{
    namespace game
    {
        
        class Platform : public gui::Widget
        {
        public:
            Platform(gui::Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b, 
                   common::uint32_t left_border, common::uint32_t right_border, common::int8_t speed);
            ~Platform();

            void OnKeyDown(char c);
        private:
            common::uint32_t x_left_border;
            common::uint32_t x_right_border;
            
            common::int8_t speed;

            void MoveLeft();
            void MoveRight();

            void ChangeToBallMode();
        };

        
        // TODO: Create common class for game platform (that user move) and for palatforms-enemy

        class Ball : public gui::Widget
        {
        public:
            Ball(gui::Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t rad,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b, 
                   common::int8_t vector_x, common::int8_t vector_y, common::int8_t speed);
            ~Ball();

            void Draw(common::GraphicsContext* gc);

            void MoveHandler();

            common::int32_t GetX();
            common::int32_t GetY();
            common::int32_t GetRad();

            void ChangeVectorX();
            void ChangeVectorY();
        private:
            common::int8_t vector_x;
            common::int8_t vector_y;
            common::int32_t rad;

            common::int8_t speed;

            common::uint64_t cnt = 0;
        };
        
        class ArkanoidGame : public gui::CompositeWidget
        {
        public:
            ArkanoidGame(gui::Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b);
            ~ArkanoidGame();

            void Draw(common::GraphicsContext* gc);

            void SetBall(Ball* ball);
        private:
            Platform* platform;
            Ball* ball;
        };
    }
}


#endif