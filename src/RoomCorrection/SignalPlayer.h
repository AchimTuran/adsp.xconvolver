#pragma once

#include <template/include/ADSPAddonMain.h>
#include <kodi/threads/threads.h>
#include <asplib/asplib_utils/buffers/TFrameBuffer.h>
#include "RoomCorrection/SignalGenerator/WaveSignal.h"
#include "RoomCorrection/SignalRecorder.h"
#include "Dialogs/GUIDialogXConvolverSettings.h"

#define PROCESSING_FRAME_LENGTH	8192*4

class CGUIDialogXConvolverSettings;
typedef asplib::TFrameBuffer<float> CFloatFrameBuffer;

class CSignalPlayer : public PLATFORM::CThread
{
public:
  CSignalPlayer(CSignalRecorder *pSignalRecorder = NULL, CGUIDialogXConvolverSettings *Window = NULL);
  ~CSignalPlayer();
  
  bool Create(uint SampleFrequency);
  bool Destroy();

  bool StartPlaying();
  bool StopPlaying();
  
private:
  virtual void *Process(void);
  ulong ProcessSamples(uint8_t **Data, unsigned int Frames);

  volatile bool                 m_bStop;
  CAddonAEStream                *m_pAudioStream;
  CWaveSignal                   *m_WaveSignal;
  CSignalRecorder               *m_pSignalRecorder;
  CGUIDialogXConvolverSettings  *m_pWindow;
  CFloatFrameBuffer             *m_AudioChannelBuffer;

  CAEChannelInfo m_AEChannelInfo;
  uint8_t        **m_PlaneAudioArray;
};
