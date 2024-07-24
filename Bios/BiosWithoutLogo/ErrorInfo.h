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
//     MESSAGES STORED FOR EACH ERROR
// ---------------------------------------------------------


struct error_message
{
    int[ 50 ] title;
    int[ 150 ] description;
};
