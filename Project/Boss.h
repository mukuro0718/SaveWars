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

	void		Initialize			() override;		//������
	void		Finalize			() override;		//�㏈��
	void		Update				() override;		//�X�V
	const void	DrawCharacterInfo	() const override;	//�`��
	void		PlayAnimation		();
	void		SetAttackComboCount	();
	void		DecAttackComboCount	() { this->attackComboCount--; }

	/*getter/setter*/
	const int	 GetAttackComboCount	()const { return this->attackComboCount; }	//�c��U���R���{�񐔂̎擾
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

		 SLASH_1		= 6, //�X���b�V��
		 SLASH_2		= 7, //�X���b�V��
		 STAB			= 8, //�˂��h���U��
		 ROTATE_SLASH	= 9, //��]�X���b�V��
		 PUNCH			= 10,//�p���`
		 SLASH_COMBO_1	= 11,//�X���b�V���R���{�P
		 SLASH_COMBO_2	= 12,//�X���b�V���R���{�Q
		 JUMP_ATTACK	= 13,//�W�����v�A�^�b�N
	};
	//�t�F�[�Y
	enum class Phase
	{
		PHASE_1,
		PHASE_2,
		PHASE_3,
		PHASE_4,
		PHASE_5,
		PHASE_6,
		PHASE_7,
		PHASE_8,
		PHASE_9,
		PHASE_10,
	};
	//�U���̎��
	enum class AttackType
	{
		NONE		  = -1,
		SLASH_1		  = 0, //�X���b�V��
		SLASH_2		  = 1, //�X���b�V��
		STAB		  = 2, //�˂��h���U��
		ROTATE_SLASH  = 3, //��]�X���b�V��
		PUNCH		  = 4,//�p���`
		SLASH_COMBO_1 = 5,//�X���b�V���R���{�P
		SLASH_COMBO_2 = 6,//�X���b�V���R���{�Q
		JUMP_ATTACK	  = 7,//�W�����v�A�^�b�N

	};
	void SetAttackType(const AttackType _type) { this->prevAttack = _type; }
	const AttackType GetPrevAttackType()const { return this->prevAttack; }
	enum class AngryStateType
	{
		NORMAL,
		TIRED,
		ANGRY,
	};
private:
	/*�ÓI�萔*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//�e�𓊉e���鍂��
	static constexpr float SHADOW_SIZE	 = 8.0f; //�e�̃T�C�Y
	static constexpr int   COUNT_NUM	 = 6;	 //�t���[���J�E���g�̐�
	//��{���
	static constexpr unsigned int DYING	= (1 << 0);//�f�X
	static constexpr unsigned int IDLE	= (1 << 1);//�ҋ@
	static constexpr unsigned int ROAR	= (1 << 2);//���K
	static constexpr unsigned int WALK	= (1 << 3);//����
	static constexpr unsigned int REST	= (1 << 4);//�x�e
	//�U��
	static constexpr unsigned int SLASH_1		= (1 << 5);//�X���b�V���P
	static constexpr unsigned int SLASH_2		= (1 << 6);//�X���b�V���Q
	static constexpr unsigned int STAB			= (1 << 7);//�˂��h��
	static constexpr unsigned int ROTATE_SLASH	= (1 << 8);//��]�X���b�V��
	static constexpr unsigned int PUNCH			= (1 << 9);//�p���`
	static constexpr unsigned int SLASH_COMBO_1	= (1 << 10);//�X���b�V���R���{�P
	static constexpr unsigned int SLASH_COMBO_2	= (1 << 11);//�X���b�V���R���{�Q
	static constexpr unsigned int JUMP_ATTACK	= (1 << 12);//�W�����v�A�^�b�N

	static constexpr unsigned int MASK_ATTACK = SLASH_1 | SLASH_2 | ROTATE_SLASH |
												STAB | PUNCH | SLASH_COMBO_1 | SLASH_COMBO_2 | JUMP_ATTACK;
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
		SLASH_1			= 7, //�X���b�V��
		SLASH_2			= 8, //�X���b�V��
		STAB			= 9, //�˂��h���U��
		ROTATE_SLASH	= 10, //��]�X���b�V��
		PUNCH			= 11,//�p���`
		SLASH_COMBO_1	= 12,//�X���b�V���R���{�P
		SLASH_COMBO_2	= 13,//�X���b�V���R���{�Q
		JUMP_ATTACK		= 14,//�W�����v�A�^�b�N
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
	int							nowAction;				//���݂̃A�N�V����
	float						angryValue;				//�{��Q�[�W
	int							angryState;				//�{����
	int							tiredInterval;			//��J�C���^�[�o��
	int							attackComboCount;		//�U���R���{��
	AttackType					prevAttack;				//�O�̍U��
};

