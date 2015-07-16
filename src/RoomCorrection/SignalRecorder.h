#pragma once

#include <template/include/ADSPAddonMain.h>
#include <kodi/threads/threads.h>
#include "RoomCorrection/CaptureDevice/Source/PortAudioSource.h"
#include <asplib/asplib_utils/buffers/TFrameBuffer.h>
#include <sndfile/sndfile.hh>

#define DEFAULT_FRAME_SIZE  2048
typedef asplib::TFrameBuffer<float> CFloatFrameBuffer;

class CSignalRecorder : public PLATFORM::CThread
{
public:
  typedef enum RecorderState
  {
    STATE_INVALID = -1,

    STATE_IDLE,
    STATE_CAPTURING,
    STATE_NEW_FILE,
    STATE_STOP,
    STATE_FINISH_RECORDING,

    STATE_MAX
  }RecorderState;

  CSignalRecorder(std::string PrefixStr="");
  ~CSignalRecorder();
  bool Create(uint SampleFrequency, uint MaxCaptureChannels, std::string CaptureDevice = "");
  bool Destroy();

  bool StartRecording(std::string PostFixStr="");
  bool StopRecording();
  bool FinishRecording();

  unsigned int Get_AvailableDevices(CCaptureDeviceList_t &DeviceList);
  unsigned int Get_AvailableDevices(CCaptureDeviceList_t &DeviceList, std::vector<uint> &SampleFrequencies);

  RecorderState Get_CurrentState();

private:
  void Set_State(RecorderState State);
  virtual void *Process(void);
  void StoreCapturedSamples(SndfileHandle &CaptureWave);

  PortAudioSource   *m_CaptureDevice;
  CFloatFrameBuffer *m_SamplesBuffer;
  SndfileHandle     *m_CaptureWave;
  uint32_t          m_FrameSize;
  uint32_t          m_MaxCaptureChannels;
  
  RecorderState     m_State;
  std::string       m_PostFixStr;
  std::string       m_PrefixStr;
  PLATFORM::CMutex  m_CriticalState;
};