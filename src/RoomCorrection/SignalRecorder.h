#pragma once

#include <template/include/ADSPAddonMain.h>
#include <kodi/threads/threads.h>
#include "RoomCorrection/CaptureDevice/Source/PortAudioSource.h"

class CSignalRecorder : public PLATFORM::CThread
{
public:
  CSignalRecorder();
  ~CSignalRecorder();
	bool Create(uint SampleFrequency, std::string CaptureDevice = "");
	bool Destroy();

	bool StartRecording();
	bool StopRecording();

  unsigned int Get_AvailableDevices(CCaptureDeviceList_t &DeviceList);
  unsigned int Get_AvailableDevices(CCaptureDeviceList_t &DeviceList, std::vector<uint> &SampleFrequencies);

private:
	virtual void *Process(void);
	ulong ProcessSamples(uint8_t *Data, unsigned int Frames);

	volatile bool m_bStop;
	PortAudioSource *m_CaptureDevice;
	float m_Samples[2048];
};