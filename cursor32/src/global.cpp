#include "stdafx.h"
#include "global.h"

// �t�@�C���ւ̃V���[�g�J�b�g�E�A�C�R�����쐬����B
// SDK �ł� Shell Links �ƌ����B
// pszShortcutFile : ���̃t�@�C���̃t���p�X���iC:\WINDOWS\NOTEPAD.EXE�j
// pszLink : �V���[�g�J�b�g�t�@�C���̃t���p�X���i C:\WINDOWS\�c.lnk�j
// pszDesc : �\����
//
// ���̍��ڂɊւ��ẮA MSDN���C�u��������"Using Shell Links"�̃^�C�g������������
// �֐��̐����Ȃǂ� "The IShellLink Interface"�ɂ���
HRESULT CreateShellLink(LPCSTR pszShortcutFile,
									LPSTR pszLink, LPSTR pszDesc)
{
    HRESULT hres;
    IShellLink *psl;

	CoInitialize(NULL);	// MSDN�T���v���t�@�C���ł͂����������Ă���̂Œ��ӂ���

	// IShellLink �I�u�W�F�N�g��(psl��)����
    hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
                            IID_IShellLink, (void **)&psl);
    if (SUCCEEDED(hres))
    {
       IPersistFile *ppf;
       
	   // Query IShellLink for the IPersistFile interface for
	   // saving the shortcut in persistent storage.
       hres = psl->QueryInterface(IID_IPersistFile, (void **)&ppf);
	   if (SUCCEEDED(hres))
	   {   
	        WORD wsz[MAX_PATH];   // buffer for Unicode string

	   		// ���̃t�@�C���i�^�[�Q�b�g�j�̃p�X��ݒ�
          	hres = psl->SetPath(pszShortcutFile);

			if (!SUCCEEDED(hres))
				AfxMessageBox("�V���[�g�J�b�g�쐬���ɃG���[���N�����܂���",
						MB_OK|MB_ICONINFORMATION|MB_TOPMOST);

            // �V���[�g�J�b�g�̐�����(Description)���Z�b�g
			// ���̂��߂Ɏg������������A�s���H
          	hres = psl->SetDescription(pszDesc);

			if (!SUCCEEDED(hres))
				AfxMessageBox("�V���[�g�J�b�g�쐬���ɃG���[���N�����܂���",
						MB_OK|MB_ICONINFORMATION|MB_TOPMOST);

	     	// �V���[�g�J�b�g�t�@�C������MBCS �� ���C�h�L�����N�^�������
	     	MultiByteToWideChar(CP_ACP, 0, pszLink, -1, wsz, MAX_PATH);

          	// �V���[�g�J�b�g�t�@�C�����f�B�X�N�ɏ�������
			// Save the shortcut via the IPersistFile::Save member function.
          	hres = ppf->Save(wsz, TRUE);

			if (!SUCCEEDED(hres))
				AfxMessageBox("�V���[�g�J�b�g�쐬���ɃG���[���N�����܂���",
						MB_OK|MB_ICONINFORMATION|MB_TOPMOST);

            // Release the pointer to IPersistFile.
          	ppf->Release();
       }
       // Release the pointer to IShellLink.
       psl->Release();
    }
	CoUninitialize();
    return hres;
}

