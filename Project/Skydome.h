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
	int model;//���f��
	VECTOR position;
	VECTOR rotation;
	VECTOR scale;
};

