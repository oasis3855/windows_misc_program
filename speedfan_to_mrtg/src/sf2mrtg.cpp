// sf2mrtg.cpp
//
// (C) 2007 INOUE Hirokazu
// GNU General Public License, free software

#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <windows.h>
#include "sf2mrtg.h"

int _tmain(int argc, _TCHAR* argv[])
{
	__sr_prof_data srProf;
	__sr_read_data srReadData;
	int nCpuTemp, nMbTemp;
	DWORD nUptime;
	char sUptime[125];

	// -h/-? オプションが付けられたときには、説明文を画面に表示
	if(argc == 2)
	{
		if(!strcmp(argv[1], "/?") || !strcmp(argv[1], "/h") || !strcmp(argv[1], "-h") || !strcmp(argv[1], "-?"))
			puts("sf2mrtg\n\tMRTG external script program for SpeedFan Shared Memory/CSV datafile\n\n"
			"Usage\n\tsf2mrtg [-h?]\n"
			"Options\n\t-h/-?\tdisplay this help\n"
			"Configuration File\n"
			"\tsf2mrtg.ini, in the same directory of sf2mrtg.exe\n"
			"\tConfiguration file is automatically made,\n\tif it doesn't exists at sf2mrtg.exe folder.\n\n"
			"\tIf CSV mode, edit SpeedFanPath key (end with \\).\n"
			"Return Value\n"
			"\tif success, return 0. otherwise return 1\n"
			"Copyright\n"
			"\t(C) 2007 INOUE. Hirokazu\n"
			"\tThis program is free software: you can use this under the GNU\n"
			"\tGeneral Public License, for details see sf2mrtg.txt\n");

		return 1;
	}

	// INIファイルから設定値を読み込む
	// （INIファイルが存在しないときには、INIファイルを作成する）
	if(GetInitData(&srProf) != 0)
	{
		puts("0\n0\n0\nError (INI file error)\n");
		return 1;	// 読み込みエラー （もしくはINIファイル新規作成）
	}

	if(!strcmp(srProf.sMode, "csv"))
	{
		// SpeedFan の CSV からデータを読み込む
		if(ReadSpeedfanCsv(&srProf, &srReadData) != 0)
		{
			puts("0\n0\n0\nError (SpeedFan CSV read error)\n");
			return 1;	// 読み込みエラー
		}
	}
	else if(!strcmp(srProf.sMode, "sharemem"))
	{
		// SpeedFan の 共有メモリー からデータを読み込む
		if(ReadSpeedfanSharedMemory(&srProf, &srReadData) != 0)
		{
			puts("0\n0\n0\nError (SpeedFan Shared Memory read error)\n");
			return 1;	// 読み込みエラー
		}
	}
	else
	{
		puts("0\n0\n0\nError (mode must be sharemem/csv)\n");
		return 1;	// 読み込みエラー
	}

	// 温度にファクターを掛ける
	nCpuTemp = (int)(srReadData.nCpuTemp * srProf.nFactor);
	nMbTemp = (int)(srReadData.nMbTemp * srProf.nFactor);

	// Windows 起動からの時間を得る
	nUptime = GetTickCount();
	sprintf(sUptime, srProf.sOutputUptimeStr, nUptime/(24*3600000L), (nUptime/3600000L)%24, (nUptime/60000)%60);

	// 画面表示　（mrtgのデータ表示方式にのっとり、4行表示）
	printf("%d\n%d\n%s\n%s\n", nMbTemp, nCpuTemp, sUptime, srProf.sOutputTempStr);

	return 0;		// 正常終了
}


