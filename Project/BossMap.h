//===========================================
// @brief �n�ʃN���X
// �n�ʂ̕`����s��
//===========================================
#pragma once

class Model;
class BossMap
{
public:
	BossMap();//�R���X�g���N�^
	~BossMap();//�f�X�g���N�^

	void Initialize();		//������
	void Update();			//�X�V
	const void Draw()const;	//�`��

private:
	/*�����o�ϐ�*/
	Model* model;
};

