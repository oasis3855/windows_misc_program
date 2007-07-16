// sf2mrtg.h
//
// (C) 2007 INOUE Hirokazu
// GNU General Public License, free software

#include <windows.h>

#pragma pack(push, 1)
typedef struct __speedfan_shared_memory{
	WORD	version;
	WORD	flags;
	int		MemSize;
	HANDLE	handle;
	WORD	numTemps;
	WORD	numFans;
	WORD	numVolts;
	int		tempsArray[32];
	int		fansArray[32];
	int		voltsArray[32];
}_speedfan_shared_memory;
#pragma pack(pop)


struct __sr_prof_data{
	char sSpeedfanPath[1024];
	char sMode[32];
	int nCpuTempColumn;
	int nMbTempColumn;
	char sOutputUptimeStr[256];
	char sOutputTempStr[256];
	char sLogBody[256];
	int nOneLineBytes;
	double nFactor;
} _sr_prof_data;

struct __sr_read_data{
	double nCpuTemp;
	double nMbTemp;
} _sr_read_data;

int GetInitData(__sr_prof_data *srProf);
void WriteInitIniFile(void);
int ReadSpeedfanCsv(__sr_prof_data *srProf, __sr_read_data *srReadData);
int ReadSpeedfanSharedMemory(__sr_prof_data *srProf, __sr_read_data *srReadData);

// EOF