// INI ファイルから設定値を読み込む
int GetInitData(__sr_prof_data *srProf)
{
	char sModulePath[1024];
	char *sPtr;
	char sTemp[1024];
	int i;
	size_t nStrSize;

	// 現在のモジュールのフルパス名を得る
	i = GetModuleFileName(NULL, sTemp, 1023);
	if(i<=0 || i>=1023) return 1;		// 現在のプロセスのパス名が得られなかった


	// モジュールパスの最後の . の位置までをファイル名のボディとする
	sPtr = strrchr(sTemp, '.');
	if(sPtr == NULL)
		nStrSize = strlen(sTemp);		// . が無い場合、パス名全体とする
	else
		nStrSize = strlen(sTemp) - strlen(sPtr);
	strncpy(sModulePath, sTemp, nStrSize);
	sModulePath[nStrSize] = (char)NULL;

	// INI 拡張子をつける
	strcat(sModulePath, ".ini");


	// INIファイルが存在するかの確認
	GetPrivateProfileString("sf2mrtg", "install", "err", sTemp, 1023, sModulePath);
	if(!strcmp(sTemp, "err"))
	{
		WriteInitIniFile();		// INI が確認できない場合、新規作成
		return 1;
	}


	// 設定値読み込み

	GetPrivateProfileString("sf2mrtg", "Mode", "err", srProf->sMode, 31, sModulePath);
	if(!strcmp(srProf->sSpeedfanPath, "err")) return 1;			// 設定値読み込み不能

	GetPrivateProfileString("sf2mrtg", "SpeedFanPath", "err", srProf->sSpeedfanPath, 1023, sModulePath);
	if(!strcmp(srProf->sSpeedfanPath, "err")) return 1;			// 設定値読み込み不能

	GetPrivateProfileString("sf2mrtg", "CpuTempColumn", "err", sTemp, 1023, sModulePath);
	if(!strcmp(sTemp, "err")) return 1;							// 設定値読み込み不能
	srProf->nCpuTempColumn = atoi(sTemp);

	GetPrivateProfileString("sf2mrtg", "MbTempColumn", "err", sTemp, 1023, sModulePath);
	if(!strcmp(sTemp, "err")) return 1;							// 設定値読み込み不能
	srProf->nMbTempColumn = atoi(sTemp);

	GetPrivateProfileString("sf2mrtg", "OutputUptimeStr", "err", srProf->sOutputUptimeStr, 254, sModulePath);
	if(!strcmp(srProf->sOutputUptimeStr, "err")) return 1;		// 設定値読み込み不能

	GetPrivateProfileString("sf2mrtg", "OutputTempStr", "err", srProf->sOutputTempStr, 254, sModulePath);
	if(!strcmp(srProf->sOutputTempStr, "err")) return 1;		// 設定値読み込み不能

	GetPrivateProfileString("sf2mrtg", "SpeedFanLogBody", "err", srProf->sLogBody, 254, sModulePath);
	if(!strcmp(srProf->sLogBody, "err")) return 1;		// 設定値読み込み不能

	GetPrivateProfileString("sf2mrtg", "CsvOneLineBytes", "err", sTemp, 1023, sModulePath);
	if(!strcmp(sTemp, "err")) return 1;							// 設定値読み込み不能
	srProf->nOneLineBytes = atoi(sTemp);

	GetPrivateProfileString("sf2mrtg", "Factor", "err", sTemp, 1023, sModulePath);
	if(!strcmp(sTemp, "err")) return 1;							// 設定値読み込み不能
	srProf->nFactor = atof(sTemp);


	return 0;
}


// 新規のINIファイルを作成する
void WriteInitIniFile(void)
{
	char sModulePath[1024];
	char *sPtr;
	char sTemp[1024], sTemp2[1280];
	int i;
	size_t nStrSize;

	// 現在のモジュールのフルパス名を得る
	i = GetModuleFileName(NULL, sTemp, 1023);
	if(i<=0 || i>=1023) return ;		// 現在のプロセスのパス名が得られなかった


	// モジュールパスの最後の . の位置までをファイル名のボディとする
	sPtr = strrchr(sTemp, '.');
	if(sPtr == NULL)
		nStrSize = strlen(sTemp);		// . が無い場合、パス名全体とする
	else
		nStrSize = strlen(sTemp) - strlen(sPtr);
	strncpy(sModulePath, sTemp, nStrSize);
	sModulePath[nStrSize] = (char)NULL;

	// INI 拡張子をつける
	strcat(sModulePath, ".ini");

	// コメント文字列を作成 （mrtg.cfgへの設定方法を示す文字列）
	GetModuleFileName(NULL, sTemp, 1023);
	sprintf(sTemp2, "set mrtg.cfg target section as \"Target[OutFile]: `%s`\"", sTemp);

	// サンプルのINIファイルを書き込む

	if(!WritePrivateProfileString("sf2mrtg", "comment1", sTemp2, sModulePath))
		return ;
	if(!WritePrivateProfileString("sf2mrtg", "install", "installed (this is for detection of ini file)", sModulePath))
		return ;
	if(!WritePrivateProfileString("sf2mrtg", "comment2", "Mode must be sharemem|csv", sModulePath))
		return ;
	if(!WritePrivateProfileString("sf2mrtg", "Mode", "sharemem", sModulePath))
		return ;
	if(!WritePrivateProfileString("sf2mrtg", "SpeedFanPath", "c:\\Program Files\\SpeedFan\\", sModulePath))
		return ;
	if(!WritePrivateProfileString("sf2mrtg", "SpeedFanLogBody", "SFLog", sModulePath))
		return ;
	if(!WritePrivateProfileString("sf2mrtg", "CpuTempColumn", "2", sModulePath))
		return ;
	if(!WritePrivateProfileString("sf2mrtg", "MbTempColumn", "1", sModulePath))
		return ;
	if(!WritePrivateProfileString("sf2mrtg", "OutputUptimeStr", "uptime %d days %02d hours %02d minutes", sModulePath))
		return ;
	if(!WritePrivateProfileString("sf2mrtg", "OutputTempStr", "Temperature", sModulePath))
		return ;
	if(!WritePrivateProfileString("sf2mrtg", "comment3", "CsvOneLineSize don't needs 'just' bytes, it is used for skipping lines close to last line of csv file", sModulePath))
		return ;
	if(!WritePrivateProfileString("sf2mrtg", "CsvOneLineBytes", "17", sModulePath))
		return ;
	if(!WritePrivateProfileString("sf2mrtg", "Factor", "1", sModulePath))
		return ;


	return ;
}

