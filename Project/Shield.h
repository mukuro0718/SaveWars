//===========================================
// @brief �V�[���h�N���X
//===========================================
#pragma once

class Model;
class Shield
{
public:
	Shield();//�R���X�g���N�^
	~Shield();//�f�X�g���N�^

	void		Initialize();							 //������
	void		Update	  (const VECTOR _targetPosition);//�X�V
	const void	Draw	  ()const;						 //�`��
private:
	/*�����o�ϐ�*/
	Model* model;//���f��
};

