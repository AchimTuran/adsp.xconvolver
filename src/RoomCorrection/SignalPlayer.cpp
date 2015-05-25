#include <template/include/typedefs.h>
#include "SignalPlayer.h"

using namespace ADDON;
using namespace std;

CSignalPlayer::CSignalPlayer()
{
  m_WaveSignal = NULL;
  m_pAudioStream = NULL;
  m_bStop = true;
}

CSignalPlayer::~CSignalPlayer()
{
  Destroy();
}

bool CSignalPlayer::Create()
{
  m_WaveSignal = m_WaveSignal = new CWaveSignal(g_strAddonPath + PATH_SEPARATOR_SYMBOL + string("measurement.signals") + PATH_SEPARATOR_SYMBOL + string("ess_10_20000_fs44100_15s.wav"));
  m_pAudioStream = AUDIOENGINE->AudioEngine_MakeStream(AUDIOENGINE_FMT_FLOAT, m_WaveSignal->get_SampleFrequency(), m_WaveSignal->get_SampleFrequency(), AUDIOENGINE_CH_LAYOUT_1_0, AUDIOENGINESTREAM_AUTOSTART);
  if(!m_pAudioStream)
  {
    KODI->Log(LOG_ERROR, "Couldn't create CAddonAEStream for measurement signals!");
    return false;
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
    AUDIOENGINE->AudioEngine_FreeStream(&m_pAudioStream);
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
  }
  else
  {
    return false;
  }

  return true;
}


ulong CSignalPlayer::ProcessSamples(uint8_t *Data, unsigned int Frames)
{
  return (ulong)m_pAudioStream->AddData((uint8_t* const*)&Data, 0, Frames*sizeof(float))/sizeof(float);
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

  // add samples to audio engine until StopPlaying is called
  // or the end of the CFloatSignal is reached
  double sampleTimeMs = 1.0 / m_WaveSignal->get_SampleFrequency() * 1000.0;
  unsigned int playPos = 0;
  uint8_t *pSamples = NULL;
  while(!m_bStop)
  {
    ulong MaxSamples = m_WaveSignal->get_Data(playPos, (float*&)pSamples);
    if(!MaxSamples || !pSamples)
    {
      // TODO: error log message
      m_bStop = true;
      break;
    }

    playPos += ProcessSamples(pSamples, MaxSamples);

    if(playPos >= m_WaveSignal->get_BufferedSamples())
    {
      m_bStop = true;
      break;
    }

    //CThread::Sleep((uint32_t));
  }

  AUDIOENGINE->AudioEngine_FreeStream(&m_pAudioStream);

  return NULL;
}