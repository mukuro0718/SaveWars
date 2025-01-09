//===============================================
// @brief �{�X�N���X
//===============================================
#pragma once

class Character;
class Enemy;
class DragonBehaviorTree;
class CharacterData;
class CharacterColliderData;
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

	/*getter/setter*/
	const bool GetIsAttack	()const override;			//�R�E�Q�L������
		  void OffIsDraw	(){ this->isDraw = false; }	//�`��t���O������
private:
	/*�ÓI�萔*/
	static constexpr float SHADOW_HEIGHT = 10.0f;//�e�𓊉e���鍂��
	static constexpr float SHADOW_SIZE	 = 8.0f; //�e�̃T�C�Y
};

