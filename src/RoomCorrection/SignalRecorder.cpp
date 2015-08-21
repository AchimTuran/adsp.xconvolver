#include "SignalRecorder.h"

#include <template/include/typedefs.h>
#include "utils/stdStringUtils.h"
#include <asplib/asplib_utils/signals/interleaver.h>

#include <iostream>

using namespace ADDON;
using namespace std;
using namespace asplib;
using namespace PLATFORM;

CSignalRecorder::CSignalRecorder(string PrefixStr)
{
  m_PrefixStr = PrefixStr;
  asplib::CPaHostAPIVector_t usedHostAPIs;
  //usedHostAPIs.push_back(paASIO);
  //usedHostAPIs.push_back(paMME);
  //usedHostAPIs.push_back(paWASAPI);
  //usedHostAPIs.push_back(paDirectSound);
  m_CaptureDevice = new PortAudioSource(usedHostAPIs);
  m_SamplesBuffer = NULL;
  m_CaptureWave   = NULL;
  m_FrameSize     = DEFAULT_FRAME_SIZE;
  Set_State(STATE_INVALID);
}

CSignalRecorder::~CSignalRecorder()
{
  Destroy();

  if(m_CaptureDevice)
  {
    delete m_CaptureDevice;
    m_CaptureDevice = NULL;
  }
}

bool CSignalRecorder::Create(uint SampleFrequency, uint MaxCaptureChannels, string CaptureDevice)
{
  // if there is already a running thread close it and delete all buffers
  Destroy();

  string capturePath = generateFilePath(g_strUserPath, "measurements");
  if(!KODI->DirectoryExists(capturePath.c_str()))
  {
    KODI->CreateDirectory(capturePath.c_str());
  }

  if(!m_CaptureDevice->Create(SampleFrequency, DEFAULT_FRAME_SIZE, MaxCaptureChannels, CaptureDevice))
  {
    KODI->Log(LOG_ERROR, "Couldn't create capture device!");
    return false;
  }

  m_FrameSize = m_CaptureDevice->get_InputFrameSize();
  m_MaxCaptureChannels = m_CaptureDevice->get_InputChannelAmount();
  if(m_MaxCaptureChannels <= 0 || m_FrameSize <= 0)
  {
    KODI->Log(LOG_ERROR, "Invalid FrameSize or CaptureChannel amount!");
    return false;
  }
  m_SamplesBuffer = new CFloatFrameBuffer(m_FrameSize, m_MaxCaptureChannels);

  Set_State(STATE_INVALID);
  if (!CThread::CreateThread())
  {
    KODI->Log(LOG_ERROR, "Couldn't create playing thread!");
    return false;
  }
  Set_State(STATE_IDLE);

  return true;
}

bool CSignalRecorder::Destroy()
{
  if(CThread::IsRunning())
  {
    // imidiatly stop thread
    CThread::StopThread(-1);
  }

  if(m_SamplesBuffer)
  {
    delete m_SamplesBuffer;
    m_SamplesBuffer = NULL;
  }

  if(m_CaptureWave)
  {
    delete m_CaptureWave;
    m_CaptureWave = NULL;
  }

  return true;
}

bool CSignalRecorder::StartRecording(string PostFixStr)
{
  if(!CThread::IsRunning())
  {
    return false;
  }

  m_PostFixStr = PostFixStr;
  Set_State(STATE_NEW_FILE);

  return true;
}

bool CSignalRecorder::StopRecording()
{
  if(!CThread::IsRunning())
  {
    return false;
  }

  Set_State(STATE_STOP);

  return true;
}

bool CSignalRecorder::FinishRecording()
{
  if(!CThread::IsRunning())
  {
    return false;
  }

  Set_State(STATE_FINISH_RECORDING);

  return true;
}

unsigned int CSignalRecorder::Get_AvailableDevices(CCaptureDeviceList_t &DeviceList)
{
  vector<uint> sampleFrequencies;
  // adding standard sample frequencies
  sampleFrequencies.push_back(8000);
  sampleFrequencies.push_back(11025);
  sampleFrequencies.push_back(16000);
  sampleFrequencies.push_back(22050);
  sampleFrequencies.push_back(32000);
  sampleFrequencies.push_back(44100);
  sampleFrequencies.push_back(48000);
  sampleFrequencies.push_back(64000);
  sampleFrequencies.push_back(88200);
  sampleFrequencies.push_back(96000);
  sampleFrequencies.push_back(176400);
  sampleFrequencies.push_back(192000);
  sampleFrequencies.push_back(384000);

  return Get_AvailableDevices(DeviceList, sampleFrequencies);
}

unsigned int CSignalRecorder::Get_AvailableDevices(CCaptureDeviceList_t &DeviceList, vector<uint> &SampleFrequencies)
{
  DeviceList.clear();
  return m_CaptureDevice->Get_Devices(DeviceList, SampleFrequencies);
}

