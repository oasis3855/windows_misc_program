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

	// -h/-? �I�v�V�������t����ꂽ�Ƃ��ɂ́A����������ʂɕ\��
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

	// INI�t�@�C������ݒ�l��ǂݍ���
	// �iINI�t�@�C�������݂��Ȃ��Ƃ��ɂ́AINI�t�@�C�����쐬����j
	if(GetInitData(&srProf) != 0)
	{
		puts("0\n0\n0\nError (INI file error)\n");
		return 1;	// �ǂݍ��݃G���[ �i��������INI�t�@�C���V�K�쐬�j
	}

	if(!strcmp(srProf.sMode, "csv"))
	{
		// SpeedFan �� CSV ����f�[�^��ǂݍ���
		if(ReadSpeedfanCsv(&srProf, &srReadData) != 0)
		{
			puts("0\n0\n0\nError (SpeedFan CSV read error)\n");
			return 1;	// �ǂݍ��݃G���[
		}
	}
	else if(!strcmp(srProf.sMode, "sharemem"))
	{
		// SpeedFan �� ���L�������[ ����f�[�^��ǂݍ���
		if(ReadSpeedfanSharedMemory(&srProf, &srReadData) != 0)
		{
			puts("0\n0\n0\nError (SpeedFan Shared Memory read error)\n");
			return 1;	// �ǂݍ��݃G���[
		}
	}
	else
	{
		puts("0\n0\n0\nError (mode must be sharemem/csv)\n");
		return 1;	// �ǂݍ��݃G���[
	}

	// ���x�Ƀt�@�N�^�[���|����
	nCpuTemp = (int)(srReadData.nCpuTemp * srProf.nFactor);
	nMbTemp = (int)(srReadData.nMbTemp * srProf.nFactor);

	// Windows �N������̎��Ԃ𓾂�
	nUptime = GetTickCount();
	sprintf(sUptime, srProf.sOutputUptimeStr, nUptime/(24*3600000L), (nUptime/3600000L)%24, (nUptime/60000)%60);

	// ��ʕ\���@�imrtg�̃f�[�^�\�������ɂ̂��Ƃ�A4�s�\���j
	printf("%d\n%d\n%s\n%s\n", nMbTemp, nCpuTemp, sUptime, srProf.sOutputTempStr);

	return 0;		// ����I��
}