// SpeedFan の CSV ファイルより、温度データを読み込む
int ReadSpeedfanCsv(__sr_prof_data *srProf, __sr_read_data *srReadData)
{
	time_t tmNowSec;
	struct tm *tmNow;
	char sTemp[1024], sTemp2[1024];
	char sCsvPath[1024];
	struct _stat statBuf;
	char sSeparator[] = " \t";
	char *sToken;
	FILE *fi;
	int i;

	// 現在時刻
	tmNowSec = time(NULL);
	tmNow = localtime(&tmNowSec);

	// Speedfan のデータファイル名 （フルパス）の作成
	sprintf(sCsvPath, "%s%s%04d%02d%02d.csv", srProf->sSpeedfanPath, srProf->sLogBody, tmNow->tm_year+1900, tmNow->tm_mon+1,tmNow->tm_mday);


	// CSV ファイルのサイズ、時刻の取得
	if(_stat(sCsvPath, &statBuf) != 0)
		return 1;		// CSVファイルが存在しない

	// CSVファイルの最終書き込み時間が、現在から60秒以内かチェック
	if(abs((int)(tmNowSec - statBuf.st_mtime)) > 60)
		return 1;


	fi = fopen(sCsvPath, "rt");
	if(fi == NULL) return 1;		// ファイルオープンに失敗

	// 最終行から10行程度手前まで一気にシークする
	if(fseek(fi, statBuf.st_size - srProf->nOneLineBytes*10, SEEK_SET) != 0)
		return 1;					// シーク失敗


	// 最終行まで読み込む
	while(fgets(sTemp, 1023, fi) > 0)
	{
	}

	fclose(fi);

	strcpy(sTemp2, sTemp);		// 切り出し時の破壊に備えて保存

	// カラム値の不正
	if(srProf->nCpuTempColumn<=0 || srProf->nMbTempColumn<=0) return 1;

	// CPU 温度を切り出す
	sToken = strtok(sTemp, sSeparator);
	for(i=1; i<srProf->nCpuTempColumn && sToken != NULL; i++)
	{
		sToken = strtok(NULL, sSeparator);
	}
	if(sToken == NULL) return 1;

	srReadData->nCpuTemp = atof(sToken);

	// MB 温度を切り出す
	sToken = strtok(sTemp2, sSeparator);
	for(i=1; i<srProf->nMbTempColumn && sToken != NULL; i++)
	{
		sToken = strtok(NULL, sSeparator);
	}
	if(sToken == NULL) return 1;

	srReadData->nMbTemp = atof(sToken);


	return 0;
}


// SpeedFan の 共有メモリー より、温度データを読み込む
int ReadSpeedfanSharedMemory(__sr_prof_data *srProf, __sr_read_data *srReadData)
{
	HANDLE hSfmem;
	__speedfan_shared_memory *pSfmem;
	int i;

	// SpeedFan 共有メモリーオブジェクトを開く
	hSfmem = OpenFileMapping(FILE_MAP_READ, FALSE, "SFSharedMemory_ALM");
	if(hSfmem == NULL)
		return 1;		// エラー

	// このプロセスのバッファ空間に、共有先のメモリー空間をマッピングする
	pSfmem = (__speedfan_shared_memory *)MapViewOfFile(hSfmem, FILE_MAP_READ, 0, 0, sizeof(__speedfan_shared_memory));
	if(pSfmem == NULL)
	{
		CloseHandle(hSfmem);
		return 1;		// エラー
	}

	// カラム値の不正
	if(srProf->nCpuTempColumn<=0 || srProf->nCpuTempColumn>=32 ||
		srProf->nMbTempColumn<=0 || srProf->nMbTempColumn>=32) return 1;

	// 温度値のコピー
	i = pSfmem->tempsArray[srProf->nCpuTempColumn-1];
	srReadData->nCpuTemp = (double)i;
	i = pSfmem->tempsArray[srProf->nMbTempColumn-1];
	srReadData->nMbTemp = (double)i;

	// マッピング、オブジェクトを閉じる
	UnmapViewOfFile(pSfmem);
	CloseHandle(hSfmem);



	return 0;
}


// EOF
