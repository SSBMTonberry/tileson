#define POCKETLZMA_LZMA_C_DEFINE
#define NOMINMAX
#include "../../extras/pocketlzma.hpp"

#include "SfmlDemoManager.h"

int main()
{
    SfmlDemoManager demo;
    if(demo.initialize({400 * 3, 240 * 3}, {600, 360}))
    {
        demo.run();
    }
    else
        return EXIT_FAILURE;

    return EXIT_SUCCESS;
}