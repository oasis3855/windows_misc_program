// BacklightControlTg.cpp : Defines the entry point for the application.
//

// ***********************
// BacklightControl for CE  (Windows CE �̃o�b�N���C�g����j
// 
// Copyright (C) 2008 INOUE. Hirokazu
//
// 
// http://www.opensource.jp/gpl/gpl.ja.html
// ���̃v���O�����̓t���[�\�t�g�E�F�A�ł��B���Ȃ��͂�����A�t���[�\�t�g�E�F
// �A���c�ɂ���Ĕ��s���ꂽ GNU ��ʌ��O���p�����_��(�o�[�W����2���A��
// �]�ɂ���Ă͂���ȍ~�̃o�[�W�����̂����ǂꂩ)�̒�߂�����̉��ōĔЕz
// �܂��͉��ς��邱�Ƃ��ł��܂��B
// 
// ���̃v���O�����͗L�p�ł��邱�Ƃ�����ĔЕz����܂����A*�S���̖��ۏ�* 
// �ł��B���Ɖ\���̕ۏ؂����̖ړI�ւ̓K�����́A���O�Ɏ����ꂽ���̂���
// �ߑS�����݂��܂���B�ڂ�����GNU ��ʌ��O���p�����_�񏑂��������������B
// 
// ���Ȃ��͂��̃v���O�����Ƌ��ɁAGNU ��ʌ��O���p�����_�񏑂̕��������ꕔ
// �󂯎�����͂��ł��B�����󂯎���Ă��Ȃ���΁A�t���[�\�t�g�E�F�A���c��
// �Ő������Ă�������(����� the Free Software Foundation, Inc., 59
// Temple Place, Suite 330, Boston, MA 02111-1307 USA)�B
//

#include "stdafx.h"
#include "BacklightControlTg.h"

#include <pm.h>


int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    lpCmdLine,
					int       nCmdShow)
{
	CEDEVICE_POWER_STATE devState;

	if(::GetDevicePower(_T("BKL1:"), POWER_NAME, &devState) != ERROR_SUCCESS)
	{
		// �o�b�N���C�g�̏�Ԃ̓ǂݏo���Ɏ��s
		return 0;
	}

	if(devState == D0)
	{	// �o�b�N���C�g��ON�̂Ƃ�

		// �o�b�N���C�g�� OFF�ɂ���
		::SetDevicePower(_T("BKL1:"), POWER_NAME, D4);	// OFF

	}
	else
	{	// �o�b�N���C�g��OFF�̂Ƃ� �iON�ȊO�̂Ƃ����ׂāj

		// �o�b�N���C�g�� ON�ɂ���

		::SetDevicePower(_T("BKL1:"), POWER_NAME, D4);	// OFF (��UOFF�ɂ��Ȃ��Ɣ������Ȃ��ꍇ����)
		::Sleep(200);
		::SetDevicePower(_T("BKL1:"), POWER_NAME, PwrDeviceUnspecified);	// �������䃂�[�h

	}

	return 0;
}

