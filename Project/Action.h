//============================================================
// @brief �A�N�V�����pUI�N���X
//============================================================
#pragma once
class Action
{
public:
	Action();//�R���X�g���N�^
	~Action();//�f�X�g���N�^

	const void Draw()const;//�`��
private:
	/*�ÓI�萔*/
	static constexpr int ELEMENT_NUM = 5;

	/*�\����*/
	struct Vector2
	{
		int x, y;
	};

	/*�����o�ϐ�*/
	int elementImageHandle[ELEMENT_NUM];//�����A�C�R���摜�n���h��
};

