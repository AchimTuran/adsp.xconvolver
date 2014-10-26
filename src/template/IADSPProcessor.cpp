#include "include/IADSPProcessor.h"

IADSPProcessor::IADSPProcessor()
{
}

IADSPProcessor::~IADSPProcessor()
{
}

unsigned int IADSPProcessor::PreProcessNeededSamplesize(unsigned int Mode_id)
{
	return 0;
}

unsigned int IADSPProcessor::InputResampleProcessNeededSamplesize()
{
	return 1024;
}

int IADSPProcessor::InputResampleSampleRate()
{
	return m_StreamSettings.iProcessSamplerate;
}
	
AE_DSP_ERROR IADSPProcessor::MasterProcessSetMode(AE_DSP_STREAMTYPE Type, unsigned int Mode_id, int Unique_db_mode_id)
{
	return AE_DSP_ERROR_NO_ERROR;
}

unsigned int IADSPProcessor::MasterProcessNeededSamplesize()
{
	return 1024;
}

int IADSPProcessor::MasterProcessGetOutChannels(unsigned long &Out_channel_present_flags)
{
	return -1;
}

const char *IADSPProcessor::MasterProcessGetStreamInfoString()
{
	return "";
}

unsigned int IADSPProcessor::PostProcessNeededSamplesize(unsigned int Mode_id)
{
	return 0;
}
	
unsigned int IADSPProcessor::OutputResampleProcessNeededSamplesize()
{
	return 0;
}

int IADSPProcessor::OutputResampleSampleRate()
{
	return m_StreamSettings.iProcessSamplerate;
}

AE_DSP_ERROR IADSPProcessor::StreamIsModeSupported(AE_DSP_MODE_TYPE Type, unsigned int Mode_id, int Unique_db_mode_id)
{
	return AE_DSP_ERROR_NO_ERROR;
}