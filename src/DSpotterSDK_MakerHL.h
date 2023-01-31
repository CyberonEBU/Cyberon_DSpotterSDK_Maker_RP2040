#ifndef __DSPOTTERSDK_MAKER_HL_H__
#define __DSPOTTERSDK_MAKER_HL_H__

#include <stdint.h>
#include <stdbool.h>

#define DSPOTTER_MEM_SIZE 80000 // Please modify this number by the return value of DSpotterSDK::GetMemoryUsage().
#define RECORD_BUF_CACHE_TIME 90 // The cahce length of recording data in ms.
#define VOLUME_SCALE_RECONG 100 // The AGC volume scale percentage for recognition. It depends on the original data volume. 100~1600.
#define COMMAND_STAGE_TIMEOUT 6000 // The maximum recording time in ms when there is no result at command stage.
#define COMMAND_STAGE_REPEAT 1 // When set to 1, the recognizer will repeat recognition in the command stage until it times out.

typedef void (*EventCallback)(int nFlag, int nID, int nScore, int nSG, int nEnergy);

struct PrivateMember;

class DSpotterSDKHL
{
public:
	static const int Success               =     0;
	static const int NotInit               = -2001;
	static const int IllegalParam          = -2002;
	static const int LeaveNoMemory         = -2003;
	static const int AudioFailed           = -2004;
	static const int LoadModelFailed       = -2005;
	static const int Stopped               = -2030;
	static const int LicenseFailed         = -2200;

	static const int InitStage    = -1;
	static const int TriggerStage =  0;
	static const int CommandStage =  1;

	static const int InitSuccess     = 0;
	static const int GetResult       = 1;
	static const int ChangeStage     = 2;
	static const int GetError        = 3;
	static const int LostRecordFrame = 4;

	DSpotterSDKHL();
	virtual ~DSpotterSDKHL();

	/* Get the seurail number of Arduino device.
	 * Return the serial number string. */
	static const char* GetSerialNumber();

	/* Show debug info, should be called after Serial.begin.
	 *   bShowDebugInfo(IN): The flag to decide to show debug infomation or not. Default is false */
	static void ShowDebugInfo(bool bShowDebugInfo = false);

	/* Initialize DSpotter engine and assign the event callback function.
	 *   lpdwLicense(IN): The license data.
	 *   nLicenseSize(IN): The size of the license data.
	 *   lpdwModel(IN): The voice model.
	 *   VRCallback(IN): The callback function to receive results from the voice recognition engine.
	 * Return Success or negative value for error. */
	int Init(const uint32_t *lpdwLicense, int nLicenseSize, const uint32_t *lpdwModel, EventCallback VRCallback);

	/* Do the voice recognition. */
	void DoVR();

	/* Release DSpotter engine. */
	void Release();

private:
	PrivateMember *p;
	static bool m_bShowDebugInfo;
};
#endif
