// =============================================================================
//   CARTRIDGE STATE
// =============================================================================


bool cartridge_connected()
{
    asm
    {
        "in R0, CAR_Connected"
    }
}


// =============================================================================
//   PROGRAM FLOW
// =============================================================================


// since there are no exit codes from main, there
// is no need to also provide C's "abort" function
void exit()
{
    asm
    {
        "hlt"
    }
}
