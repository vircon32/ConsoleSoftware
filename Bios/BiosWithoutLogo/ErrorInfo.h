// ---------------------------------------------------------
//     HARDWARE ERROR CODES
// ---------------------------------------------------------


enum error_codes
{
    error_memory_read = 0,
    error_memory_write,
    error_port_read,
    error_port_write,
    error_stack_overflow,
    error_stack_underflow,
    error_division,
    error_arc_cosine,
    error_arc_tangent_2,
    error_logarithm,
    error_power,
    error_unknown
};


// ---------------------------------------------------------
//     MESSAGES FOR EACH ERROR
// ---------------------------------------------------------


struct error_message
{
    int[ 50 ] title;
    int[ 150 ] description;
};

// ---------------------------------------------------------

error_message[ 12 ] error_messages =
{
    {
        "ERROR: INVALID MEMORY READ",
        "Program attempted to read from a memory address\n"
        "that does not exist or is in a write-only device."
    },
    {
        "ERROR: INVALID MEMORY WRITE",
        "Program attempted to write on a memory address\n"
        "that does not exist or is in a read-only device."
    },
    {
        "ERROR: INVALID PORT READ",
        "Program attempted to read from a port number\n"
        "that does not exist or is set as write-only."
    },
    {
        "ERROR: INVALID PORT WRITE",
        "Program attempted to write on a port number\n"
        "that does not exist or is set as read-only."
    },
    {
        "ERROR: STACK OVERFLOW",
        "Program pushed too many values in the stack\n"
        "and available RAM memory was exhausted."
    },
    {
        "ERROR: STACK UNDERFLOW",
        "Program popped too many values from the stack\n"
        "and all data stored in stack was exhausted."
    },
    {
        "ERROR: DIVISION BY ZERO",
        "Program attempted to perform a division or\n"
        "modulus operation where the divisor was zero."
    },
    {
        "ERROR: ARC COSINE OUT OF RANGE",
        "Program attempted to perform an arc cosine\n"
        "operation when the argument was not in [-1,+1]."
    },
    {
        "ERROR: ARC TANGENT NOT DEFINED",
        "Program attempted to perform an arc tangent\n"
        "operation when both of the arguments were 0."
    },
    {
        "ERROR: LOGARITHM OUT OF RANGE",
        "Program attempted to perform a logarithm\n"
        "operation when the argument is not positive."
    },
    {
        "ERROR: POWER HAS NO REAL SOLUTION",
        "Program attempted to perform a power operation\n"
        "when base was negative and exponent non integer."
    },
    {
        "UNKNOWN ERROR",
        "Program caused a hardware error with an error\n"
        "code that was not recognized by the BIOS."
    }
};
