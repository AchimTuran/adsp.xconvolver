#include <template/include/typedefs.h>
#include "SignalPlayer.h"
#include "utils/stdStringUtils.h"
#include "kodi/AudioEngine/AEChannelInfo.h"

#include <iostream>

using namespace ADDON;
using namespace std;

CSignalPlayer::CSignalPlayer(CSignalRecorder *pSignalRecorder, CGUIDialogXConvolverSettings *pWindow)
{
  m_WaveSignal          = NULL;
  m_pAudioStream        = NULL;
  m_bStop               = true;
  m_pSignalRecorder     = pSignalRecorder;
  m_pWindow             = pWindow;
  m_AudioChannelBuffer  = NULL;

  m_AEChannelInfo.Reset();
}

CSignalPlayer::~CSignalPlayer()
{
  Destroy();

  m_AEChannelInfo.Reset();
}

bool CSignalPlayer::Create(uint SampleFrequency)
{
  Destroy();
  m_WaveSignal = new CWaveSignal(g_strAddonPath + PATH_SEPARATOR_SYMBOL + string("measurement.signals") + PATH_SEPARATOR_SYMBOL + string("ess_10_20000_fs44100_15s.wav"));

  AudioEngineFormat sinkFormat;
  if (!AUDIOENGINE->GetCurrentSinkFormat(sinkFormat))
  {
	  KODI->Log(LOG_ERROR, "Couldn't get sink format! Aborting measurement.");
	  return false;
  }

  m_AEChannelInfo = sinkFormat.m_channels;
  if (m_AEChannelInfo.Count() <= 0)
  {
	  KODI->Log(LOG_ERROR, "No available sink speaker channels! Aborting measurement.");
	  return false;
  }

  m_PlaneAudioArray = new uint8_t*[m_AEChannelInfo.Count()];
  m_AudioChannelBuffer = new CFloatFrameBuffer(PROCESSING_FRAME_LENGTH, m_AEChannelInfo.Count());

  for(uint32_t ch = 0; ch < m_AEChannelInfo.Count(); ch++)
  {
    uint8_t *p = (uint8_t*)m_AudioChannelBuffer->get_Frame(ch);
    if(!p)
    {
      return false;
    }

    m_PlaneAudioArray[ch] = p;
  }

  m_pAudioStream = AUDIOENGINE->MakeStream(AE_FMT_FLOATP, m_WaveSignal->get_SampleFrequency(), m_WaveSignal->get_SampleFrequency(), m_AEChannelInfo, AESTREAM_AUTOSTART | AESTREAM_BYPASS_ADSP);
  if(!m_pAudioStream)
  {
    KODI->Log(LOG_ERROR, "Couldn't create CAddonAEStream for measurement signals! Aborting measurement.");
    return false;
  }

  string channelNames = m_AEChannelInfo;
  KODI->Log(LOG_DEBUG, "Opening CAddonAEStrem with %i speaker channels (%s)", m_AEChannelInfo.Count(), channelNames.c_str());
 
  if(!CThread::CreateThread())
  {
    KODI->Log(LOG_ERROR, "Couldn't create playing thread! Aborting measurement.");
    return false;
  }

  return true;
}

bool CSignalPlayer::Destroy()
{
  if(CThread::IsRunning())
  {
   // imidiatly stop thread
    CThread::StopThread(-1);
  }

  if(m_pAudioStream)
  {
    AUDIOENGINE->FreeStream(&m_pAudioStream);
  }

  if(m_WaveSignal)
  {
    delete m_WaveSignal;
    m_WaveSignal = NULL;
  }

  if(m_PlaneAudioArray)
  {
    delete[] m_PlaneAudioArray;
    m_PlaneAudioArray = NULL;
  }

  if(m_AudioChannelBuffer)
  {
    delete m_AudioChannelBuffer;
    m_AudioChannelBuffer = NULL;
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
    CThread::StopThread(-1);
  }
  else
  {
    return false;
  }

  return true;
}


ulong CSignalPlayer::ProcessSamples(uint8_t **Data, unsigned int Frames)
{
  return (ulong)m_pAudioStream->AddData((uint8_t* const*)Data, 0, Frames);
}

void *CSignalPlayer::Process(void)
{
  if(!m_pAudioStream)
  {
    KODI->Log(LOG_ERROR, "%s: started without a valid audio stream!", __func__);
    return NULL;
  }

  if(!m_pSignalRecorder)
  {
    KODI->Log(LOG_NOTICE, "%s: started without a valid capture device. Measurement will not be saved to a file.", __func__);
  }

  if(!m_pWindow)
  {
    KODI->Log(LOG_NOTICE, "%s: started without a valid window pointer. Measurement status will is not display on the UI.", __func__);
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
  for(uint32_t ch = 0; ch < m_AEChannelInfo.Count() && !m_bStop; ch++)
  {
    m_AudioChannelBuffer->ResetBuffer();
    playPos = 0;

    if(m_pWindow)
    {
      string str = "Measuring ";
      str += CAEChannelInfo::GetChName(m_AEChannelInfo[ch]);
      str += "...";
      m_pWindow->SetMeasurementStatus(str);
    }

    if(m_pSignalRecorder)
    {
      if(!m_pSignalRecorder->StartRecording(CAEChannelInfo::GetChName(m_AEChannelInfo[ch])))
      {
        return NULL;
      }

      // wait until signal recorder is capturing
      while(m_pSignalRecorder->Get_CurrentState() != CSignalRecorder::STATE_CAPTURING);
    }

    // now measure speaker response
    while(playPos < maxPlayPos && !m_bStop)
    {
      pSamples = NULL;
      ulong MaxSamples = m_WaveSignal->get_Data(playPos, pSamples);
      if(!MaxSamples || !pSamples)
      {
        m_bStop = true;
        break;
      }

      ulong samplesToWrite = 0;
      if(MaxSamples >= PROCESSING_FRAME_LENGTH)
      {
        samplesToWrite = PROCESSING_FRAME_LENGTH;
      }
      else
      {
        samplesToWrite = MaxSamples;
      }

      memcpy(m_AudioChannelBuffer->get_Frame(ch), pSamples, sizeof(float)*samplesToWrite);
      ulong samplesWritten = ProcessSamples(m_PlaneAudioArray, samplesToWrite);
      
      if(samplesWritten > 0)
      {
        playPos += samplesWritten;
        if(m_pAudioStream->GetDelay() > 0.0)
        {
          CThread::Sleep((uint32_t)(m_pAudioStream->GetDelay()*1000.0 / 2));
        }
      }
    }

    // wait until all samples are played before the capture process is stoppped
    CThread::Sleep(m_pAudioStream->GetDelay()*1000);
    if(m_pSignalRecorder && !m_pSignalRecorder->StopRecording())
    {
      return NULL;
    }

    // wait 500ms before the next speaker channel will be measured
    CThread::Sleep(500);
  }

  // stop audio stream
  m_pAudioStream->Drain(true);
  while(!m_pAudioStream->IsDrained());

  AUDIOENGINE->FreeStream(&m_pAudioStream);

  // if an capture device is available stop 
  if(m_pSignalRecorder)
  {
    CThread::Sleep(500);
    if(!m_pSignalRecorder->FinishRecording())
    {
      KODI->Log(ADDON::LOG_ERROR, "Unable to stop signal recorder!");
    }
  }

  if(m_pWindow)
  {
    m_pWindow->SetMeasurementStatus(string("Finished measurement"));
  }

  return NULL;
}
