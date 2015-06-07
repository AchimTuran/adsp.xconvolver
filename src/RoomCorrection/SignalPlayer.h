#pragma once



#include <template/include/ADSPAddonMain.h>
#include <kodi/threads/threads.h>
#include "RoomCorrection/SignalGenerator/WaveSignal.h"
#include "RoomCorrection/CaptureDevice/Source/PortAudioSource.h"
#include <string>

class CSignalPlayer : public PLATFORM::CThread
{
public:
  CSignalPlayer();
  ~CSignalPlayer();
  
  bool Create(uint SampleFrequency, std::string CaptureDevice = "");
  bool Destroy();

  bool StartPlaying();
  bool StopPlaying();

  unsigned int Get_AvailableDevices(CCaptureDeviceList_t &DeviceList);
  unsigned int Get_AvailableDevices(CCaptureDeviceList_t &DeviceList, std::vector<uint> &SampleFrequencies);
  
private:
  virtual void *Process(void);
  ulong ProcessSamples(uint8_t *Data, unsigned int Frames);

  volatile bool m_bStop;
  CAddonAEStream *m_pAudioStream;
  CWaveSignal *m_WaveSignal;
  PortAudioSource *m_CaptureDevice;
  float m_Samples[2048];
};