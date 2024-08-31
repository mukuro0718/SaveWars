//===========================================
// @brief �v���C���[�X�L���U��
//===========================================

#pragma once

class Physics;
class Collidable;
class PlayerSpecialAttack : public GoriLib::Collidable
{
public:
	PlayerSpecialAttack();//�R���X�g���N�^
	~PlayerSpecialAttack();//�f�X�g���N�^

	void		Initialize(GoriLib::Physics* _physics);							//������
	void		Finalize(GoriLib::Physics* _physics);							//�㏈��
	void		Update(GoriLib::Physics* _physics, const VECTOR _position, const VECTOR _direction);	//�X�V
	void		OnCollide(const Collidable& _colider)override;					//�Փ˂����Ƃ�
	void		OnIsStart() { this->isStartHitCheck = true; }
	const void	Draw()const;												//�`��
private:
	static constexpr int ATTACK_NUM = 2;
	bool isStartHitCheck;
	int	 frameCount;
	bool isDontStartPrevFrame;
	int	 damage;
	float radius;
	VECTOR basePosition;
	VECTOR direction;
	//�����蔻��̃X�t�B�A�ƍ��W��Collidable�������߁A�����^�C�~���O���Ǘ�����
};

