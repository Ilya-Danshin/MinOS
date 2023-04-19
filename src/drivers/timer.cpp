#include <drivers/timer.h>

using namespace myos::common;
using namespace myos::drivers;
using namespace myos::hardwarecommunication;


//void printf(char*);
//void printfHex(uint8_t);

TimerEventHandler::TimerEventHandler() 
{
}

void TimerEventHandler::Tick() {
    //this->tick++;
    tick++;

    //printf("Tick ");
    //printfHex(tick);
    //printf("\n");
}

TimerDriver::TimerDriver(InterruptManager* manager, TimerEventHandler *handler, uint32_t freq)
: InterruptHandler(manager, 0x20), 
dataport(0x40), 
commandport(0x43)
{
    this->freq = freq;
    this->handler = handler;
}

TimerDriver::~TimerDriver()
{
}

void TimerDriver::Activate() {
    /* Get the PIT value: hardware clock at 1193180 Hz */
    uint32_t divisor = 1193180 / this->freq;
    uint8_t low  = (uint8_t)(divisor & 0xFF);
    uint8_t high = (uint8_t)( (divisor >> 8) & 0xFF);

    /* Send the command */
    commandport.Write(0x36);
    dataport.Write(low);
    dataport.Write(high);
}

uint32_t TimerDriver::HandleInterrupt(uint32_t esp)
{
    if(handler == 0)
        return esp;

    handler->Tick();

    return esp;
}
