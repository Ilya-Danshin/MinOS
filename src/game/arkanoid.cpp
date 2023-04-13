#include <game/arkanoid.h>
#include <common/random.h>

using namespace myos;
using namespace myos::gui;
using namespace myos::game;
using namespace myos::common;

#define PLATFORM_WIDTH 50
#define PLATFORM_HEIGHT 8

 Platform::Platform(Widget* parent,
                   int32_t x, int32_t y, int32_t w, int32_t h,
                   uint8_t r, uint8_t g, uint8_t b, 
                   uint32_t left_border, uint32_t right_border, int8_t speed)
: CompositeWidget(parent, x, y, w, h, r, g, b)
{
    this->x_left_border = left_border;
    this->x_right_border = right_border;
    this->speed = speed;
    this->mode = 0;
}

Platform::~Platform()
{

}

void Platform::OnKeyDown(char c)
{
    switch(c)
    {
        case 'a':
            if (mode == 0)
                this->MoveLeft();
            break;
        case 'd':
            if (mode == 0)
                this->MoveRight();
            break;
        case 'f':
            if (mode == 0)
                ChangeToBallMode();
            break;
        default:
            break;
    }
}

void Platform::MoveLeft()
{
    if (x > x_left_border) {
        x -= speed;
    }
}

void Platform::MoveRight()
{
    if(x + w < x_right_border) {
        x += speed;
    }
}

void Platform::ChangeToBallMode()
{
    mode++;
    RandomGenerator rand(42);

    int8_t vector_x = 0;
    int8_t vector_y = 0; 

    while(true) {
        int8_t r = rand.Generate() & 0x07;
        switch (r)
        {
            case 0:
                vector_x = -2;
                vector_y = -1;
                break;
            case 0b001: 
                vector_x = -1;
                vector_y = -1;
                break;
            case 0b010:
                vector_x = -1;
                vector_y = -2;
                break;
            case 0b011:
                vector_x = 1;
                vector_y = -2;
                break;
            case 0b100:
                vector_x = 1;
                vector_y = -1;
                break;
            case 0b101:
                vector_x = 2;
                vector_y = -1;
                break;
            default:
                continue;
        }
        break;
    }  

    Widget* ball = new Ball(this, PLATFORM_WIDTH/2, y, 4, 0xFF, 0xFF, 0xFF, vector_x, vector_y);
    this->AddChild(ball);
}

Ball::Ball(Widget* parent,
                   int32_t x, int32_t y, int32_t rad,
                   uint8_t r, uint8_t g, uint8_t b, 
                   int8_t vector_x, int8_t vector_y)
: Widget(parent, x, y, rad, rad, r, g, b)
{
    this->vector_x = vector_x;
    this->vector_y = vector_y;

    this->rad = rad;
}

Ball::~Ball()
{

}

void Ball::Draw(GraphicsContext* gc) 
{
    int32_t X = 0;
    int32_t Y = 0;
    ModelToScreen(X, Y);

    gc->DrawCircle(X, Y, rad, r, g, b);
}


ArkanoidGame::ArkanoidGame(Widget* parent,
                   int32_t x, int32_t y, int32_t w, int32_t h,
                   uint8_t r, uint8_t g, uint8_t b)
: CompositeWidget(parent, x, y, w, h, r, g, b)
{
    platform = new Platform(this, w/2 - PLATFORM_WIDTH/2, h - PLATFORM_HEIGHT - 2, PLATFORM_WIDTH, PLATFORM_HEIGHT, 
    0xFF, 0xFF, 0xFF, x, x + w, 5);
    this->AddChild(platform);
    this->GetFocus(platform);
}

ArkanoidGame::~ArkanoidGame() 
{

}