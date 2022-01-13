// =============================================================================
//   REQUESTING SPU COMMANDS
// =============================================================================


void play_sound_in_channel( int sound_id, int channel_id )
{
    asm
    {
        "mov R0, {channel_id}"
        "out SPU_SelectedChannel, R0"
        "mov R0, {sound_id}"
        "out SPU_ChannelAssignedSound, R0"
        "out SPU_Command, SPUCommand_PlaySelectedChannel"
    }
}

// ---------------------------------------------------------

void stop_all_channels()
{
    asm
    {
        "out SPU_Command, SPUCommand_StopAllChannels"
    }
}