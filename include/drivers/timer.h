#ifndef __MYOS__DRIVERS__TIMER_H
#define __MYOS__DRIVERS__TIMER_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>

static myos::common::uint64_t tick = 0;

namespace myos 
{
    namespace drivers 
    {
                        
        class TimerEventHandler
        {
        private:
            
        public:
            TimerEventHandler();

            virtual void Tick();
        };
        
        
        class TimerDriver : public myos::hardwarecommunication::InterruptHandler, public Driver
        {
        public:
            TimerDriver(myos::hardwarecommunication::InterruptManager* manager, TimerEventHandler *handler, const myos::common::uint32_t freq);
            ~TimerDriver();

            virtual myos::common::uint32_t HandleInterrupt(myos::common::uint32_t esp);
            virtual void Activate();

        private:
            myos::hardwarecommunication::Port8Bit dataport;
            myos::hardwarecommunication::Port8Bit commandport;

            myos::common::uint32_t freq;

            TimerEventHandler* handler;
        };  
    }
}


#endif
