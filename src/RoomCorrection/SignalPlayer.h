#pragma once



#include <template/include/ADSPAddonMain.h>
#include <kodi/threads/threads.h>
#include "RoomCorrection/SignalGenerator/WaveSignal.h"

class CSignalPlayer : public PLATFORM::CThread
{
public:
  CSignalPlayer();
  ~CSignalPlayer();
  
  bool Create();
  bool Destroy();

  bool StartPlaying();
  bool StopPlaying();
  
private:
  virtual void *Process(void);
  ulong ProcessSamples(uint8_t *Data, unsigned int Frames);

  bool m_bStop;
  CAddonAEStream *m_pAudioStream;
  CWaveSignal *m_WaveSignal;
};