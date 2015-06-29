#include "SignalRecorder.h"

#include <template/include/typedefs.h>
#include "utils/stdStringUtils.h"
#include <sndfile/sndfile.hh>

#include <iostream>

using namespace ADDON;
using namespace std;

CSignalRecorder::CSignalRecorder()
{
  m_bStop = true;

  asplib::CPaHostAPIVector_t usedHostAPIs;
  //usedHostAPIs.push_back(paASIO);
  //usedHostAPIs.push_back(paMME);
  //usedHostAPIs.push_back(paWASAPI);
  //usedHostAPIs.push_back(paDirectSound);
  m_CaptureDevice = new PortAudioSource(usedHostAPIs);
}

CSignalRecorder::~CSignalRecorder()
{
  Destroy();
}

bool CSignalRecorder::Create(uint SampleFrequency, string CaptureDevice)
{
  if(IsRunning())
  {
    StopRecording();
    while (IsRunning());
  }

  if (!KODI->DirectoryExists(generateFilePath(g_strUserPath, "measurements").c_str()))
  {
    KODI->CreateDirectory(generateFilePath(g_strUserPath, "measurements").c_str());
  }

  memset(m_Samples, 0, sizeof(float) * 2048);

  if (!m_CaptureDevice->Create(SampleFrequency, 2048, CaptureDevice))
  {
    KODI->Log(LOG_ERROR, "Couldn't create capture device!");
    return false;
  }

  m_CaptureDevice->get_InputFrameSize();

  if (CThread::IsRunning())
  {
    CThread::StopThread(100);
    m_bStop = true;
    while (!CThread::IsStopped());
  }
  if (!CThread::CreateThread())
  {
    KODI->Log(LOG_ERROR, "Couldn't create playing thread!");
    return false;
  }

  return true;
}

bool CSignalRecorder::Destroy()
{
  // wait 500ms to stop the playing thread
  CThread::StopThread(-1);

  if(m_CaptureDevice)
  {
    delete m_CaptureDevice;
    m_CaptureDevice = NULL;
  }

  return true;
}

bool CSignalRecorder::StartRecording()
{
  if (m_bStop)
  {
    m_bStop = false;
  }
  else
  {
    return false;
  }

  return true;
}

bool CSignalRecorder::StopRecording()
{
  if (!m_bStop)
  {
    m_bStop = true;
    bool ret = CThread::StopThread(100);
  }
  else
  {
    return false;
  }

  return true;
}

unsigned int CSignalRecorder::Get_AvailableDevices(CCaptureDeviceList_t &DeviceList)
{
  vector<uint> sampleFrequencies;
  return Get_AvailableDevices(DeviceList, sampleFrequencies);
}

unsigned int CSignalRecorder::Get_AvailableDevices(CCaptureDeviceList_t &DeviceList, vector<uint> &SampleFrequencies)
{
  DeviceList.clear();
  return m_CaptureDevice->Get_Devices(DeviceList, SampleFrequencies);
}

void *CSignalRecorder::Process(void)
{
  if (!m_CaptureDevice)
  {
    KODI->Log(LOG_ERROR, "%s: called with no audio stream!", __func__);
    return NULL;
  }

  string wavFilename = generateFilePath(generateFilePath(g_strUserPath, "measurements"), "capture.wav");
  SndfileHandle captureWave(wavFilename.c_str(), SFM_WRITE, SF_FORMAT_WAV | SF_FORMAT_FLOAT, 1, m_CaptureDevice->get_InputSampleFrequency());
  if (captureWave.error() != SF_ERR_NO_ERROR)
  {
    KODI->Log(LOG_ERROR, "Couldn't create capture wave file! libsndfile error: %s", sf_error_number(captureWave.error()));
    return NULL;
  }

  // wait until StartRecording is called
  while (m_bStop);
  KODI->Log(LOG_DEBUG, "Starting measurement process.");
  m_CaptureDevice->StartCapturing();

  // record until StopRecording(...) is called
  while (!m_bStop)
  {
    ulong capturedSamples = 0;
    do
    {
      capturedSamples = m_CaptureDevice->GetSamples(m_Samples, 2048);
      if (capturedSamples > 0)
      {
        ulong writtenSamples = (ulong)captureWave.write(m_Samples, capturedSamples);
        if (writtenSamples < capturedSamples)
        {
          m_CaptureDevice->Destroy();
          KODI->Log(LOG_ERROR, "Failed to write to capture wave file!");
          return NULL;
        }
      }
    } while (capturedSamples > 0);

    if (m_bStop)
    {
      break;
    }

    CThread::Sleep((uint32_t)((double)(m_CaptureDevice->get_InputFrameSize()*2*1000.0))/((double)m_CaptureDevice->get_InputSampleFrequency()));
  }

  CThread::Sleep((uint32_t)((double)(m_CaptureDevice->get_InputFrameSize()*2*1000.0))/((double)m_CaptureDevice->get_InputSampleFrequency()));

  m_CaptureDevice->Destroy();

  return NULL;
}
