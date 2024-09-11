//===========================================
// @brief �v���C���[�ʏ�U���P
//===========================================
#pragma once

class ColliderData;
class PlayerAttack
{
public:
	PlayerAttack();//�R���X�g���N�^
	~PlayerAttack();//�f�X�g���N�^

	void		Initialize	();							//������
	void		Finalize	();							//�㏈��
	void		Update		(const VECTOR _position, const VECTOR _direction);	//�X�V
	const void	Draw		()const;												//�`��
	void		OnIsStart	() { this->isStartHitCheck = true; this->isNotOnHit = false;}
	const bool	GetIsStart	()const { return this->isStartHitCheck; }
private:
	ColliderData* collider;
	bool isStartHitCheck;
	bool isNotOnHit;
	int	 frameCount;
	//�����蔻��̃X�t�B�A�ƍ��W��Collidable�������߁A�����^�C�~���O���Ǘ�����
};

