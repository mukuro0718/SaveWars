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
		  void Update	 ();		//�X�V
	const void Draw		 ()const;	//�`��
private:
	/*�����o�ϐ�*/
	int		model;		//���f��
	float	addRotate;	//��]���̑���
	VECTOR	position;	//���W
	VECTOR	rotation;	//��]�l
	VECTOR	scale;		//�g�嗦
};

