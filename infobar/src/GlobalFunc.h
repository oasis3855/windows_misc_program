


extern volatile BOOL bUpdated;			// �X���b�h�ōX�V�����������ʒm
extern volatile BOOL bInThread;			// �X���b�h���쒆�̃t���O
extern volatile int nThreadError;		// �X���b�h�ŋN�������G���[�̎�ނ�Ԃ�
extern CString sTransBuf;		// �X���b�h�ƃ_�C�A���O�̒ʐM�p
extern CString sURL;			// �X���b�h�ɓn�� URL
extern CString sProxy;			// �X���b�h�ɓn�� PROXY
extern volatile UINT nPort;				// �X���b�h�ɓn�� �|�[�g�ԍ�
extern CString sPhHeader;		// �؂蕪���p �w�b�_�[������
extern volatile int nPhSkip;	// �؂蕪���p �X�L�b�v��
extern volatile int nPhGetcount;	// �؂蕪���p �擾��
extern CString sTitle;				// �^�C�g��


void thread_main(void *pVoid);	// �X���b�h�֐�
