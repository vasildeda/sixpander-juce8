#include "../plugin/include/GainSmoother.h"
#include <iostream>
#include <cassert>

void testGainSmoother()
{
    GainSmoother smoother;
    smoother.setSampleRate(48000.0);
    smoother.setAttackTime(0.01f);
    smoother.setReleaseTime(0.1f);

    smoother.reset(0.0f);

    int s = 0;
    for (; s < 48000; ++s)
    {
        float first = smoother.process(1.0f);
        assert(first > 0.0f && "Expected increase");
        std::cout << s << ": " << first << std::endl;
    }

    smoother.reset(1.0f);
    for (; s < 96000; ++s)
    {
        float second = smoother.process(0.0f);
        assert(second < 1.0f && "Expected decrease");
        std::cout << s << ": " << second << std::endl;
    }


    std::cout << "GainSmoother tests passed.\n";
}

int main()
{
    testGainSmoother();
    return 0;
}