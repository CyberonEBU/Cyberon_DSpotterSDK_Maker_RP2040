#include "DSpotterSDK_Maker.h"
#include "DSpotterSDK_MakerHL.h"
#include "Audio.h"

#define SERIAL_PRINT

struct PrivateMember
{
    unsigned char m_lpbyDSpotterMem[DSPOTTER_MEM_SIZE];
    DSpotterSDK m_oDSpotterSDK;
    int m_nRecordLostCount;
    int m_nStage;
    EventCallback m_fCallback;
};

/**
   Callback function to process the data from the PDM microphone.
   NOTE: This callback is executed as part of an ISR.
   Therefore using `Serial` to print messages inside this function isn't supported.
**/
void onAudioCallback(void *pVoid, short *pcm, int nPcmSize)
{
    PrivateMember *pStr = (PrivateMember*)pVoid;

    // DSpotter needs mono channel, 16 KHz, 16 bits PCM data as input.
    // H7 using stereo, otherwise audio broken
    #if defined(TARGET_PORTENTA_H7)
    for (int i = 0; i < nPcmSize / 2; i++)
        pcm[i] = pcm[i * 2];
    pStr->m_oDSpotterSDK.PutRecordData(pcm, nPcmSize / 2);
    #else
    // One channel, 16-bit, 2 bytes per sample
    pStr->m_oDSpotterSDK.PutRecordData(pcm, nPcmSize);
    #endif
}

bool DSpotterSDKHL::m_bShowDebugInfo = false;

DSpotterSDKHL::DSpotterSDKHL()
{
    p = new PrivateMember;
    memset(p->m_lpbyDSpotterMem, 0, sizeof(unsigned char) * DSPOTTER_MEM_SIZE);
    p->m_nRecordLostCount = 0;
    p->m_nStage = DSpotterSDK::InitStage;
    p->m_fCallback = NULL;
}

DSpotterSDKHL::~DSpotterSDKHL()
{
    Release();
    delete p;
}

