#include "PDM.h"

#if defined(TARGET_PORTENTA_H7)
#define CHANNEL 2 // Number of record channel. On H7, the record data of mono channel is noisy.
#else
#define CHANNEL 1 // Number of record channel
#endif
#define SAMPLE_RATE 16000 // Record frequency

typedef void (*AudioCallBack)(void* pVoid, short *pPcm, int nPcmSize);

bool Audio_Start(AudioCallBack onPDMdata, void* pVoid);

void Audio_Stop();
