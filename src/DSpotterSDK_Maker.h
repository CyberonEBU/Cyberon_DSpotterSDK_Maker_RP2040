#ifndef __DSPOTTERSDK_MAKER_H
#define __DSPOTTERSDK_MAKER_H

#include <stdint.h>
#include <stdbool.h>

#if defined(TARGET_ARDUINO_NANO33BLE) || defined(TARGET_PORTENTA_H7)
// For ARDUINO_NANO33BLE and PORTENTA_H7
#include <pins_arduino.h>
#elif defined(TARGET_NANO_RP2040_CONNECT)
// For NANO_RP2040_CONNECT
#include <ArduinoECCX08.h>
#include <WiFiNINA.h>
#endif

class DSpotterSDK
{
public:
	static const int Success               =     0;
	static const int NotInit               = -2001;
	static const int IllegalParam          = -2002;
	static const int LeaveNoMemory         = -2003;
	static const int LoadModelFailed       = -2005;
	static const int NeedMoreSample        = -2009;
	static const int TrialLimitStage       = -2010;
	static const int Stopped               = -2030;
	static const int LicenseFailed         = -2200;
	static const int DeviceModelNotPair    = -2201;

	static const int InitStage    = -1;
	static const int TriggerStage =  0;
	static const int CommandStage =  1;

	DSpotterSDK();
	virtual ~DSpotterSDK();

	/* Get the version information of DSpotter.
	 * Return the version string. */
	static const char* GetVerInfo();

	/* Get the seurail number of Arduino device.
	 * Return the serial number string. */
	static const char* GetSerialNumber();

	/* Get the memory usage of DSpotter.
	 *   lpdwModel(in): The voice model.
	 *   nRecordCacheTimeMS(in): The cache length of record data, unit is millisecond.
	 *                           It must be greater than or equal to 60.
	 * Return the memory usage. */
	static int GetMemoryUsage(const uint32_t *lpdwModel, int nRecordCacheTimeMS);

	/* Initialize DSpotterSDK Maker.
	 *   lpdwLicense(in): The license data.
	 *   nLicenseSize(in): The size of the license data.
	 *   lpdwModel(in): The voice model.
	 *   nRecordCacheTimeMS(in): The cache length of record data, unit is millisecond.
	 *                           It must be greater than or equal to 60.
	 *   lpbyMemPool(in): The memory buffer that will be used by DSpotter.
	 *   nMemSize(in):
	 * Return Success: Initial OK.
	 *        IllegalParam: If pointer parameter is null or nRecordCacheTimeMS < 60.  
	 *        LeaveNoMemory: If nMemSize less than the return value of GetMemoryUsage().
	 *        LoadModelFailed: If lpdwModel is not a valid voice model.
	 *        LicenseFailed: If lpdwLicense doesn't match the device ID. */
	int Init(const uint32_t *lpdwLicense, int nLicenseSize, const uint32_t *lpdwModel, int nRecordCacheTimeMS, unsigned char *lpbyMemPool, int nMemSize);
	
	/* To release DSpotter, then the memory buffer(lpbyMemPool) can be reused by others.
	 * Return Success or negative value for error. */
	int Release();

	/* Set the option of auto gain control(AGC).
	 *   bEnableAGC(in): To enable or disable AGC.
	 *   nScalePercentage(in): The scale percentage of gain is 100 ~ 1600. The default value is 100.
	 * Return Success or negative value for error. */
	int SetAGC(bool bEnableAGC = false, int nScalePercentage = 100);

	/* Set the flow property at command stage.
	 *   nTimeout(in): The maximum recording time in ms when there is no result at command stage.
	 *                 The valid range is 1000 to 30000. The default value is 6000.
	 *   bCommandStageRepeatUntilTimeout(in): 
	 *                 If false, the recognition flow will switch to trigger stage immediately after
	 *                 command recognized. If true, it will recognize repeatedly at command stage until
	 *                 timeout. The default value is false.
	 * Return Success or negative value for error. */
	int SetCommandStageProperty(int nTimeout = 6000, bool bCommandStageRepeatUntilTimeout = false);

	/* Start the process of recognition. Please call Start() before starting the recording device.
	 * Return Success or negative value for error. */
	int Start();

	/* Stop the process of recognition. PutRecordData() and DoRecognition() will do nothing 
	 * after calling Stop().
	 * Return Success or negative value for error. */
	int Stop();

	/* Put the record data to the cached record buffer.
	 *   lpsSample(in): The record data buffer.
	 *   nNumSample(in): The number of samples in the record data buffer.
	 * Return Success: Put OK.
	 *        NotInit: If call it before Init().
	 *        Stopped: If call it before Start().
	 *        LeaveNoMemory: If the cached record buffer has no enough free space. The lost count will
	 *                       increment by 1. If it only appears temporarily, please enlarge the cache 
	 *                       length of record data. If it keeps appearing, it is usually because the MCU
	 *                       computing resources are exhausted. */
	int PutRecordData(const short *lpsSample, int nNumSample);

	/* Get the lost count when putting record data.
	 * Return the lost count */
	int GetRecordLostCount();

	/* Get 10 millisecond data from cached record buffer and process it. 
	 *   pnCurrentState(out): The current stage.
	 * Return Success: Get the recognition result.
	 *        NotInit: If call it before Init().
	 *        Stopped: If call it before Start().
	 *        NeedMoreSample: No result and need more sample to process. */
	int DoRecognition(int *pnCurrentState);
	
	/* Get the information of recognition result.
	 *   pnID(out): The ID of command.
	 *   lpszCommand(out): The command buffer.
	 *   nCmdLength(in): The length of command buffer.
	 *   pnConfi(out): The confidence score of command.
	 *   pnSGDiff(out): The human voice similarity of command.
	 *   pnCmdEnergy(out): The energy of command.
	 * Return Success or negative value for error. */
	int GetRecogResult(int *pnID, char *lpszCommand, int nCmdLength, int *pnConfi, int *pnSGDiff, int *pnCmdEnergy);

	/* Get the count of recognition command at different stage.
	 *   nStage(in): TriggerStage or CommandStage.
	 * Return the command count */
	int GetCommandCount(int nStage);

	/* Get the information of command.
	 *   nStage(in): TriggerStage or CommandStage.
	 *   nIndex(in): From 0 to GetCommandCount(nStage) - 1.
	 *   lpszCommand(out): The command buffer.
	 *   nCmdLength(in): The length of command buffer.
	 *   pnID(out): The ID of command.
	 * Return Success or negative value for error. */
	int GetCommand(int nStage, int nIndex, char *lpszCommand, int nCmdLength, int *pnID);

protected:
	void *m_hCybModel;
	void *m_hDSpotter;
	void *m_hRingBuffer;
	unsigned char *m_lpbyMemPool;
	int m_nCmdIndex;
	int m_nActiveGroupIndex;
	int m_nRecogGroupIndex;
	int m_nCommandStageTimeout;
	int m_nCommandStageLimit;
	bool m_bCommandStageRepeatUntilTimeout;
	int m_nCommandRecordSample;
	int m_nConfi;
	int m_nSGDiff;
	int m_nCmdEnergy;
	bool m_bEnableAGC;
	int m_nScalePercentage;
	int m_nRecordLostCount;
	bool m_bStart;
	int m_nModelType;
	int m_nTVD;

	int InitRecognition(int nGroupIndex);
	void ReleaseRecognition();
};


#endif