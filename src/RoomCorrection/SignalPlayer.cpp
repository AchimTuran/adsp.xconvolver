#include <template/include/typedefs.h>
#include "SignalPlayer.h"
#include "utils/stdStringUtils.h"
#include "kodi/AudioEngine/AEChannelInfo.h"

#include <iostream>

using namespace ADDON;
using namespace std;

CSignalPlayer::CSignalPlayer(CSignalRecorder *pSignalRecorder, CGUIDialogXConvolverSettings *pWindow)
{
  m_WaveSignal      = NULL;
  m_pAudioStream    = NULL;
  m_bStop           = true;
  m_pSignalRecorder = pSignalRecorder;
  m_pWindow         = pWindow;
}

CSignalPlayer::~CSignalPlayer()
{
  Destroy();
}

bool CSignalPlayer::Create(uint SampleFrequency)
{
  if(IsRunning())
  {
    StopPlaying();
    while(IsRunning());
  }
  
  if(m_WaveSignal)
  {
    delete m_WaveSignal;
    m_WaveSignal = NULL;
  }
  m_WaveSignal = new CWaveSignal(g_strAddonPath + PATH_SEPARATOR_SYMBOL + string("measurement.signals") + PATH_SEPARATOR_SYMBOL + string("ess_10_20000_fs44100_15s.wav"));

  if(m_pAudioStream)
  {
    AUDIOENGINE->FreeStream(&m_pAudioStream);
  }

  AudioEngineFormat sinkFormat;
  AUDIOENGINE->GetCurrentSinkFormat(sinkFormat);

  CAEChannelInfo AEChannelInfo(sinkFormat.m_channels);
  m_pAudioStream = AUDIOENGINE->MakeStream(AE_FMT_FLOAT, m_WaveSignal->get_SampleFrequency(), m_WaveSignal->get_SampleFrequency(), AEChannelInfo, AESTREAM_AUTOSTART | AESTREAM_BYPASS_ADSP);
  if(!m_pAudioStream)
  {
    KODI->Log(LOG_ERROR, "Couldn't create CAddonAEStream for measurement signals!");
    return false;
  }
 
  if(CThread::IsRunning())
  {
    CThread::StopThread(100);
    m_bStop = true;
    while(!CThread::IsStopped());
  }
  if(!CThread::CreateThread())
  {
    KODI->Log(LOG_ERROR, "Couldn't create playing thread!");
    return false;
  }

  return true;
}

bool CSignalPlayer::Destroy()
{
  // wait 500ms to stop the playing thread
  CThread::StopThread(-1);

  if(m_pAudioStream)
  {
    AUDIOENGINE->FreeStream(&m_pAudioStream);
    m_pAudioStream = NULL;
  }

  if(m_WaveSignal)
  {
    delete m_WaveSignal;
    m_WaveSignal = NULL;
  }

  return true;
}

bool CSignalPlayer::StartPlaying()
{
  if(m_bStop)
  {
    m_bStop = false;
  }
  else
  {
    return false;
  }

  return true;
}

bool CSignalPlayer::StopPlaying()
{
  if(!m_bStop)
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


ulong CSignalPlayer::ProcessSamples(uint8_t *Data, unsigned int Frames)
{
  return (ulong)m_pAudioStream->AddData((uint8_t* const*)&Data, 0, Frames);
}

void *CSignalPlayer::Process(void)
{
  if(!m_pAudioStream)
  {
    KODI->Log(LOG_ERROR, "%s: called with no audio stream!", __func__);
    return NULL;
  }

  // wait until StartPlaying is called
  while(m_bStop);
  KODI->Log(LOG_DEBUG, "Starting speaker measurement process.");

  // add samples to audio engine until StopPlaying is called
  // or the end of the CFloatSignal is reached
  double sampleTimeMs = 1.0 / m_WaveSignal->get_SampleFrequency() * 1000.0;
  unsigned long playPos = 0;
  const unsigned long maxPlayPos = m_WaveSignal->get_BufferedSamples();
  float *pSamples = NULL;
  while(!m_bStop)
  {
    pSamples = NULL;
    ulong MaxSamples = m_WaveSignal->get_Data(playPos, pSamples);
    if(!MaxSamples || !pSamples)
    {
      m_bStop = true;
      break;
    }

    ulong samplesToWrite = 0;
    if(MaxSamples >= 8182)
    {
      samplesToWrite = 8182;
    }
    else
    {
      samplesToWrite = MaxSamples;
    }

    playPos += ProcessSamples((uint8_t*)pSamples, samplesToWrite);
    if(playPos >= m_WaveSignal->get_BufferedSamples() || playPos >= maxPlayPos)
    {
      m_bStop = true;
    }

    if(m_bStop)
    {
      break;
    }

    CThread::Sleep((uint32_t)(m_pAudioStream->GetDelay()*1000.0/2));
  }

  // stop audio stream
  m_pAudioStream->Drain(true);
  while(!m_pAudioStream->IsDrained());

  AUDIOENGINE->FreeStream(&m_pAudioStream);

  // if an capture device is available stop 
  if(m_pSignalRecorder)
  {
    CThread::Sleep(500);
    if(!m_pSignalRecorder->StopRecording())
    {
      KODI->Log(ADDON::LOG_ERROR, "Unable to stop signal recorder!");
    }
  }

  if(m_pWindow)
  {
    if(!m_pWindow->OnClick(BUTTON_STOP_CHIRP_SIGNAL))
    {
      KODI->Log(ADDON::LOG_ERROR, "Unable to stop measurement via button id: %i!", BUTTON_STOP_CHIRP_SIGNAL);
    }
  }

  return NULL;
}
