#ifndef __MYOS__COMMON__RANDOM_H
#define __MYOS__COMMON__RANDOM_H

#include <common/types.h>

namespace myos
{
    namespace common
    {
        class RandomGenerator 
        {
        public:
            RandomGenerator(int64_t seed);
            ~RandomGenerator();

            virtual int64_t Generate();

        private:
            int64_t seed;

            int64_t last_value;
        };
    }

}



#endif