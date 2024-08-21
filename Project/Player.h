//===============================================
// @brief �v���C���[�N���X
//===============================================
#pragma once

class Model;
class BitFlag;
class Satellite;
class Shield;
class Collider;
class Player
{
public:
	Player();//�R���X�g���N�^
	~Player();//�f�X�g���N�^

	void		Initialize();		//������
	void		Action	  ();		//�A�N�V����
	void		Update	  ();		//�X�V
	const void	Draw	  ()const;	//�`��

	/*getter*/
	const VECTOR	GetPosition		  ()const;									  //���W�̎擾
	const VECTOR	GetDirection	  ()const { return this->direction; }		  //�ړ��x�N�g���̎擾
	const bool		IsMove			  ()const;									  //�ړ��t���O�̎擾
	const bool		IsAttack		  ()const;									  //�V���b�g�t���O�̎擾
	const bool		IsLightAttack	  ()const;									  //�y�U��
	const bool		IsHeavyAttack	  ()const;									  //�d�U��
	const void		FixMoveVector	  (const VECTOR _fixVector);				  //�ړ��x�N�g���̏C��
	const Collider	GetCollider		  ();										  //�R���C�_�[�̎擾
	const int		GetHP			  ()const { return this->hp; }				  //HP�̎擾
	const int		GetCurrentElement ()const { return this->element; }			  //���݂̑����̎擾
	const bool		IsShowElementWheel()const { return this->isShowElementWheel; }//�����z�C�[����\�����邩
	const float GetElementAngle		  ()const { return this->elementAngle; }
private:
	/*�ÓI�萔*/
	static constexpr int SATELLITE_NUM	 = 2;//�q����
	static constexpr unsigned int IDLE			 = (1 << 0); //�ҋ@
	static constexpr unsigned int RUN			 = (1 << 1); //����
	static constexpr unsigned int WALK			 = (1 << 2); //����
	static constexpr unsigned int AVOID			 = (1 << 3); //���
	static constexpr unsigned int JUMP			 = (1 << 4); //�K�[�h
	static constexpr unsigned int FLAME_ATTACK	 = (1 << 5); //�d�U��
	static constexpr unsigned int BOLT_ATTACK	 = (1 << 6); //�d�U��
	static constexpr unsigned int ICE_ATTACK	 = (1 << 7); //�d�U��
	static constexpr unsigned int EARTH_ATTACK	 = (1 << 8); //�d�U��
	static constexpr unsigned int STORM_ATTACK	 = (1 << 9); //�d�U��
	static constexpr unsigned int MASK_ATTACK	 = FLAME_ATTACK | BOLT_ATTACK | ICE_ATTACK | EARTH_ATTACK | STORM_ATTACK;
	static constexpr unsigned int MASK_MOVE		 = RUN | WALK;
	static constexpr unsigned int MASK_ALL		 = IDLE | MASK_MOVE | MASK_ATTACK | AVOID | JUMP;
	/*�񋓑�*/
	//�t���[���J�E���g�̎��
	enum class FrameCountType
	{
		ATTACK_INTERVAL = 0,//�U���ҋ@
	};
	//�A�j���[�V�����̎��
	enum class AnimationType
	{
		IDLE		 = 0,
		WALK		 = 1,
		RUN			 = 2,
		AVOID		 = 3,
		JUMP		 = 4,
		FLAME_ATTACK = 5,
		BOLT_ATTACK	 = 6,
		ICE_ATTACK	 = 7,
		EARTH_ATTACK = 8,
		STORM_ATTACK = 9,
	};
	enum class Element
	{
		FRAME,
		BOLT,
		ICE,
		EARTH,
		STORM,
	};
	/*���������֐�*/
		  void UpdateVelocity	 ();		//���x�̍X�V
		  void UpdateMoveVector	 ();		//�ړ��x�N�g���̍X�V
		  void UpdateRotation	 ();		//��]���̍X�V
		  void Attack			 ();		//�U��
		  void Move				 ();		//�ړ�
		  void Jump				 ();		//�W�����v
		  void UpdateAnimation	 ();		//���݂̃A�j���[�V�����̍X�V
		  void UpdateElement	 ();
	const bool CanMove			 ()const;//�ړ��ł��邩
	const bool CanAttack		 ()const;//�U���ł��邩
	const bool CanJump			 ()const;//�W�����v�ł��邩


	/*�����o�ϐ�*/

	Model*				model;						//���f��
	BitFlag*			state;						//�A�N�V�����̏��
	Satellite*			satellite[SATELLITE_NUM];	//�q��
	Shield*				shield;						//�V�[���h
	Collider*			collider;					//�R���C�_�[�N���X
	VECTOR				moveVector;					//�ړ��x�N�g��
	VECTOR				direction;					//�����Ă������
	VECTOR				fixVector;					//�␳�x�N�g��
	VECTOR				moveVectorRotation;			//�ړ��x�N�g���p��]�l
	VECTOR				wasd;						//wasd����
	VECTOR				lStick;						//lStick����
	float				velocity;					//���x
	std::vector<int>	frameCount;					//�t���[���J�E���g
	std::vector<bool>	isCount;					//�J�E���g�����邩
	std::map<unsigned int, int> animationMap;		//�A�j���[�V����
	std::map<int, unsigned int> attackTypeMap;
	bool				isShot;						//���@����������
	float				jumpPower;					//�W�����v��
	int					nowAnimation;				//�A�j���[�V����
	float				animationPlayTime;			//�A�j���[�V�����Đ�����
	int					hp;
	int					element;
	bool				isShowElementWheel;//�����z�C�[���̕\��
	std::vector<int> rStick;
	float elementAngle;
};

