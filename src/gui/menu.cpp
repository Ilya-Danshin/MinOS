
#include <gui/menu.h>
#include <drivers/vga.h>
#include <memorymanagement.h>
#include <game/arkanoid.h>

using namespace myos;
using namespace myos::common;
using namespace myos::gui;
using namespace myos::game;

#define CURSOR_WIDTH 20
#define MENU_PUNKT_HEIGHT 20

Menu::Menu(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b)
: CompositeWidget(parent, x, y, w, h, r, g, b)
{
    // Save Desktop 
    this->parent = parent;
    
    // Add title
    int32_t X = 0;
    int32_t Y = 0;
    this->ModelToScreen(X, Y);
    title = new Title(this, (X+w)/2, Y, w, MENU_PUNKT_HEIGHT, 0xFF, 0xFF, 0xFF);
    this->AddChild(title);

    // Add Menu punkts
    MenuPunkt* game = new GamePunkt(this, X+CURSOR_WIDTH, Y+MENU_PUNKT_HEIGHT, w, MENU_PUNKT_HEIGHT, 0xFF, 0xFF, 0xFF);
    this->AddChild(game);
    punkts[0] = game;

    MenuPunkt* shutdown = new ShutdownPunkt(this, X+CURSOR_WIDTH, Y+2*MENU_PUNKT_HEIGHT, w, MENU_PUNKT_HEIGHT, 0xFF, 0xFF, 0xFF);
    this->AddChild(shutdown);
    punkts[1] = shutdown;

    num_of_punkts = 2;

    // Add cursor
    cursor = new Cursor(this, X+CURSOR_WIDTH/2, Y+MENU_PUNKT_HEIGHT, CURSOR_WIDTH, MENU_PUNKT_HEIGHT, 0xFF, 0xFF, 0xFF);
    this->AddChild(cursor);
}

Menu::~Menu() 
{
    delete (void*)title;
    delete[] (void*)punkts;
    delete (void*)cursor;
}

void Menu::Draw(common::GraphicsContext* gc){
    //if(is_visible){
        CompositeWidget::Draw(gc);
        gc->DrawRectangle(x, y, w, h, 0xFF, 0xFF, 0xFF);
    //}
}

void Menu::OnKeyDown(char c)
{
    switch (c)
    {
        case '\n':
            punkts[cur_punkt]->Run();
            break;

        
        case 'w':
            if (cur_punkt != 0)
            {
                cur_punkt = cur_punkt - 1;
            }
            else
            {
                cur_punkt = num_of_punkts - 1;
            }
            break;
        case 's':
            cur_punkt = (cur_punkt + 1) % num_of_punkts;
        default:
            break;
    }
    cursor->MoveCursor(cur_punkt);
}

void Menu::SetVisible(bool is_visible)
{
    this->is_visible = is_visible;
}


Title::Title(Widget* parent,
            common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
            common::uint8_t r, common::uint8_t g, common::uint8_t b) 
: Widget(parent, x, y, w, h, r, g, b)
{
}

Title::~Title()
{
}

void Title::Draw(common::GraphicsContext* gc) 
{
    gc->DrawString("Menu", x, y+h/2, r, g, b);
}

MenuPunkt::MenuPunkt(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b)
:CompositeWidget(parent, x, y, w, h, r, g, b)
{
    this->parent = parent;
}

MenuPunkt::~MenuPunkt()
{
}

ShutdownPunkt::ShutdownPunkt(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b)
:MenuPunkt(parent, x, y, w, h, r, g, b)
{
}

ShutdownPunkt::~ShutdownPunkt()
{
}

void ShutdownPunkt::Draw(common::GraphicsContext* gc)
{
    gc->DrawString("Shutdown", x, y, r, g, b);
}

void ShutdownPunkt::Run() 
{
    // TODO: Implementation for shutdown OS
    r = ~r;
    g = ~g;
    b = ~b;
}

GamePunkt::GamePunkt(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b)
:MenuPunkt(parent, x, y, w, h, r, g, b)
{
}

GamePunkt::~GamePunkt()
{
}

void GamePunkt::Draw(common::GraphicsContext* gc)
{
    gc->DrawString("Game", x, y, r, g, b);
}

void GamePunkt::Run()
{
    CompositeWidget* desktop = (CompositeWidget*)this->parent->GetParent();
    
    ArkanoidGame* game = new ArkanoidGame(desktop, 0, 0, 320, 200, 0x00, 0x00, 0x00);
    desktop->AddChild((CompositeWidget*)game);
    desktop->GetFocus((CompositeWidget*)game);

    // Delete menu from Desktop
    desktop->DeleteChild(this->parent);
    delete (void*)this->parent;
    //((Menu*)this->parent)->SetVisible(false);

}

Cursor::Cursor(Widget* parent,
                   common::int32_t x, common::int32_t y, common::int32_t w, common::int32_t h,
                   common::uint8_t r, common::uint8_t g, common::uint8_t b)
:Widget(parent, x, y, w, h, r, g, b)
{
}

Cursor::~Cursor() 
{
}

void Cursor::Draw(common::GraphicsContext* gc) {
    gc->DrawString(">", x, y, r, g, b);
}

void Cursor::MoveCursor(common::int8_t current_punkt)
{
    y += (current_punkt - prev_punkt) * MENU_PUNKT_HEIGHT;
    prev_punkt = current_punkt;
}