CSignalRecorder::RecorderState CSignalRecorder::Get_CurrentState()
{
  CLockObject lock(m_CriticalState);
  
  return m_State;
}

void CSignalRecorder::Set_State(RecorderState State)
{
  CLockObject lock(m_CriticalState);

  m_State = State;
}

void *CSignalRecorder::Process(void)
{
  try
  {
    if(!m_CaptureDevice)
    {
      KODI->Log(LOG_ERROR, "%s: called with no audio stream!", __func__);
      return NULL;
    }
    string waveFilename;
    string wavFilePath;
    RecorderState recState = STATE_INVALID;

    while(Get_CurrentState() == STATE_INVALID);
    recState = Get_CurrentState();

    KODI->Log(LOG_DEBUG, "Starting signal recorder.");

    // record until StopRecording(...) is called
    while(recState > STATE_INVALID && recState < STATE_MAX && recState != STATE_FINISH_RECORDING)
    {
      switch(recState)
      {
        case STATE_IDLE:
        break;

        case STATE_CAPTURING:
          StoreCapturedSamples(*m_CaptureWave);
        break;

        case STATE_NEW_FILE:
          if(m_CaptureWave)
          {
            if(m_CaptureDevice->GetStoredSamples() > 0)
            {
              StoreCapturedSamples(*m_CaptureWave);
            }

            delete m_CaptureWave;
            m_CaptureWave = NULL;
          }

          waveFilename = m_PrefixStr + m_PostFixStr + string(".wav");
          wavFilePath = generateFilePath(generateFilePath(g_strUserPath, "measurements"), waveFilename);
          m_CaptureWave = new SndfileHandle(wavFilePath.c_str(), SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_FLOAT, m_MaxCaptureChannels, (int)m_CaptureDevice->get_InputSampleFrequency());
          if(m_CaptureWave->error() != SF_ERR_NO_ERROR)
          {
            KODI->Log(LOG_ERROR, "Couldn't create capture wave file! libsndfile error: %s", sf_error_number(m_CaptureWave->error()));
            return NULL;
          }

          if(!m_CaptureDevice->StartCapturing())
          {
            KODI->Log(LOG_ERROR, "Failed to start capturing device! Aborting...");
            Set_State(STATE_INVALID);
          }

          Set_State(STATE_CAPTURING);
        break;

        case STATE_STOP:
          // save last captured samples
          if(m_CaptureDevice->GetStoredSamples() > 0)
          {
            StoreCapturedSamples(*m_CaptureWave);
          }

          if(!m_CaptureDevice->StopCapturing())
          {
            KODI->Log(LOG_ERROR, "Failed to stop capturing device! Aborting...");
            Set_State(STATE_INVALID);
          }

          Set_State(STATE_IDLE);
        break;

        default:
        {
          KODI->Log(LOG_ERROR, "Invalid signal recorder state! Aborting measurement!");
          Set_State(STATE_INVALID);
        }
        break;
      }

      // Update current signal recorder state
      recState = Get_CurrentState();

      if(recState == STATE_CAPTURING)
      {
        CThread::Sleep((uint32_t)((double)(m_CaptureDevice->get_InputFrameSize()*2.0*1000.0))/((double)m_CaptureDevice->get_InputSampleFrequency()));
      }
    }

    m_CaptureDevice->StopCapturing();
    if(m_CaptureWave && m_CaptureDevice->GetStoredSamples() > 0)
    {
      StoreCapturedSamples(*m_CaptureWave);
    }
    m_CaptureDevice->Destroy();

    Set_State(STATE_INVALID);

    return NULL;
  }
  catch(bad_alloc &e)
  {
    KODI->Log(LOG_ERROR, "%s: catched exception: %s", __func__, e.what());
  }
  catch(...)
  {
    KODI->Log(LOG_ERROR, "%s: catched unhandled exception: %s", __func__);
  }

  m_CaptureDevice->Destroy();
  return NULL;
}

void CSignalRecorder::StoreCapturedSamples(SndfileHandle &CaptureWave)
{
  ulong capturedSamples = 0;
  do
  {
    capturedSamples = m_CaptureDevice->GetSamples(m_SamplesBuffer->get_Frame(0), m_FrameSize*m_MaxCaptureChannels);
    capturedSamples /= m_MaxCaptureChannels;
    if(capturedSamples > 0)
    {
      ulong writtenSamples = 0;
      if(m_CaptureDevice->get_InputChannelAmount() > 1)
      {
        writtenSamples = (ulong)CaptureWave.writef(m_SamplesBuffer->get_Frame(0), capturedSamples);
      }
      else
      {
        writtenSamples = (ulong)CaptureWave.write(m_SamplesBuffer->get_Frame(0), capturedSamples);
      }

      if(writtenSamples < capturedSamples)
      {
        KODI->Log(LOG_ERROR, "Failed to write to capture wave file!");
        Set_State(STATE_INVALID);
      }
    }
  }while(capturedSamples > 0);
}
