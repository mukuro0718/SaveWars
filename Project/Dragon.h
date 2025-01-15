//===============================================
// @brief �{�X�N���X
//===============================================
#pragma once

class Character;
class Enemy;
class DragonBehaviorTree;
class CharacterData;
class CharacterColliderData;
class AttackCapsuleColliderData;
class Dragon : public Enemy
{
public:
	 Dragon();//�R���X�g���N�^
	~Dragon();//�f�X�g���N�^

	void		Initialize			() override;										//������
	void		Finalize			() override;										//�㏈��
	void		Update				() override;										//�X�V
	const void	DrawCharacterInfo	() const override;									//�`��
	void		PlayAnimation		(const float _playAnimation, const float _playTime);//�A�j���[�V�����̍Đ�
	void		OnAttackCollider	(const short _index);								//�w��̍U���R���C�_�[�̃t���O�𗧂Ă�
	void		UpdateAttackCollider(const short _index);								//�w��̍U���R���C�_�[���X�V
	void		OffAttackCollider	(const short _index);								//�w��̍U���R���C�_�[�̃t���O������

	/*�񋓑�*/
	//�A�j���[�V����
	enum class AnimationType
	{
		 DYING			= 0,//�f�X
		 IDLE			= 1,//�ҋ@
		 ROAR			= 2,//���K
		 WALK			= 3,//����
		 TURN_LEFT		= 4,//��������
		 TURN_RIGHT		= 5,//�E������
		 SMASH			= 6,//�@������
		 SWEEP			= 7,//�Ȃ�����
		 ROTATE_ATTACK	= 8,//��]�U��
		 BREATH			= 9,//�u���X
	};
	//���ʂ��Ƃ̃R���C�_�[
	enum class PartsCollider
	{
		HEAD		= 0,
		BODY		= 1,
		LEFT_HAND	= 2,
		RIGHT_HAND	= 3,
		LEFT_FOOT	= 4,
		RIGHT_FOOT	= 5,
		TAIL1		= 6,
		TAIL2		= 7,
		TAIL3		= 8,
	};
	//�U���R���C�_�[
	enum class AttackCollider
	{
		RIGHT_HAND	= 0,
		TAIL1		= 1,
		TAIL2		= 2,
		TAIL3		= 3,
		BREATH		= 4
	};
	/*getter/setter*/
	const bool GetIsAttack	()const override;			//�R�E�Q�L������
		  void OffIsDraw	(){ this->isDraw = false; }	//�`��t���O������
private:
	/*�ÓI�萔*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//�e�𓊉e���鍂��
	static constexpr float SHADOW_SIZE	 = 30.0f; //�e�̃T�C�Y

	/*�����o�ϐ�*/
	vector<AttackCapsuleColliderData*>  attackCollider;				//���ʂ��Ƃ̍U���R���C�_�[
	vector<CharacterColliderData*>		partsCollider;				//���ʂ��Ƃ̃R���C�_�[
	vector<vector<short>>				frameIndexUsePartsColider;	//���ʂ��Ƃ̃R���C�_�[�̎w��Ɏg�p����t���[���ԍ�
	vector<vector<short>>				frameIndexUseAttackColider;	//�U���̃R���C�_�[�̎w��Ɏg�p����t���[���ԍ�
	short								maxPartsColliderNum;		//�R���C�_�[�̐�
	vector<float>						prevPartsHp;				//1�t���[���O�̕��ʂ��Ƃ�HP
	float								maxHp;						//���ʂ̍ő�HP
	float								breathLength;				//�u���X�̒���

};