int DSpotterSDKHL::Init(const uint32_t *lpdwLicense, int nLicenseSize, const uint32_t *lpdwModel, EventCallback VRCallback)
{
    int nRet;
    int nMemSize;

    if (p == NULL)
        return LeaveNoMemory;

    if (lpdwLicense == NULL || lpdwModel == NULL) 
        return IllegalParam;

    p->m_fCallback = VRCallback;

#if defined(SERIAL_PRINT)
    // Check VR engine memory usage
    if (m_bShowDebugInfo && Serial)
    {
            Serial.print("DSpotter version: " + String(DSpotterSDK::GetVerInfo()));
    }
#endif

    nMemSize = DSpotterSDK::GetMemoryUsage(lpdwModel, RECORD_BUF_CACHE_TIME);
    if (sizeof(p->m_lpbyDSpotterMem) < nMemSize)
    {
#if defined(SERIAL_PRINT)
        if (m_bShowDebugInfo && Serial)
        {
            Serial.print("The declared memory size for DSpotter is ");
            Serial.println(sizeof(p->m_lpbyDSpotterMem));
            Serial.print("The required memory size for DSpotter is ");
            Serial.println(nMemSize);
            Serial.println("The DSpotter declared memory buffer size is too small!");
        }
#endif
        if (p->m_fCallback!=NULL)
            p->m_fCallback(GetError, LeaveNoMemory, 0, 0, 0);
        return LeaveNoMemory;
    }
    nRet = p->m_oDSpotterSDK.Init(lpdwLicense, nLicenseSize, lpdwModel, RECORD_BUF_CACHE_TIME, p->m_lpbyDSpotterMem, sizeof(p->m_lpbyDSpotterMem));
    switch (nRet)
    {
    case DSpotterSDK::Success:
        // Show VR trigger/command word
        for (int nStage = 0; nStage < 2; nStage++)
        {
            char szCommand[64];
            int nID;
#if defined(SERIAL_PRINT)
            if (m_bShowDebugInfo && Serial)
            {
                if (nStage == 0)
                    Serial.println("The command list at trigger stage:");
                else
                    Serial.println("The command list at command stage:");

                for (int i = 0; i < p->m_oDSpotterSDK.GetCommandCount(nStage); i++)
                {
                    p->m_oDSpotterSDK.GetCommand(nStage, i, szCommand, sizeof(szCommand), &nID);
                    Serial.print("    ");
                    Serial.print(szCommand);
                    Serial.print(", ID = ");
                    Serial.println(nID);
                }
                Serial.println("");
            }
#endif
        }
        break;
    case DSpotterSDK::LicenseFailed:
#if defined(SERIAL_PRINT)
        if (m_bShowDebugInfo && Serial)
        {
#if defined(TARGET_PORTENTA_H7) || defined(TARGET_NANO_RP2040_CONNECT) || defined(TARGET_ARDUINO_NANO33BLE)
            Serial.print("DSpotter license failed! The serial number of your device is ");
            Serial.println(DSpotterSDK::GetSerialNumber());
            Serial.println("Please go to https://tool.cyberon.com.tw/ArduinoDSpotterAuth/FDMain.php to get license data with the serial number.");
#else
            Serial.println("Device not support!");
#endif
        }
#endif
        if (p->m_fCallback != NULL)
            p->m_fCallback(GetError, LicenseFailed, 0, 0, 0);
        return LicenseFailed;
    case DSpotterSDK::DeviceModelNotPair:
#if defined(SERIAL_PRINT)
        if (m_bShowDebugInfo && Serial)
        {
            Serial.println("Authorization check failed.");
            Serial.println("The Voice Recognition model was not created with this device serial number.");
        }
#endif
        if (p->m_fCallback != NULL)
            p->m_fCallback(GetError, LicenseFailed, 0, 0, 0);
        return LicenseFailed;
    case DSpotterSDK::LoadModelFailed:
#if defined(SERIAL_PRINT)
        if (m_bShowDebugInfo && Serial)
        {
            Serial.println("DSpotter load model failed! Please check the model input parameter.");
        }
#endif
        if (p->m_fCallback != NULL)
            p->m_fCallback(GetError, LoadModelFailed, 0, 0, 0);
        return LoadModelFailed;
    default:
#if defined(SERIAL_PRINT)
        if (m_bShowDebugInfo && Serial)
        {
            Serial.print("DSpotter init failed! Error Code = ");
            Serial.println(nRet);
        }
#endif
        if (p->m_fCallback != NULL)
            p->m_fCallback(GetError, nRet, 0, 0, 0);
        return nRet;
    }

    // Turn on automatic gain control [optional]
#if VOLUME_SCALE_RECONG != 100
    p->m_oDSpotterSDK.SetAGC(true, VOLUME_SCALE_RECONG);
#endif

    // Set command stage timeout
    p->m_oDSpotterSDK.SetCommandStageProperty(COMMAND_STAGE_TIMEOUT, COMMAND_STAGE_REPEAT == 1);

    // VR start
    p->m_oDSpotterSDK.Start();

    //Audio init at last avoid lost frame and null pointer
    if(!Audio_Start(onAudioCallback, (void*)p))
    {
#if defined(SERIAL_PRINT)
        if (m_bShowDebugInfo && Serial)
        {
            Serial.println("PDM Audio init failed");
        }
#endif
        /*if (p->m_fCallback != NULL)
            p->m_fCallback(GetError, AudioFailed);*/
        DSpotterSDKHL::Release();
        return AudioFailed;
    }

    if (p->m_fCallback != NULL)
        p->m_fCallback(InitSuccess, nRet, 0, 0, 0);
    return nRet;
}

