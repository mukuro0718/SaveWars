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
	/*�񋓑�*/
	enum class PositionType
	{
		ENTRY,//�o�ꎞ
		NORMAL,//�ʏ펞
	};

	/*���������֐�*/
	void AddTextIndex();
	void ChangePositionType();

	/*�ÓI�萔*/
	static constexpr int TEXT_COLOR = 0xdfffff;

	/*�����o�ϐ�*/
	vector<int> fontHandle;
	int positionType;//���W�̎��
	int drawTextIndex;//�`�悷�镶���̔z��
	int addTextIndexInterval;
	int entryInterval;
};

