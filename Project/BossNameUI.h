//===================================================
// @brief �{�X�̖��O��\������N���X
// �G���g���[���Ƀ{�X��HP��\�����A
// ���̌�A�{�X��HP�̏�ɖ��O��\������
//===================================================
#pragma once

class Image;
class BossNameUI
{
public:
	BossNameUI();//�R���X�g���N�^
	~BossNameUI();//�f�X�g���N�^

		  void Initialize();	 //������
		  void Update	 ();	 //�X�V
	const void Draw		 ()const;//�`��
private:
	/*�ÓI�萔*/
	static constexpr int TEXT_COLOR = 0xffffff;

	/*�����o�ϐ�*/
	int fontHandle;
	int imageHandle;
};

