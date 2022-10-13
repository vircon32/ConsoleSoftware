// *****************************************************************************
    // include C/C++ headers
    #include <string>           // [ C++ STL ] Strings
    #include <stdexcept>        // [ C++ STL ] Exceptions
    #include <iostream>         // [ C++ STL ] I/O streams
    #include <fstream>          // [ C++ STL ] File streams
    
    // declare used namespaces
    using namespace std;
// *****************************************************************************


// =============================================================================
//      DEFINITIONS
// =============================================================================


// we need precise control over the length
// and character format for these signatures
uint8_t FileSignature[ 8 ] =
{
    'V','3','2','-','M','E','M','C'
};

uint32_t CardSignature[ 20 ] =
{
    'C','H','I','P','-','8',' ','R','O','M',
     0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};


// =============================================================================
//      MAIN FUNCTION
// =============================================================================


int main( int NumberOfArguments, char** Arguments )
{
    try
    {
        // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
        // Process command line arguments
        
        if( NumberOfArguments != 3 )
        {
            cout << "USAGE: chip8tovircon32 <input file> <output file>" << endl;
            return 0;
        }
        
        // capture input parameters
        string InputPath = Arguments[ 1 ];
        string OutputPath = Arguments[ 2 ];
        
        // determine input file size in bytes
        ifstream InputFile;
        InputFile.open( InputPath, ios::binary | ios::ate );
        
        if( !InputFile.good() )
          throw runtime_error( "Cannot open input file" );
        
        uint32_t ROMSize = InputFile.tellg();
        
        // Chip-8 ROMs have a size limit, so enforce it
        uint32_t ROMSizeLimit = 4096 - 512;
        
        if( ROMSize > ROMSizeLimit )
          throw runtime_error( "Source ROM is larger than the Chip-8 size limit" );
        
        // write standard memory card header and the
        // chip-8 rom signature expected by the emulator
        ofstream OutputFile;
        OutputFile.open( OutputPath, ios::binary );
        OutputFile.write( (char*)&FileSignature, 8 );
        OutputFile.write( (char*)&CardSignature, 4*20 );
        
        // write rom size
        OutputFile.write( (char*)&ROMSize, 4 );
        
        // convert each byte to a 4-byte word
        union ByteConverter
        {
            uint32_t Word;
            int Bytes[ 4 ];
        };
        
        ByteConverter Converter;
        Converter.Word = 0;
        InputFile.seekg( 0 );
        
        for( unsigned i = 0; i < ROMSize; i++ )
        {
            InputFile.read( (char*)&Converter.Bytes[ 0 ], 1 );
            OutputFile.write( (char*)&Converter.Word, 4 );
        }
        
        // fill the remainder of the file with zeroes
        // to reach the standard card size of 256 KWords
        Converter.Word = 0;
        int FilledWords = 1 + 20 + ROMSize;
        
        for( unsigned long i = FilledWords; i < (256*1024); i++ )
          OutputFile.write( (char*)&Converter.Word, 4 );
    }
    
    catch( const exception& e )
    {
        cerr << "chip8tovircon32: error: " << e.what() << endl;
        return 1;
    }
    
    // report success
    cout << "conversion successful" << endl;
    return 0;
}
