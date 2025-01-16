//===================================================
// @�v���C���[�A�N�V�����x�[�X�N���X
//===================================================
#pragma once

class Player;
class PlayerAction abstract
{
public:
			 PlayerAction(); //�R���X�g���N�^
	virtual ~PlayerAction(){}//�f�X�g���N�^

	virtual void Initialize	() abstract;				//������
	virtual void Finalize	() abstract;				//�㏈��
	virtual void Update		(Player& _player) abstract;	//�X�V

	/*getter/setter*/
	const bool GetIsChangeAction()const { return this->isChangeAction; }//�A�N�V�����ύX�t���O�̎擾
	const bool GetIsEndAction	()const { return this->isEndAction; }	//�A�N�V�����ύX�t���O�̎擾
protected:
	/*�\����*/
	struct MoveData
	{
		VECTOR nextRotation = { 0.0f,0.0f,0.0f };
		float maxSpeed		= 0.0f;
		bool isSkip			= false;
		bool isLerp			= false;
		void Set(const VECTOR _nextRotation, const float _speed, const bool _isSkip, const bool _isLerp)
		{
			this->nextRotation = _nextRotation;
			this->maxSpeed = _speed;
			this->isSkip = _isSkip;
			this->isLerp = _isLerp;
		}
	};

	/*���������֐�*/
	void	Move			(Player& _player, MoveData& _set);																//�ړ�
	void	UpdateRotation	(const bool isSkip, VECTOR& _nextRotation, VECTOR& _nowRotation);								//��]���̍X�V
	void	UpdateSpeed		(float& _nowSpeed, const float _maxSpeed, const VECTOR nowRotation, const VECTOR _nextRotation);//�ړ����x�̍X�V
	VECTOR	UpdateVelocity	(const VECTOR _rotation, const VECTOR _prevVelocity, const float _speed, const bool _isLerp);	//�ړ��x�N�g���̍X�V

	/*�����o�ϐ�*/
	bool  isChangeAction;//�A�N�V�����̕ύX���ł����ԂɂȂ�΂��̃t���O�𗧂Ă�
	bool  isEndAction;//�A�N�V���������ׂďI�������炱�̃t���O�𗧂Ă�
	short frameCount;
	int	  frameTime;//�����܂łɂ�����������
	float staminaRecoveryValue;
	float maxStamina;
	float playTime;
	int   nextAnimation;
	VECTOR rotateLerpValue;
	VECTOR velocityLerpValue;
	float accel;
	float decel;
	float totalPlayTime;
	float nowTotalPlayTime;
};