// INI �t�@�C������ݒ�l��ǂݍ���
int GetInitData(__sr_prof_data *srProf)
{
	char sModulePath[1024];
	char *sPtr;
	char sTemp[1024];
	int i;
	size_t nStrSize;

	// ���݂̃��W���[���̃t���p�X���𓾂�
	i = GetModuleFileName(NULL, sTemp, 1023);
	if(i<=0 || i>=1023) return 1;		// ���݂̃v���Z�X�̃p�X���������Ȃ�����


	// ���W���[���p�X�̍Ō�� . �̈ʒu�܂ł��t�@�C�����̃{�f�B�Ƃ���
	sPtr = strrchr(sTemp, '.');
	if(sPtr == NULL)
		nStrSize = strlen(sTemp);		// . �������ꍇ�A�p�X���S�̂Ƃ���
	else
		nStrSize = strlen(sTemp) - strlen(sPtr);
	strncpy(sModulePath, sTemp, nStrSize);
	sModulePath[nStrSize] = (char)NULL;

	// INI �g���q������
	strcat(sModulePath, ".ini");


	// INI�t�@�C�������݂��邩�̊m�F
	GetPrivateProfileString("sf2mrtg", "install", "err", sTemp, 1023, sModulePath);
	if(!strcmp(sTemp, "err"))
	{
		WriteInitIniFile();		// INI ���m�F�ł��Ȃ��ꍇ�A�V�K�쐬
		return 1;
	}


	// �ݒ�l�ǂݍ���

	GetPrivateProfileString("sf2mrtg", "Mode", "err", srProf->sMode, 31, sModulePath);
	if(!strcmp(srProf->sSpeedfanPath, "err")) return 1;			// �ݒ�l�ǂݍ��ݕs�\

	GetPrivateProfileString("sf2mrtg", "SpeedFanPath", "err", srProf->sSpeedfanPath, 1023, sModulePath);
	if(!strcmp(srProf->sSpeedfanPath, "err")) return 1;			// �ݒ�l�ǂݍ��ݕs�\

	GetPrivateProfileString("sf2mrtg", "CpuTempColumn", "err", sTemp, 1023, sModulePath);
	if(!strcmp(sTemp, "err")) return 1;							// �ݒ�l�ǂݍ��ݕs�\
	srProf->nCpuTempColumn = atoi(sTemp);

	GetPrivateProfileString("sf2mrtg", "MbTempColumn", "err", sTemp, 1023, sModulePath);
	if(!strcmp(sTemp, "err")) return 1;							// �ݒ�l�ǂݍ��ݕs�\
	srProf->nMbTempColumn = atoi(sTemp);

	GetPrivateProfileString("sf2mrtg", "OutputUptimeStr", "err", srProf->sOutputUptimeStr, 254, sModulePath);
	if(!strcmp(srProf->sOutputUptimeStr, "err")) return 1;		// �ݒ�l�ǂݍ��ݕs�\

	GetPrivateProfileString("sf2mrtg", "OutputTempStr", "err", srProf->sOutputTempStr, 254, sModulePath);
	if(!strcmp(srProf->sOutputTempStr, "err")) return 1;		// �ݒ�l�ǂݍ��ݕs�\

	GetPrivateProfileString("sf2mrtg", "SpeedFanLogBody", "err", srProf->sLogBody, 254, sModulePath);
	if(!strcmp(srProf->sLogBody, "err")) return 1;		// �ݒ�l�ǂݍ��ݕs�\

	GetPrivateProfileString("sf2mrtg", "CsvOneLineBytes", "err", sTemp, 1023, sModulePath);
	if(!strcmp(sTemp, "err")) return 1;							// �ݒ�l�ǂݍ��ݕs�\
	srProf->nOneLineBytes = atoi(sTemp);

	GetPrivateProfileString("sf2mrtg", "Factor", "err", sTemp, 1023, sModulePath);
	if(!strcmp(sTemp, "err")) return 1;							// �ݒ�l�ǂݍ��ݕs�\
	srProf->nFactor = atof(sTemp);


	return 0;
}


