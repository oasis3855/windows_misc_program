/***************************************************

DllFelicaLib2.cpp : DLL�̃G���g���|�C���g�֐����`���܂�

  Copyright (C) 2007, INOUE. Hirokazu
  All rights reserved.

���̃\�t�g�E�G�A�ł́Afelicalib ���C�u�����𗘗p���Ă��܂�
  Copyright (C) 2007, Takuya Murakami
  All rights reserved.

The BSD License (http://opensource.org/licenses/bsd-license.php)
---------------
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer. 
* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution. 
* Neither the name of the this project nor the names of its contributors may
  be used to endorse or promote products derived from this software without
  specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 THE POSSIBILITY OF SUCH DAMAGE.

�ȉ��̓��{���͎Q�l�̂��߂ɓY�t�������̂ł��B�p��ł������@���I�ɗL���ł���
���Ƃɒ��ӂ��Ă��������B

BSD���C�Z���X���� �i���{���j
    (http://sourceforge.jp/projects/opensource/wiki/licenses%2Fnew_BSD_license)
-----------------------------
�\�[�X�R�[�h�`�����o�C�i���`�����A�ύX���邩���Ȃ������킸�A�ȉ��̏�����
�������ꍇ�Ɍ���A�ĔЕz����юg�p��������܂��B

* �\�[�X�R�[�h���ĔЕz����ꍇ�A��L�̒��쌠�\���A�{�����ꗗ�A����щ��L
  �Ɛӏ������܂߂邱�ƁB 
* �o�C�i���`���ōĔЕz����ꍇ�A�Еz���ɕt���̃h�L�������g���̎����ɁA��L��
  ���쌠�\���A�{�����ꗗ�A����щ��L�Ɛӏ������܂߂邱�ƁB 
* ���ʂɂ����ʂ̋��Ȃ��ɁA�{�\�t�g�E�F�A����h���������i�̐�`�܂��͔̔�
  ���i�ɁA���̃v���W�F�N�g�̖��O�܂��̓R���g���r���[�^�[�̖��O���g�p���Ă�
  �Ȃ�Ȃ��B

�{�\�t�g�E�F�A�́A���쌠�҂���уR���g���r���[�^�[�ɂ���āu����̂܂܁v��
����Ă���A�����َ����킸�A���ƓI�Ȏg�p�\���A����ѓ���̖ړI�ɑ΂���
�K�����Ɋւ���Öق̕ۏ؂��܂߁A�܂�����Ɍ��肳��Ȃ��A�����Ȃ�ۏ؂�
����܂���B���쌠�҂��R���g���r���[�^�[���A���R�̂�������킸�A���Q������
������������킸�A���ӔC�̍������_��ł��邩���i�ӔC�ł��邩�i�ߎ�
���̑��́j�s�@�s�ׂł��邩���킸�A���ɂ��̂悤�ȑ��Q����������\����
�m�炳��Ă����Ƃ��Ă��A�{�\�t�g�E�F�A�̎g�p�ɂ���Ĕ��������i��֕i�܂���
��p�T�[�r�X�̒��B�A�g�p�̑r���A�f�[�^�̑r���A���v�̑r���A�Ɩ��̒��f���܂߁A
�܂�����Ɍ��肳��Ȃ��j���ڑ��Q�A�Ԑڑ��Q�A�����I�ȑ��Q�A���ʑ��Q�A�����I
���Q�A�܂��͌��ʑ��Q�ɂ��āA��ؐӔC�𕉂�Ȃ����̂Ƃ��܂��B



***************************************************/

#include "stdafx.h"
#include "_FelicaLib\felicalib.h"


BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
    return TRUE;
}


/**************
FeliCa ���AIDm, PMm ��ǂݏo��

sIdReturn :		char(33) �ǂݏo����IDm��PMm���i�[���镶����
				��F"01020304050607080102030405060708"
				16�i����ID�𕶎���ɕϊ��������̂ł���
�߂�l :		0: �G���[
				1: ����i�ǂݍ��ݐ����j
***************/
int ReadFelicaIdOnce(char *sIdReturn)
{
	pasori *p;
	felica *f;
	char sTemp[128];

	if(!GetFelicaDllPath())
	{
		return(0);
	}

	p = pasori_open(NULL);

	if(!p)
	{
		return(0);
	}

	pasori_init(p);

	f = felica_polling(p, POLLING_ANY, 0, 0);
	if(!f)
	{
		pasori_close(p);
		return(0);
	}

	HexDumpToStr(f->IDm, sTemp, 8);
	strcpy(sIdReturn, sTemp);

	HexDumpToStr(f->PMm, sTemp, 8);
	strcat(sIdReturn, sTemp);

	felica_free(f);
	pasori_close(p);


	return(1);

}