//============================================
// @brief ���f���N���X
// ���f�����g�p����Ƃ��Ɏg�p����f�[�^�^�N���X
//============================================

#pragma once

class Transform;
class Animation;
class Model
{
public:
	 Model(const int _modelHandle);//�R���X�g���N�^
	~Model();//�f�X�g���N�^

	const void Draw	  ()const;																				//�`��
	void AddAnimation (const std::vector<string> _animationHandle, const std::vector<int> _animationIndex);	//�A�j���[�V�����̒ǉ�
	void PlayAnimation(const int _nowAnimation, const float _animationPlayTime);							//�A�j���[�V�����̍Đ�

	/*getter*/
	const VECTOR GetPosition    ()const;								//���W�̎擾
	const VECTOR GetRotation    ()const;								//��]���̎擾
	const VECTOR GetScale	    ()const;								//�g�嗦�̎擾
	const int	 GetModelHandle ()const { return this->modelHandle; }	//���f���n���h���̎擾
	const bool	 GetIsChangeAnim()const;

	/*setter*/
	void SetPosition		 (const VECTOR	   _position );											//���W�̐ݒ�
	void SetRotation		 (const VECTOR	   _rotation );											//��]���̐ݒ�
	void SetRotationOfRadian (const VECTOR	   _rotation);											//��]���̐ݒ�
	void SetScale			 (const VECTOR	   _scale	 );											//�g�嗦�̐ݒ�
	void SetTransform		 (const VECTOR _position, const VECTOR _rotation, const VECTOR _scale);	//�g�����X�t�H�[���̐ݒ�

private:
	/*�����o�ϐ�*/
	int			modelHandle;//���f���n���h��
	Transform*	transform;	//�g�����X�t�H�[��
	Animation*	animation;	//�A�j���[�V����
};

