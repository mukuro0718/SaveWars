//==================================
// @brief �X�J�C�h�[���N���X
//==================================
#pragma once

class Model;
class Skydome
{
public:
	Skydome();
	~Skydome();

	void Initialize();		//������
	const void Draw()const;	//�`��
private:
	Model* model;//���f��
};