// �V�K��INI�t�@�C�����쐬����
void WriteInitIniFile(void)
{
	char sModulePath[1024];
	char *sPtr;
	char sTemp[1024], sTemp2[1280];
	int i;
	size_t nStrSize;

	// ���݂̃��W���[���̃t���p�X���𓾂�
	i = GetModuleFileName(NULL, sTemp, 1023);
	if(i<=0 || i>=1023) return ;		// ���݂̃v���Z�X�̃p�X���������Ȃ�����


	// ���W���[���p�X�̍Ō�� . �̈ʒu�܂ł��t�@�C�����̃{�f�B�Ƃ���
	sPtr = strrchr(sTemp, '.');
	if(sPtr == NULL)
		nStrSize = strlen(sTemp);		// . �������ꍇ�A�p�X���S�̂Ƃ���
	else
		nStrSize = strlen(sTemp) - strlen(sPtr);
	strncpy(sModulePath, sTemp, nStrSize);
	sModulePath[nStrSize] = (char)NULL;

	// INI �g���q������
	strcat(sModulePath, ".ini");

	// �R�����g��������쐬 �imrtg.cfg�ւ̐ݒ���@������������j
	GetModuleFileName(NULL, sTemp, 1023);
	sprintf(sTemp2, "set mrtg.cfg target section as \"Target[OutFile]: `%s`\"", sTemp);

	// �T���v����INI�t�@�C������������

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

// SpeedFan �� CSV �t�@�C�����A���x�f�[�^��ǂݍ���
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

	// ���ݎ���
	tmNowSec = time(NULL);
	tmNow = localtime(&tmNowSec);

	// Speedfan �̃f�[�^�t�@�C���� �i�t���p�X�j�̍쐬
	sprintf(sCsvPath, "%s%s%04d%02d%02d.csv", srProf->sSpeedfanPath, srProf->sLogBody, tmNow->tm_year+1900, tmNow->tm_mon+1,tmNow->tm_mday);


	// CSV �t�@�C���̃T�C�Y�A�����̎擾
	if(_stat(sCsvPath, &statBuf) != 0)
		return 1;		// CSV�t�@�C�������݂��Ȃ�

	// CSV�t�@�C���̍ŏI�������ݎ��Ԃ��A���݂���60�b�ȓ����`�F�b�N
	if(abs((int)(tmNowSec - statBuf.st_mtime)) > 60)
		return 1;


	fi = fopen(sCsvPath, "rt");
	if(fi == NULL) return 1;		// �t�@�C���I�[�v���Ɏ��s

	// �ŏI�s����10�s���x��O�܂ň�C�ɃV�[�N����
	if(fseek(fi, statBuf.st_size - srProf->nOneLineBytes*10, SEEK_SET) != 0)
		return 1;					// �V�[�N���s


	// �ŏI�s�܂œǂݍ���
	while(fgets(sTemp, 1023, fi) > 0)
	{
	}

	fclose(fi);

	strcpy(sTemp2, sTemp);		// �؂�o�����̔j��ɔ����ĕۑ�

	// �J�����l�̕s��
	if(srProf->nCpuTempColumn<=0 || srProf->nMbTempColumn<=0) return 1;

	// CPU ���x��؂�o��
	sToken = strtok(sTemp, sSeparator);
	for(i=1; i<srProf->nCpuTempColumn && sToken != NULL; i++)
	{
		sToken = strtok(NULL, sSeparator);
	}
	if(sToken == NULL) return 1;

	srReadData->nCpuTemp = atof(sToken);

	// MB ���x��؂�o��
	sToken = strtok(sTemp2, sSeparator);
	for(i=1; i<srProf->nMbTempColumn && sToken != NULL; i++)
	{
		sToken = strtok(NULL, sSeparator);
	}
	if(sToken == NULL) return 1;

	srReadData->nMbTemp = atof(sToken);


	return 0;
}


// SpeedFan �� ���L�������[ ���A���x�f�[�^��ǂݍ���
int ReadSpeedfanSharedMemory(__sr_prof_data *srProf, __sr_read_data *srReadData)
{
	HANDLE hSfmem;
	__speedfan_shared_memory *pSfmem;
	int i;

	// SpeedFan ���L�������[�I�u�W�F�N�g���J��
	hSfmem = OpenFileMapping(FILE_MAP_READ, FALSE, "SFSharedMemory_ALM");
	if(hSfmem == NULL)
		return 1;		// �G���[

	// ���̃v���Z�X�̃o�b�t�@��ԂɁA���L��̃������[��Ԃ��}�b�s���O����
	pSfmem = (__speedfan_shared_memory *)MapViewOfFile(hSfmem, FILE_MAP_READ, 0, 0, sizeof(__speedfan_shared_memory));
	if(pSfmem == NULL)
	{
		CloseHandle(hSfmem);
		return 1;		// �G���[
	}

	// �J�����l�̕s��
	if(srProf->nCpuTempColumn<=0 || srProf->nCpuTempColumn>=32 ||
		srProf->nMbTempColumn<=0 || srProf->nMbTempColumn>=32) return 1;

	// ���x�l�̃R�s�[
	i = pSfmem->tempsArray[srProf->nCpuTempColumn-1];
	srReadData->nCpuTemp = (double)i;
	i = pSfmem->tempsArray[srProf->nMbTempColumn-1];
	srReadData->nMbTemp = (double)i;

	// �}�b�s���O�A�I�u�W�F�N�g�����
	UnmapViewOfFile(pSfmem);
	CloseHandle(hSfmem);



	return 0;
}


// EOF
