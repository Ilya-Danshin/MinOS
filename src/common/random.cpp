#include <common/random.h>



using namespace myos;
using namespace myos::common;

RandomGenerator::RandomGenerator(int64_t seed) 
{
    this->seed = seed;
    this->last_value = seed;

    for(int16_t i = 0; i < 128; i++) {
        Generate();
    }
}

RandomGenerator::~RandomGenerator()
{

}

int64_t RandomGenerator::Generate()
{
    last_value = a + last_value + c;
}