void DSpotterSDKHL::DoVR()
{
    int nRet, nRetGRR;
    int nCmdScore, nCmdSGDiff, nCmdEnergy, nID;
    int nCurrentStage;
    // Check if lost record frames occurred
    if (p->m_nRecordLostCount != p->m_oDSpotterSDK.GetRecordLostCount())
    {
        p->m_nRecordLostCount = p->m_oDSpotterSDK.GetRecordLostCount();
#if defined(SERIAL_PRINT)
        if (m_bShowDebugInfo && Serial)
        {
            Serial.println("Lost recording frame!");
        }
#endif
        if (p->m_fCallback != NULL)
            p->m_fCallback(LostRecordFrame, p->m_nRecordLostCount, 0, 0, 0);
    }

    // Do VR
    nRet = p->m_oDSpotterSDK.DoRecognition(&nCurrentStage);

    // VR success, get keyword info
    if (nRet == DSpotterSDK::Success)
    {
        char szCommand[64];
        nRetGRR = p->m_oDSpotterSDK.GetRecogResult(&nID, szCommand, sizeof(szCommand), &nCmdScore, &nCmdSGDiff, &nCmdEnergy);

#if defined(SERIAL_PRINT)
        if (m_bShowDebugInfo && Serial)
        {
            if(nRetGRR == DSpotterSDK::TrialLimitStage)//m_oDSpotterSDK.GetRecogResult return DSpotterSDK::TrialLimitStage
            {
                Serial.println("Exceed the limit on recognition in trial version.");
            }
            Serial.print("Get command: ");
            Serial.print(szCommand);
            Serial.print(", Score=");
            Serial.print(nCmdScore);
            Serial.print(", SG_Diff=");
            Serial.print(nCmdSGDiff);
            Serial.print(", Energy=");
            Serial.print(nCmdEnergy);
            Serial.print(", ID=");
            Serial.println(nID);
        }
#endif
        if (p->m_fCallback != NULL)
            p->m_fCallback(GetResult, nID, nCmdScore, nCmdSGDiff, nCmdEnergy);
    }

    // Check VR stage
    if(nRet == DSpotterSDK::TrialLimitStage)
    {
#if defined(SERIAL_PRINT)
        if (m_bShowDebugInfo && Serial)
        {
            Serial.print("Trial version limit ");
            Serial.print(nCurrentStage);
            Serial.println(" seconds left...");
        }
#endif
    }
    else
    {
    	if (p->m_nStage != nCurrentStage)
    	{
    		p->m_nStage = nCurrentStage;
#if defined(SERIAL_PRINT)
            if (m_bShowDebugInfo && Serial)
            {
                if (nCurrentStage == DSpotterSDK::TriggerStage)
                    Serial.println("Switch to trigger stage");
                else
                    Serial.println("Switch to command stage");
            }
#endif
            if (p->m_fCallback != NULL)
                p->m_fCallback(ChangeStage, nCurrentStage, 0, 0, 0);
        }
    }

    // VR error handle
    if (nRet != DSpotterSDK::Success && nRet != DSpotterSDK::NeedMoreSample && nRet != DSpotterSDK::TrialLimitStage)
    {
#if defined(SERIAL_PRINT)
        if (m_bShowDebugInfo && Serial)
        {
            Serial.print("DoRecognition() error, Error Code = ");
            Serial.println(nRet);
        }
#endif
        if (p->m_fCallback != NULL)
            p->m_fCallback(GetError, nRet, 0, 0, 0);
    }
}

void DSpotterSDKHL::Release()
{
    Audio_Stop();
    p->m_oDSpotterSDK.Stop();
    p->m_oDSpotterSDK.Release();
}

const char* DSpotterSDKHL::GetSerialNumber()
{
    return DSpotterSDK::GetSerialNumber();
}

void DSpotterSDKHL::ShowDebugInfo(bool bShowDebugInfo)
{
    // Check if Serial exist
    if (!Serial)
        m_bShowDebugInfo = false;
    else
        m_bShowDebugInfo = bShowDebugInfo;
}
