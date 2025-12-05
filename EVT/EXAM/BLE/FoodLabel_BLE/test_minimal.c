/*
 * Minimal test to verify RISC-V toolchain and CH572 compilation
 */

#include "CH57x_common.h"

int main(void)
{
    // Simple infinite loop - just to test compilation
    while(1)
    {
        __NOP();
    }

    return 0;
}
