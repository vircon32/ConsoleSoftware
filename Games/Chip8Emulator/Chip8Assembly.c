// include corresponding header
#include "Chip8.h"


// ---------------------------------------------------------
//   FUNCTIONS WITH ASSEMBLY OPTIMIZATIONS
// ---------------------------------------------------------


void Chip8_DrawScreen_HighResolution()
{
    // main function already covered the screen in
    // black so now draw only pixels set to white
    select_region( RegionSmallPixel );
    
    // this is needed for the assembly block
    // to be able to obtain display RAM address
    bool* DisplayRamPointer = Chip8.DisplayRAM;
    
    // REGISTERS USED:
    // - R1 = pointer to display RAM
    // - R2 = value of current pixel
    // - R3 = drawing point y
    // - R4 = auxiliary register for comparisons
	
    asm
    {
        // save registers
        "push R1"
        "push R2"
        "push R3"
        "push R4"
        
        // initializations
        "mov R1, {DisplayRamPointer}"
        "mov R3, 13"
        "out GPU_DrawingPointY, R3"
        "__drawscreen_hires_start:"
        
        // unrolled loop for 1 horizontal line
        "mov R2, [R1+0]"
        "jf R2, __black_0"
        "out GPU_DrawingPointX, 64"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_0:"

        "mov R2, [R1+1]"
        "jf R2, __black_1"
        "out GPU_DrawingPointX, 68"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_1:"

        "mov R2, [R1+2]"
        "jf R2, __black_2"
        "out GPU_DrawingPointX, 72"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_2:"

        "mov R2, [R1+3]"
        "jf R2, __black_3"
        "out GPU_DrawingPointX, 76"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_3:"

        "mov R2, [R1+4]"
        "jf R2, __black_4"
        "out GPU_DrawingPointX, 80"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_4:"

        "mov R2, [R1+5]"
        "jf R2, __black_5"
        "out GPU_DrawingPointX, 84"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_5:"

        "mov R2, [R1+6]"
        "jf R2, __black_6"
        "out GPU_DrawingPointX, 88"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_6:"

        "mov R2, [R1+7]"
        "jf R2, __black_7"
        "out GPU_DrawingPointX, 92"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_7:"

        "mov R2, [R1+8]"
        "jf R2, __black_8"
        "out GPU_DrawingPointX, 96"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_8:"

        "mov R2, [R1+9]"
        "jf R2, __black_9"
        "out GPU_DrawingPointX, 100"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_9:"

        "mov R2, [R1+10]"
        "jf R2, __black_10"
        "out GPU_DrawingPointX, 104"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_10:"

        "mov R2, [R1+11]"
        "jf R2, __black_11"
        "out GPU_DrawingPointX, 108"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_11:"

        "mov R2, [R1+12]"
        "jf R2, __black_12"
        "out GPU_DrawingPointX, 112"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_12:"

        "mov R2, [R1+13]"
        "jf R2, __black_13"
        "out GPU_DrawingPointX, 116"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_13:"

        "mov R2, [R1+14]"
        "jf R2, __black_14"
        "out GPU_DrawingPointX, 120"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_14:"

        "mov R2, [R1+15]"
        "jf R2, __black_15"
        "out GPU_DrawingPointX, 124"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_15:"

        "mov R2, [R1+16]"
        "jf R2, __black_16"
        "out GPU_DrawingPointX, 128"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_16:"

        "mov R2, [R1+17]"
        "jf R2, __black_17"
        "out GPU_DrawingPointX, 132"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_17:"

        "mov R2, [R1+18]"
        "jf R2, __black_18"
        "out GPU_DrawingPointX, 136"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_18:"

        "mov R2, [R1+19]"
        "jf R2, __black_19"
        "out GPU_DrawingPointX, 140"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_19:"

        "mov R2, [R1+20]"
        "jf R2, __black_20"
        "out GPU_DrawingPointX, 144"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_20:"

        "mov R2, [R1+21]"
        "jf R2, __black_21"
        "out GPU_DrawingPointX, 148"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_21:"

        "mov R2, [R1+22]"
        "jf R2, __black_22"
        "out GPU_DrawingPointX, 152"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_22:"

        "mov R2, [R1+23]"
        "jf R2, __black_23"
        "out GPU_DrawingPointX, 156"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_23:"

        "mov R2, [R1+24]"
        "jf R2, __black_24"
        "out GPU_DrawingPointX, 160"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_24:"

        "mov R2, [R1+25]"
        "jf R2, __black_25"
        "out GPU_DrawingPointX, 164"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_25:"

        "mov R2, [R1+26]"
        "jf R2, __black_26"
        "out GPU_DrawingPointX, 168"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_26:"

        "mov R2, [R1+27]"
        "jf R2, __black_27"
        "out GPU_DrawingPointX, 172"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_27:"

        "mov R2, [R1+28]"
        "jf R2, __black_28"
        "out GPU_DrawingPointX, 176"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_28:"

        "mov R2, [R1+29]"
        "jf R2, __black_29"
        "out GPU_DrawingPointX, 180"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_29:"

        "mov R2, [R1+30]"
        "jf R2, __black_30"
        "out GPU_DrawingPointX, 184"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_30:"

        "mov R2, [R1+31]"
        "jf R2, __black_31"
        "out GPU_DrawingPointX, 188"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_31:"

        "mov R2, [R1+32]"
        "jf R2, __black_32"
        "out GPU_DrawingPointX, 192"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_32:"

        "mov R2, [R1+33]"
        "jf R2, __black_33"
        "out GPU_DrawingPointX, 196"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_33:"

        "mov R2, [R1+34]"
        "jf R2, __black_34"
        "out GPU_DrawingPointX, 200"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_34:"

        "mov R2, [R1+35]"
        "jf R2, __black_35"
        "out GPU_DrawingPointX, 204"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_35:"

        "mov R2, [R1+36]"
        "jf R2, __black_36"
        "out GPU_DrawingPointX, 208"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_36:"

        "mov R2, [R1+37]"
        "jf R2, __black_37"
        "out GPU_DrawingPointX, 212"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_37:"

        "mov R2, [R1+38]"
        "jf R2, __black_38"
        "out GPU_DrawingPointX, 216"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_38:"

        "mov R2, [R1+39]"
        "jf R2, __black_39"
        "out GPU_DrawingPointX, 220"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_39:"

        "mov R2, [R1+40]"
        "jf R2, __black_40"
        "out GPU_DrawingPointX, 224"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_40:"

        "mov R2, [R1+41]"
        "jf R2, __black_41"
        "out GPU_DrawingPointX, 228"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_41:"

        "mov R2, [R1+42]"
        "jf R2, __black_42"
        "out GPU_DrawingPointX, 232"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_42:"

        "mov R2, [R1+43]"
        "jf R2, __black_43"
        "out GPU_DrawingPointX, 236"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_43:"

        "mov R2, [R1+44]"
        "jf R2, __black_44"
        "out GPU_DrawingPointX, 240"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_44:"

        "mov R2, [R1+45]"
        "jf R2, __black_45"
        "out GPU_DrawingPointX, 244"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_45:"

        "mov R2, [R1+46]"
        "jf R2, __black_46"
        "out GPU_DrawingPointX, 248"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_46:"

        "mov R2, [R1+47]"
        "jf R2, __black_47"
        "out GPU_DrawingPointX, 252"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_47:"

        "mov R2, [R1+48]"
        "jf R2, __black_48"
        "out GPU_DrawingPointX, 256"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_48:"

        "mov R2, [R1+49]"
        "jf R2, __black_49"
        "out GPU_DrawingPointX, 260"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_49:"

        "mov R2, [R1+50]"
        "jf R2, __black_50"
        "out GPU_DrawingPointX, 264"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_50:"

        "mov R2, [R1+51]"
        "jf R2, __black_51"
        "out GPU_DrawingPointX, 268"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_51:"

        "mov R2, [R1+52]"
        "jf R2, __black_52"
        "out GPU_DrawingPointX, 272"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_52:"

        "mov R2, [R1+53]"
        "jf R2, __black_53"
        "out GPU_DrawingPointX, 276"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_53:"

        "mov R2, [R1+54]"
        "jf R2, __black_54"
        "out GPU_DrawingPointX, 280"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_54:"

        "mov R2, [R1+55]"
        "jf R2, __black_55"
        "out GPU_DrawingPointX, 284"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_55:"

        "mov R2, [R1+56]"
        "jf R2, __black_56"
        "out GPU_DrawingPointX, 288"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_56:"

        "mov R2, [R1+57]"
        "jf R2, __black_57"
        "out GPU_DrawingPointX, 292"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_57:"

        "mov R2, [R1+58]"
        "jf R2, __black_58"
        "out GPU_DrawingPointX, 296"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_58:"

        "mov R2, [R1+59]"
        "jf R2, __black_59"
        "out GPU_DrawingPointX, 300"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_59:"

        "mov R2, [R1+60]"
        "jf R2, __black_60"
        "out GPU_DrawingPointX, 304"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_60:"

        "mov R2, [R1+61]"
        "jf R2, __black_61"
        "out GPU_DrawingPointX, 308"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_61:"

        "mov R2, [R1+62]"
        "jf R2, __black_62"
        "out GPU_DrawingPointX, 312"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_62:"

        "mov R2, [R1+63]"
        "jf R2, __black_63"
        "out GPU_DrawingPointX, 316"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_63:"

        "mov R2, [R1+64]"
        "jf R2, __black_64"
        "out GPU_DrawingPointX, 320"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_64:"

        "mov R2, [R1+65]"
        "jf R2, __black_65"
        "out GPU_DrawingPointX, 324"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_65:"

        "mov R2, [R1+66]"
        "jf R2, __black_66"
        "out GPU_DrawingPointX, 328"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_66:"

        "mov R2, [R1+67]"
        "jf R2, __black_67"
        "out GPU_DrawingPointX, 332"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_67:"

        "mov R2, [R1+68]"
        "jf R2, __black_68"
        "out GPU_DrawingPointX, 336"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_68:"

        "mov R2, [R1+69]"
        "jf R2, __black_69"
        "out GPU_DrawingPointX, 340"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_69:"

        "mov R2, [R1+70]"
        "jf R2, __black_70"
        "out GPU_DrawingPointX, 344"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_70:"

        "mov R2, [R1+71]"
        "jf R2, __black_71"
        "out GPU_DrawingPointX, 348"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_71:"

        "mov R2, [R1+72]"
        "jf R2, __black_72"
        "out GPU_DrawingPointX, 352"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_72:"

        "mov R2, [R1+73]"
        "jf R2, __black_73"
        "out GPU_DrawingPointX, 356"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_73:"

        "mov R2, [R1+74]"
        "jf R2, __black_74"
        "out GPU_DrawingPointX, 360"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_74:"

        "mov R2, [R1+75]"
        "jf R2, __black_75"
        "out GPU_DrawingPointX, 364"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_75:"

        "mov R2, [R1+76]"
        "jf R2, __black_76"
        "out GPU_DrawingPointX, 368"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_76:"

        "mov R2, [R1+77]"
        "jf R2, __black_77"
        "out GPU_DrawingPointX, 372"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_77:"

        "mov R2, [R1+78]"
        "jf R2, __black_78"
        "out GPU_DrawingPointX, 376"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_78:"

        "mov R2, [R1+79]"
        "jf R2, __black_79"
        "out GPU_DrawingPointX, 380"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_79:"

        "mov R2, [R1+80]"
        "jf R2, __black_80"
        "out GPU_DrawingPointX, 384"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_80:"

        "mov R2, [R1+81]"
        "jf R2, __black_81"
        "out GPU_DrawingPointX, 388"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_81:"

        "mov R2, [R1+82]"
        "jf R2, __black_82"
        "out GPU_DrawingPointX, 392"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_82:"

        "mov R2, [R1+83]"
        "jf R2, __black_83"
        "out GPU_DrawingPointX, 396"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_83:"

        "mov R2, [R1+84]"
        "jf R2, __black_84"
        "out GPU_DrawingPointX, 400"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_84:"

        "mov R2, [R1+85]"
        "jf R2, __black_85"
        "out GPU_DrawingPointX, 404"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_85:"

        "mov R2, [R1+86]"
        "jf R2, __black_86"
        "out GPU_DrawingPointX, 408"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_86:"

        "mov R2, [R1+87]"
        "jf R2, __black_87"
        "out GPU_DrawingPointX, 412"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_87:"

        "mov R2, [R1+88]"
        "jf R2, __black_88"
        "out GPU_DrawingPointX, 416"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_88:"

        "mov R2, [R1+89]"
        "jf R2, __black_89"
        "out GPU_DrawingPointX, 420"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_89:"

        "mov R2, [R1+90]"
        "jf R2, __black_90"
        "out GPU_DrawingPointX, 424"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_90:"

        "mov R2, [R1+91]"
        "jf R2, __black_91"
        "out GPU_DrawingPointX, 428"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_91:"

        "mov R2, [R1+92]"
        "jf R2, __black_92"
        "out GPU_DrawingPointX, 432"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_92:"

        "mov R2, [R1+93]"
        "jf R2, __black_93"
        "out GPU_DrawingPointX, 436"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_93:"

        "mov R2, [R1+94]"
        "jf R2, __black_94"
        "out GPU_DrawingPointX, 440"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_94:"

        "mov R2, [R1+95]"
        "jf R2, __black_95"
        "out GPU_DrawingPointX, 444"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_95:"

        "mov R2, [R1+96]"
        "jf R2, __black_96"
        "out GPU_DrawingPointX, 448"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_96:"

        "mov R2, [R1+97]"
        "jf R2, __black_97"
        "out GPU_DrawingPointX, 452"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_97:"

        "mov R2, [R1+98]"
        "jf R2, __black_98"
        "out GPU_DrawingPointX, 456"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_98:"

        "mov R2, [R1+99]"
        "jf R2, __black_99"
        "out GPU_DrawingPointX, 460"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_99:"

        "mov R2, [R1+100]"
        "jf R2, __black_100"
        "out GPU_DrawingPointX, 464"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_100:"

        "mov R2, [R1+101]"
        "jf R2, __black_101"
        "out GPU_DrawingPointX, 468"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_101:"

        "mov R2, [R1+102]"
        "jf R2, __black_102"
        "out GPU_DrawingPointX, 472"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_102:"

        "mov R2, [R1+103]"
        "jf R2, __black_103"
        "out GPU_DrawingPointX, 476"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_103:"

        "mov R2, [R1+104]"
        "jf R2, __black_104"
        "out GPU_DrawingPointX, 480"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_104:"

        "mov R2, [R1+105]"
        "jf R2, __black_105"
        "out GPU_DrawingPointX, 484"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_105:"

        "mov R2, [R1+106]"
        "jf R2, __black_106"
        "out GPU_DrawingPointX, 488"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_106:"

        "mov R2, [R1+107]"
        "jf R2, __black_107"
        "out GPU_DrawingPointX, 492"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_107:"

        "mov R2, [R1+108]"
        "jf R2, __black_108"
        "out GPU_DrawingPointX, 496"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_108:"

        "mov R2, [R1+109]"
        "jf R2, __black_109"
        "out GPU_DrawingPointX, 500"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_109:"

        "mov R2, [R1+110]"
        "jf R2, __black_110"
        "out GPU_DrawingPointX, 504"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_110:"

        "mov R2, [R1+111]"
        "jf R2, __black_111"
        "out GPU_DrawingPointX, 508"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_111:"

        "mov R2, [R1+112]"
        "jf R2, __black_112"
        "out GPU_DrawingPointX, 512"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_112:"

        "mov R2, [R1+113]"
        "jf R2, __black_113"
        "out GPU_DrawingPointX, 516"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_113:"

        "mov R2, [R1+114]"
        "jf R2, __black_114"
        "out GPU_DrawingPointX, 520"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_114:"

        "mov R2, [R1+115]"
        "jf R2, __black_115"
        "out GPU_DrawingPointX, 524"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_115:"

        "mov R2, [R1+116]"
        "jf R2, __black_116"
        "out GPU_DrawingPointX, 528"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_116:"

        "mov R2, [R1+117]"
        "jf R2, __black_117"
        "out GPU_DrawingPointX, 532"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_117:"

        "mov R2, [R1+118]"
        "jf R2, __black_118"
        "out GPU_DrawingPointX, 536"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_118:"

        "mov R2, [R1+119]"
        "jf R2, __black_119"
        "out GPU_DrawingPointX, 540"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_119:"

        "mov R2, [R1+120]"
        "jf R2, __black_120"
        "out GPU_DrawingPointX, 544"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_120:"

        "mov R2, [R1+121]"
        "jf R2, __black_121"
        "out GPU_DrawingPointX, 548"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_121:"

        "mov R2, [R1+122]"
        "jf R2, __black_122"
        "out GPU_DrawingPointX, 552"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_122:"

        "mov R2, [R1+123]"
        "jf R2, __black_123"
        "out GPU_DrawingPointX, 556"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_123:"

        "mov R2, [R1+124]"
        "jf R2, __black_124"
        "out GPU_DrawingPointX, 560"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_124:"

        "mov R2, [R1+125]"
        "jf R2, __black_125"
        "out GPU_DrawingPointX, 564"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_125:"

        "mov R2, [R1+126]"
        "jf R2, __black_126"
        "out GPU_DrawingPointX, 568"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_126:"

        "mov R2, [R1+127]"
        "jf R2, __black_127"
        "out GPU_DrawingPointX, 572"
        "out GPU_Command, GPUCommand_DrawRegion"
        "__black_127:"
        
        // begin a new line
        "iadd R1, 128"
        "iadd R3, 4"
        "out GPU_DrawingPointY, R3"
        
        // check if this was the last line
        "mov R4, R3"
        "ige R4, 269"
        "jt R4, __drawscreen_hires_end"
        "jmp __drawscreen_hires_start"
        "__drawscreen_hires_end:"
        
        // restore registers
        "pop R4"
        "pop R3"
        "pop R2"
        "pop R1"
    }
}
