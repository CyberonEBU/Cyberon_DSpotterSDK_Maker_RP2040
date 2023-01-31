#include "Audio.h"

static short g_lpsSampleBuffer[512];
static AudioCallBack g_AudioCallBack;
static bool g_AudioCreate = false;
static void* g_CBPass = 0;//pointer of data struct need by CDSpotterHL

void onPDMdata()
{
  int bytesAvailable = PDM.available();

  if (bytesAvailable > sizeof(g_lpsSampleBuffer))
    bytesAvailable = sizeof(g_lpsSampleBuffer);

  PDM.read(g_lpsSampleBuffer, bytesAvailable);

  g_AudioCallBack(g_CBPass, g_lpsSampleBuffer, bytesAvailable/sizeof(short));
  
}

bool Audio_Start(AudioCallBack onAudioCallBack, void* pVoid)
{
    if(g_AudioCreate)
		return false;
	// PDM start
    // Configure the data receive callback
    PDM.onReceive(onPDMdata);
	
	//
	g_AudioCallBack = onAudioCallBack;
	g_CBPass = pVoid;

    // Optionally set the gain
    // Defaults to 20 on the BLE Sense and 24 on the Portenta Vision Shield
    // PDM.setGain(30);
#if defined(TARGET_ARDUINO_NANO33BLE)
    // The default recording gain of Nano 33 BLE Sense is too slow.
    PDM.setGain(80);
#endif

    // Initialize PDM with:
    // - one channel (mono mode)
    // - a 16 kHz sample rate for the Arduino Nano 33 BLE Sense
    // - a 32 kHz or 64 kHz sample rate for the Arduino Portenta Vision Shield
    if (!PDM.begin(CHANNEL, SAMPLE_RATE))
    {
        return false;
    }
	g_AudioCreate = true;
	
    return true;
}

void Audio_Stop()
{
    PDM.end();
	g_AudioCreate = false;
}