#include <game/arkanoid.h>
#include <common/random.h>
#include <drivers/timer.h>

using namespace myos;
using namespace myos::gui;
using namespace myos::game;
using namespace myos::common;
using namespace myos::drivers;

#define PLATFORM_WIDTH 50
#define PLATFORM_HEIGHT 8
#define PLATFORM_SPEED 5

#define BALL_RADIUS 4
#define BALL_SPEED 4

extern uint64_t tick;

 Panel::Panel(Widget* parent,
                   int32_t x, int32_t y, int32_t w, int32_t h,
                   uint8_t r, uint8_t g, uint8_t b, bool is_physical)
: Widget(parent, x, y, w, h, r, g, b)
{
    this->is_physical = is_physical;
}

Panel::~Panel()
{

}

 bool Panel::IsHit(int32_t ball_x, int32_t ball_y, int32_t ball_rad, bool& isXHit)
 {
    if (!is_physical)
        return false;

    if (ContainsCoordinate(ball_x - ball_rad, ball_y)) {
        isXHit = true;
        return true;
    }

    if (ContainsCoordinate(ball_x + ball_rad, ball_y)) {
        isXHit = true;
        return true;
    }

    if (ContainsCoordinate(ball_x, ball_y - ball_rad)) {
        isXHit = false;
        return true;
    }

    if (ContainsCoordinate(ball_x, ball_y + ball_rad)) {
        isXHit = false;
        return true;
    }

    return false;
 }

TargetPanel::TargetPanel(Widget* parent,
                   int32_t x, int32_t y, int32_t w, int32_t h,
                   uint8_t r, uint8_t g, uint8_t b, bool is_physical, uint32_t lifes)
: Panel(parent, x, y, w, h, r, g, b, is_physical)
{
    this->lifes = lifes;
}

TargetPanel::~TargetPanel()
{

}

bool TargetPanel::IsHit(common::int32_t ball_x, common::int32_t ball_y, common::int32_t ball_rad, bool& isXHit)
{
    if(Panel::IsHit(ball_x, ball_y, ball_rad, isXHit)){
        LifesDecrement();
        return true;
    }

}

void TargetPanel::LifesDecrement()
{
    lifes--;
    //if (lifes < 0)
    //{
    //    delete this;
    //}
}


 Platform::Platform(Widget* parent,
                   int32_t x, int32_t y, int32_t w, int32_t h,
                   uint8_t r, uint8_t g, uint8_t b, bool is_physical,
                   uint32_t left_border, uint32_t right_border, int8_t speed)
: Panel(parent, x, y, w, h, r, g, b, is_physical)
{
    this->x_left_border = left_border;
    this->x_right_border = right_border;
    this->speed = speed;
}

Platform::~Platform()
{

}

void Platform::OnKeyDown(char c)
{
    switch(c)
    {
        case 'a':
            this->MoveLeft();
            break;
        case 'd':
            this->MoveRight();
            break;
        case 'f':
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

    int32_t X = 0;
    int32_t Y = 0;
    ModelToScreen(X, Y);

    Widget* ball = new Ball(this->parent, X + PLATFORM_WIDTH/2, Y-BALL_RADIUS, BALL_RADIUS, 0xFF, 0xFF, 0xFF, vector_x, vector_y, BALL_SPEED);
    ((CompositeWidget*)(this->parent))->AddChild(ball);
    ((ArkanoidGame*)(this->parent))->SetBall((Ball*)ball);
}

Ball::Ball(Widget* parent,
                   int32_t x, int32_t y, int32_t rad,
                   uint8_t r, uint8_t g, uint8_t b, 
                   int8_t vector_x, int8_t vector_y, int8_t speed)
: Widget(parent, x, y, rad, rad, r, g, b)
{
    this->vector_x = vector_x;
    this->vector_y = vector_y;

    this->rad = rad;
    this->speed = speed;
}

Ball::~Ball()
{

}

void Ball::Draw(GraphicsContext* gc) 
{
    //int32_t X = 0;
    //int32_t Y = 0;
    //ModelToScreen(X, Y);

    //gc->DrawCircle(X, Y, rad, r, g, b);
    Widget::Draw(gc);

    Move();
}

void Ball::Move()
{
    x += speed * vector_x;
    y += speed * vector_y;
}

int32_t Ball::GetX()
{
    return this->x;
}

int32_t Ball::GetY()
{
    return this->y;
}

int32_t  Ball::GetRad()
{
    return this->rad;
}

void Ball::ChangeVectorX()
{
    vector_x *= -1;
}

void Ball::ChangeVectorY()
{
     vector_y *= -1;
}

ArkanoidGame::ArkanoidGame(Widget* parent,
                   int32_t x, int32_t y, int32_t w, int32_t h,
                   uint8_t r, uint8_t g, uint8_t b)
: CompositeWidget(parent, x, y, w, h, r, g, b)
{
    // Create moving platform
    platform = new Platform(this, w/2 - PLATFORM_WIDTH/2, h - PLATFORM_HEIGHT - 2, PLATFORM_WIDTH, PLATFORM_HEIGHT, 
    0xFF, 0xFF, 0xFF, true, x, x + w, PLATFORM_SPEED);
    this->AddChild(platform);
    this->GetFocus(platform);
    platforms[num_of_platforms] = (Panel*)platform;
    num_of_platforms++;

    // Create panels
    TargetPanel* p1 = new TargetPanel(this, w/2, 10, 40, 10, 0xA8, 0x00, 0x00, true, 1);
    this->AddChild(p1);
    platforms[num_of_platforms] = (Panel*)p1;
    num_of_platforms++;
}

ArkanoidGame::~ArkanoidGame() 
{

}

void ArkanoidGame::Draw(common::GraphicsContext* gc)
{
    CompositeWidget::Draw(gc);
    
    int32_t X = ball->GetX();
    int32_t Y = ball->GetY();
    int32_t rad = ball->GetRad();

    if (X <= 0 || X >= w) 
    {
        ball->ChangeVectorX();
        return;
    }  

    if (Y <= 0) 
    {
        ball->ChangeVectorY();
        return;
    }
    
    for (int i = 0; i < this->num_of_platforms; i++) {
        bool IsXHit;
        if(platforms[i]->IsHit(X, Y, rad, IsXHit)) {
            if (IsXHit) {
                ball->ChangeVectorX();
                return;
            } else {
                ball->ChangeVectorY();
                return;
            }
        }
    }


}

void ArkanoidGame::SetBall(Ball* ball)
{
    this->ball = ball;
}