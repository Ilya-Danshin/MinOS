#include <game/arkanoid.h>
#include <common/random.h>
#include <drivers/timer.h>
#include <gui/menu.h>

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

WinScreen::WinScreen(gui::Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b)
: Widget(parent, x, y, w, h, r, g, b)
{

}

WinScreen::~WinScreen()
{
}

void WinScreen::Draw(common::GraphicsContext* gc)
{
    Widget::Draw(gc);

    gc->DrawString("Win!", w/2 - 16, h/2, 0xFF, 0xFF, 0xFF);
}

void WinScreen::OnKeyDown(char c)
{
    if (c == '\n')
    {
        //((CompositeWidget*)this->parent)->DeleteChild(this);

        //Widget* menu = new Menu(this, (w-200)/2, (h-100)/2, 200, 100, 0x00, 0x00, 0xA8);
        //((CompositeWidget*)this->parent)->AddChild(menu);
        //((CompositeWidget*)this->parent)->GetFocus(menu);

        //delete (void*)this;
    }
}

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

 bool Panel::IsPhysical()
 {
    return this->is_physical;
 }

TargetPanel::TargetPanel(Widget* parent,
                   int32_t x, int32_t y, int32_t w, int32_t h,
                   uint8_t r, uint8_t g, uint8_t b,
                    bool is_physical, uint32_t lifes)
: Panel(parent, x, y, w, h, r, g, b, is_physical)
{
    // 1 life - white
    // 2 lifes - green
    // 3 lifes - blue
    // 4 lifes - red
    // 0 lifes - black & is_physical = false

    this->lifes = lifes;
    ChangeColor();
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
    return false;
}

void TargetPanel::LifesDecrement()
{
    this->lifes--;
    if (lifes == 0) {
        this->is_physical = false;
    }

    ChangeColor();
}

void TargetPanel::ChangeColor()
{
    switch(lifes)
        {
        case 1:
            r = 0xFF;
            g = 0xFF;
            b = 0xFF;
            break;
        case 2:
            r = 0x00;
            g = 0xA8;
            b = 0x00;
            break;
        case 3:
            r = 0x00;
            g = 0x00;
            b = 0xA8;
            break;
        default:
            r = 0xA8;
            g = 0x00;
            b = 0x00;
            break;
        case 0:
            uint8_t r = 0x00;
            uint8_t g = 0x00;
            uint8_t b = 0x00;

            ((ArkanoidGame*)this->parent)->GetBackgroudColor(r, g, b);

            this->r = r;
            this->g = g;
            this->b = b;
            break;
        }
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
            if (mode == 0){
                ChangeToBallMode();
            }
            break;
        default:
            break;
    }
}

void Platform::SetModeZero()
{
    this->mode = 0;
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
    // Create panels
    int32_t cnt = 1;
    for (int32_t j = 0; j < 3; j++) {
        for (int32_t i = 0; i < w; i+= 40) {
        
            TargetPanel* p1 = new TargetPanel(this, i, j*10, 40, 10, 0xFF, 0xFF, 0xFF, true, cnt++ /*1*/);
            this->AddChild(p1);
            panels[num_of_panels] = (Panel*)p1;
            num_of_panels++;
            
            if (cnt == 4)
            {
                cnt = 1;
            }
        }
    }

    // 1 panel for win demonstration
    //TargetPanel* p1 = new TargetPanel(this, w/2 + 20, 0, 40, 10, 0xFF, 0xFF, 0xFF, true, 1);
    //this->AddChild(p1);
    //panels[num_of_panels] = (Panel*)p1;
    //num_of_panels++;
    
    // Create moving platform
    platform = new Platform(this, w/2 - PLATFORM_WIDTH/2, h - PLATFORM_HEIGHT - 2, PLATFORM_WIDTH, PLATFORM_HEIGHT, 
    0xFF, 0xFF, 0xFF, true, x, x + w, PLATFORM_SPEED);
    this->AddChild(platform);
    this->GetFocus(platform);
    panels[num_of_panels] = (Panel*)platform;
    num_of_panels++;

    
}

ArkanoidGame::~ArkanoidGame() 
{
    DeleteBall();
    DeletePlatform();

    for (int i = 0; i < numChildren; i++) {
        delete (void*)children[i];
    }
}

void ArkanoidGame::Draw(common::GraphicsContext* gc)
{
    CompositeWidget::Draw(gc);
    
    int32_t X = ball->GetX();
    int32_t Y = ball->GetY();
    int32_t rad = ball->GetRad();

    if (Y >= h) {
        platform->SetModeZero();
        DeleteBall();
    }

    // After hit we can check that all platforms are not physicaly and if this is true we can write congrac
    if(this->CheckWin())
    {
        ShowWinScreen();
    }

    if (X <= 0 || X + rad + BALL_SPEED >= w) 
    {
        ball->ChangeVectorX();
        return;
    }  

    if (Y <= 0) 
    {
        ball->ChangeVectorY();
        return;
    }
    
    for (int i = 0; i < this->num_of_panels; i++) {
        bool IsXHit;
        if(panels[i]->IsHit(X, Y, rad, IsXHit)) {
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


void ArkanoidGame::GetBackgroudColor(uint8_t& r, uint8_t& g, uint8_t& b)
{
    r = this->r;
    g = this->g;
    b = this->b;
}

bool ArkanoidGame::CheckWin()
{
    // last panel is always players movig platform so start check from 1st
    for(int i = 0; i < num_of_panels - 1; i++) {
        if (panels[i]->IsPhysical()) {
            return false;
        }
    }
    return true;
}

void ArkanoidGame::ShowWinScreen()
{
    DeleteBall();
    DeletePlatform();

    ((CompositeWidget*)this->parent)->DeleteChild(this);

    Widget* win = new WinScreen(this->parent, 0, 0, w, h, 0x00, 0xA8, 0x00);
    ((CompositeWidget*)this->parent)->AddChild(win);
    ((CompositeWidget*)this->parent)->GetFocus(win);
}

void ArkanoidGame::DeleteBall()
{
    DeleteChild(ball);
    delete (void*)ball;
    this->ball = 0;
}

void ArkanoidGame::DeletePlatform()
{
    DeleteChild(platform);
    delete (void*)platform;
    this->platform = 0;
}