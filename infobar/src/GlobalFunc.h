
#ifndef __GLOBAL_FUNC
#define __GLOBAL_FUNC


extern char _sTransBuf[];		// �X���b�h�ƃ_�C�A���O�̒ʐM�p
extern char _sURL[];			// �X���b�h�ɓn�� URL
extern char _sProxy[];			// �X���b�h�ɓn�� PROXY
extern char _sPhHeader[];		// �؂蕪���p �w�b�_�[������
extern char _sPhHeaderB[];			// �؂蕪���p �w�b�_�[������ B
extern char _sPhHeaderC[];			// �؂蕪���p �w�b�_�[������ C
extern char _sTitle[];				// �^�C�g��
extern char _sItems[];				// �،��f�[�^�̎w��ȂǂɎg���A�C�e��

extern BOOL bUpdated;		// �X���b�h�ōX�V�����������ʒm
extern BOOL bInThread;		// �X���b�h���쒆�̃t���O
extern int nThreadError;	// �X���b�h�ŋN�������G���[�̎�ނ�Ԃ�
extern int _nMode;			// ���샂�[�h�i�ʏ��M�E�،���M�c�j
extern UINT _nPort;			// �X���b�h�ɓn�� �|�[�g�ԍ�
extern int _nPhSkip;		// �؂蕪���p �X�L�b�v��
extern int _nPhGetcount;	// �؂蕪���p �擾��
extern int _nDelSpace;		// �d���󔒕����̍폜���[�h		2003/06/01
extern int _nCr2Spc;		// �S�f�[�^�P�s���i���s���󔒁j���[�h		2003/06/02

extern BOOL bDebugMode;		// �f�o�b�O���[�h


void thread_main(void *pVoid);	// �X���b�h�֐�

#endif
