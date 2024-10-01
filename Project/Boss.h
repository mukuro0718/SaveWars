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
	const void	DrawCharacterInfo() const override;//�`��

	/*getter*/
	const float  GetSpeed				()const { return this->speed; }
	const VECTOR GetHeadPosition		()const;
	const VECTOR GetNowMoveTarget		()const { return this->moveTarget; }
	const bool	 GetIsAttack			()const override;
	const int	 GetNowPhase			()const { return this->nowPhase; }									//���̃t�F�[	�Y�̎擾
	const int	 GetPrevPhase			()const { return this->prevPhase; }								//�O�̃t�F�[�Y�̎擾
	const float	 GetAnimationPlayTime	()const;															//�A�j���[�V�����Đ����Ԃ̎擾
	const float  GetNowAnimationPlayTime()const { return this->animationPlayTime; }
		  void	 UnifyPhases			() { this->prevPhase = this->nowPhase; }							//�t�F�[�Y�𓝈ꂷ��
		  void	 SetNowMoveTarget		(const VECTOR _moveTarget) { this->moveTarget = _moveTarget; }
		  void	 SetRotation			(const VECTOR _rotation);											//��]���̐ݒ�
		  void	 SetVelocity			(const VECTOR _velocity);											//�ړ��x�N�g���̐ݒ�
		  void	 SetSpeed				(const float _speed) { this->speed = _speed; }						//�����̐ݒ�
		  void	 SetAnimationPlayTime	(const float _playTime) { this->animationPlayTime = _playTime; }
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
	static constexpr unsigned int FLY_ATTACK	 = (1 << 6);//�n�ʂ�������
	static constexpr unsigned int HURRICANE_KICK = (1 << 7);//��]�R��
	static constexpr unsigned int JUMP_ATTACK	 = (1 << 8);//����
	static constexpr unsigned int ROTATE_PUNCH	 = (1 << 9);//�X���b�V��

	static constexpr unsigned int MASK_ATTACK = SLASH | ROTATE_PUNCH | JUMP_ATTACK |HURRICANE_KICK | FLY_ATTACK;
	static constexpr unsigned int MASK_ALL = MASK_ATTACK | WALK | DYING | REST | REST | IDLE | ROAR;
	
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
		SLASH = 0,
		FLY_ATTACK = 1,
		HURRICANE_KICK = 2,
		JUMP_ATTACK = 3,
		ROTATE_PUNCH = 4,
		REST = 5,
	};
	enum class Phase
	{
		PHASE_1,
		PHASE_2,
		PHASE_3,
	};
	enum class AttackType
	{
		NONE			 = -1,
		SLASH			 = 0,//�p���`
		FLY_ATTACK		 = 1,//�n�ʂ�������
		HURRICANE_KICK	 = 2,//��]�R��
		JUMP_ATTACK		 = 3,//�˂��h���U��
		ROTATE_PUNCH	 = 4,//��]�p���`
	};
	enum class AnimationType
	{
		DYING			= 0,//�f�X
		IDLE			= 1,//�ҋ@
		ROAR			= 2,//���K
		WALK			= 3,//����

		SLASH			= 4,//�X���b�V��
		FLY_ATTACK		= 5,//��эU��
		HURRICANE_KICK	= 6,//��]�R��
		JUMP_ATTACK		= 7,//�˂��h���U��
		ROTATE_PUNCH	= 8,//��]�p���`
	};
	enum class ActionType
	{
		DYING			= 0,//�f�X
		IDLE			= 1,//�ҋ@
		ROAR			= 2,//���K
		WALK			= 3,//����
		REST			= 4,//����
		SLASH			= 5,//�X���b�V��
		FLY_ATTACK		= 6,//��эU��
		HURRICANE_KICK	= 7,//��]�R��
		JUMP_ATTACK		= 8,//�˂��h���U��
		ROTATE_PUNCH	= 9,//��]�p���`
	};

	/*���������֐�*/
	void ChangeState();
	void SetPhase	();

	/*�����o�ϐ�*/
	std::map<unsigned int, int>	stateAnimationMap;		//���ڂ��Ƃ̃A�j���[�V����
	std::map<int, unsigned int>	actionTypeMap;			//�A�N�V�����^�C�v
	std::vector<BossAction*>	parameters;				//�A�N�V�����p�����[�^�[
	VECTOR						moveTarget;				//�ړ��ڕW
	float						animationPlayTime;		//�A�j���[�V�����Đ�����
	int							nowAnimation;			//���݂̃A�j���[�V����
	int							nowPhase;				//���݂̃t�F�[�Y
	int							prevPhase;				//�O�̃t�F�[�Y
	int							actionType;
};

