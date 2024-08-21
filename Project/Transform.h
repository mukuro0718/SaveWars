//====================================================
// @brief �g�����X�t�H�[���N���X
// �g�嗦�E��]���E���W��ێ�����f�[�^�^�N���X
//====================================================
#pragma once

class Transform
{
public:
	Transform();			//�R���X�g���N�^
	virtual ~Transform(){}	//�f�X�g���N�^

	const float DegreeToRadian(const float _degree)const;//�x�������W�A���ɕϊ�����
	
	/*getter*/
	const VECTOR GetPosition()const { return this->position; }//���W�̎擾
	const VECTOR GetRotation()const { return this->rotation; }//��]���̎擾
	const VECTOR GetScale	()const { return this->scale;	 }//�g�嗦�̎擾

	/*setter*/
	void SetPosition		(const VECTOR _position ) { this->position  = _position; }//���W�̐ݒ�
	void SetScale			(const VECTOR _scale	) { this->scale		= _scale;	 }//�g�嗦�̐ݒ�
	void SetRotation		(const VECTOR _rotation);								  //��]���̐ݒ�
	void SetRotationOfRadian(const VECTOR _rotation);								  //��]���̐ݒ�
	void SetRotation		(const std::vector<float> _rotation);					  //��]���̐ݒ�
	void SetRotation		(const float _x, const float _y, const float _z);		  //��]���̐ݒ�

private:

	/*�����o�ϐ�*/
	VECTOR position;//���W
	VECTOR rotation;//��]��
	VECTOR scale;	//�g�嗦
};

