#include "StdAfx.h"
#include "CookieCtrl.h"
#include "GlobalFunc.h"
#include <io.h>			// findfirst

// ************************************************************
// �����폜 ���C�����[�`��
// ************************************************************
BOOL DeleteCookie(void)
{
	char str_path[MAX_PATH];
	char str_m_ext[MAX_PATH];
	char str_m_exclude[MAX_PATH];
	int int_days;

	char str_srch_path[MAX_PATH*2];
	char str_full_path[MAX_PATH*2];

	char *str_ext;

	struct _finddata_t finddata;
	long hFind;

	// �e��ݒ�l�̃Z�b�g
	strcpy(str_path, ::gstr_folder);
	strcpy(str_m_ext, ::gstr_ext);
	if(::gint_sw_exclude)
		strcpy(str_m_exclude, ::gstr_exclude);
	else
		strcpy(str_m_exclude, "");
	if(::gint_sw_days)
		int_days = ::gint_days;
	else
		int_days = -1;		// ���̐��őΏۊO�Ƃ���ݒ�


	// �p�X���܂��͊g���q���w�肳��Ă��Ȃ��Ƃ�
	if((strlen(str_path) == 0) || (strlen(str_m_ext) == 0)) return FALSE;

	// �p�X�̍Ō�� \ �L��
	if(str_path[strlen(str_path)-1] != '\\') strcat(str_path, "\\");


	// ************************************************************
	// �g���q�i�����w��j�̐؂蕪��   �i��F *.txt;*.url�j
	// ************************************************************
	str_ext = strtok(str_m_ext, ";");
	while(str_ext != NULL)
	{
		// �����p�X�̊���
		sprintf(str_srch_path, "%s%s", str_path, str_ext);

	    if( (hFind = _findfirst((LPCSTR)str_srch_path, &finddata )) == -1L )
		{	// 1�ڂ̃t�@�C����������Ȃ�
			str_ext = strtok(NULL, ";");	// ���̊g���q
			continue;	
		}

		// ************************************************************
		// ���C���h�J�[�h�w��ɂ��t�@�C���̌���
		// ************************************************************
		if(!(finddata.attrib & _A_SUBDIR))
		{
			sprintf(str_full_path, "%s%s", str_path, finddata.name);
			if(ChkExcludeFile(finddata.name, str_m_exclude, finddata.time_create, int_days))
				::unlink((LPCSTR)str_full_path);	// �폜
		}

		while(_findnext( hFind, &finddata ) == 0)
		{
			if(!(finddata.attrib & _A_SUBDIR))
			{
				sprintf(str_full_path, "%s%s", str_path, finddata.name);
				if(ChkExcludeFile(finddata.name, str_m_exclude, finddata.time_create, int_days))
					::unlink((LPCSTR)str_full_path);	// �폜
			}
		}
		str_ext = strtok(NULL, ";");	// ���̊g���q
	}
	return TRUE;
}

// ************************************************************
// �^����ꂽ�t�@�C�����������ׂ��Ώۂ��ǂ������f����
//
// str_fname : �Ώۃt�@�C��
// str_m_exclude : �ΏۊO�̕�����i�Z�~�R�����ŕ����w��j
// time_create : �Ώۃt�@�C���̍쐬�� (CTime)
// days : ���̓��t�ȏ㗧�����t�@�C���͏����ΏۂƂ���i���̐��Ŕ���O�j
// �������� OK �̏ꍇ TRUE
// ************************************************************
BOOL ChkExcludeFile(char *str_fname, char *str_m_exclude, time_t time_create, int days)
{
	char str_tmp[MAX_PATH];
	char *str_exclude;
	CString str_tmp_find;
	time_t time_now;

	_tzset();
	time(&time_now);

	// ���t�w�肪����Ƃ�
	if(days >= 0)
	{
		// ���t���w����o�߂��Ă���ꍇ TRUE (��v�����񂪉��ł��낤�Ƃ�)
		if(time_now > time_create + (long)(days*24*60*60)) return TRUE;
		// ��v�����񂪎w�肳��Ă��Ȃ��ꍇ ����������܂� FALSE
		if(strlen(str_m_exclude) == 0) return FALSE;
		// �A�X�^���X�N�͑S�Ă̕�����Ɉ�v�Ɖ��߂��A����������܂� FALSE
		if(strcmp(str_m_exclude, "*") == 0) return FALSE;
	}
	else
	{	// ���t�w�肪�����Ƃ�
		// ��v�����񂪎w�肳��Ă��Ȃ��ꍇ �����ɏ��� TRUE
		if(strlen(str_m_exclude) == 0) return TRUE;
	}

	// �����񂪈�v���邩�ǂ������ׂ�
	strcpy(str_tmp, str_m_exclude);
	str_exclude = strtok(str_tmp, ";");
	while(str_exclude != NULL)
	{
		str_tmp_find = str_fname;
		// ��v���������񂪖����ꍇ -1 ���Ԃ� Find �֐����g��
		if(str_tmp_find.Find((LPTSTR)str_exclude) != -1) return FALSE;	// ��v����
		str_exclude = strtok(NULL, ";");
	}

	// �P�[�X�P�F���t�w�肪����A���������B���Ă��Ȃ��ꍇ�ŁA������w�肪�����v���Ȃ��ꍇ
	// �P�[�X�Q�F���t�w�肪�����A������w�肪�����v���Ȃ��ꍇ
	//   �͏������� TRUE
	return TRUE;
}

