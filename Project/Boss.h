//===============================================
// @brief �{�X�N���X
//===============================================
#pragma once

class Character;
class BossAction;
class Boss : public Character
{
public:
	Boss();//�R���X�g���N�^
	~Boss();//�f�X�g���N�^

	void		Initialize		 () override;		//������
	void		Finalize		 () override;		//�㏈��
	void		Update			 () override;		//�X�V
	const void	DrawCharacterInfo() const override;	//�`��
	void		PlayAnimation();

	/*getter/setter*/
	const int    GetAngryState			()const { return this->angryState; }		//�{���Ԃ̎擾
	const VECTOR GetHeadPosition		()const;									//���̍��W���擾
	const float	 GetAnimationPlayTime	()const;									//�A�j���[�V�����Đ����Ԃ̎擾
	const bool	 GetIsAttack			()const override;							//�R�E�Q�L������
	const float  GetSpeed				()const { return this->speed; }				//�X�s�[�h�̎擾
	const VECTOR GetNowMoveTarget		()const { return this->moveTarget; }		//���݂̈ړ��ڕW�̎擾
	const int	 GetNowPhase			()const { return this->nowPhase; }			//���̃t�F�[	�Y�̎擾
	const int	 GetPrevPhase			()const { return this->prevPhase; }			//�O�̃t�F�[�Y�̎擾
	const float  GetNowAnimationPlayTime()const { return this->animationPlayTime; }	//���݂̃A�j���[�V�����Đ����Ԃ̎擾
		  void	 SetRotation			(const VECTOR _rotation);											//��]���̐ݒ�
		  void	 SetVelocity			(const VECTOR _velocity);											//�ړ��x�N�g���̐ݒ�
		  void	 SetNowMoveTarget		(const VECTOR _moveTarget)	{ this->moveTarget = _moveTarget; }		//���݂̈ړ��ڕW��ݒ�
		  void	 UnifyPhases			()							{ this->prevPhase = this->nowPhase; }	//�t�F�[�Y�𓝈ꂷ��
		  void	 SetSpeed				(const float _speed)		{ this->speed = _speed; }				//�����̐ݒ�
		  void	 SetAnimationPlayTime	(const float _playTime)		{ this->animationPlayTime = _playTime; }//�A�j���[�V�����Đ����Ԃ̐ݒ�
		  void	 SetNowAnimation		(const int _animation)		{ this->nowAnimation = _animation; }	//���݂̃A�j���[�V�����̐ݒ�
		  void	 OffIsDraw				()							{ this->isDraw = false; }				//�`��t���O������

	/*�񋓑�*/
	//�A�j���[�V����
	enum class AnimationType
	{
		 DYING			= 0, //�f�X
		 IDLE			= 1, //�ҋ@
		 ROAR			= 2, //���K
		 WALK			= 3, //����
		 WALK_LEFT		= 4, //����
		 WALK_RIGHT		= 5, //����
		 SLASH			= 6, //�X���b�V��
		 STAB			= 7, //�˂��h���U��
		 ROTATE_PUNCH	= 8,//��]�p���`
	};
	//�t�F�[�Y
	enum class Phase
	{
		PHASE_1,
		PHASE_2,
		PHASE_3,
	};
	//�U���̎��
	enum class AttackType
	{
		NONE		 = -1,
		SLASH		 = 0,//�p���`
		STAB		 = 1,//�˂��h���U��
		ROTATE_PUNCH = 2,//��]�p���`
	};
	enum class AngryStateType
	{
		NORMAL,
		TIRED,
		ANGRY,
	};
private:
	/*�ÓI�萔*/
	static constexpr int COUNT_NUM = 6;
	//��{���
	static constexpr unsigned int DYING	= (1 << 0);//�f�X
	static constexpr unsigned int IDLE	= (1 << 1);//�ҋ@
	static constexpr unsigned int ROAR	= (1 << 2);//���K
	static constexpr unsigned int WALK	= (1 << 3);//����
	static constexpr unsigned int REST	= (1 << 4);//�x�e
	//�U��
	static constexpr unsigned int SLASH			 = (1 << 5);//�p���`
	static constexpr unsigned int STAB			 = (1 << 6);//����
	static constexpr unsigned int ROTATE_PUNCH	 = (1 << 7);//�X���b�V��

	static constexpr unsigned int MASK_ATTACK = SLASH | ROTATE_PUNCH | STAB ;
	static constexpr unsigned int MASK_ALL	  = MASK_ATTACK | WALK | DYING | REST | REST | IDLE | ROAR;
	
	/*�񋓑�*/
	//�R���C�_�[�̎��
	enum class ColliderType
	{
		CHARACTER = 0,
		ATTACK = 1,
	};
	//�t���[���J�E���g�̎��
	enum class FrameCountType
	{
		SLASH			= 0,
		STAB			= 1,
		ROTATE_PUNCH	= 2,
		REST			= 3,
	};
	enum class ActionType
	{
		NONE = -1,
		DYING			= 0,//�f�X
		IDLE			= 1,//�ҋ@
		ROAR			= 2,//���K
		WALK			= 3,//����
		WALK_LEFT		= 4,//����
		WALK_RIGHT		= 5,//����
		REST			= 6,//����
		SLASH			= 7,//�X���b�V��
		STAB			= 8,//�˂��h���U��
		ROTATE_PUNCH	= 9,//��]�p���`
	};

	/*���������֐�*/
	void ChangeState();
	void SetPhase	();
	void SetAngryState();

	/*�����o�ϐ�*/
	std::map<int, unsigned int>	actionTypeMap;			//�A�N�V�����^�C�v
	std::vector<BossAction*>	parameters;				//�A�N�V�����p�����[�^�[
	VECTOR						moveTarget;				//�ړ��ڕW
	float						animationPlayTime;		//�A�j���[�V�����Đ�����
	int							nowAnimation;			//���݂̃A�j���[�V����
	int							nowPhase;				//���݂̃t�F�[�Y
	int							prevPhase;				//�O�̃t�F�[�Y
	int							actionType;
	float angryValue;
	int angryState;
	int tiredInterval;
};

