//=============================================================
// @brief �v���C���[���U���A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class AttackSphereColliderData;
class PlayerStrongAttack : public PlayerAction
{
public:
	 PlayerStrongAttack();	//�R���X�g���N�^
	~PlayerStrongAttack();	//�f�X�g���N�^
	void Initialize	() override;				//������
	void Finalize	() override;				//�㏈��
	void Update		(Player& _player) override;	//�X�V
private:
	AttackSphereColliderData* collider;			 //�R���C�_�[
	float					  firstPlayTime;	 //�ŏ��̃A�j���[�V�����Đ�����	
	int						  damage;			 //�_���[�W
	short					  cancelableFrame;	 //�L�����Z���\�t���[��
	int						  startHitCheckFrame;//�����蔻��J�n�t���[��
	int						  endHitCheckFrame;	 //�����蔻��I���t���[��
	float					  positionOffset;	 //���W�I�t�Z�b�g
	float					  yOffset;			 //Y�I�t�Z�b�g
	VECTOR					  firstDirection;	 //�ŏ��̌���
};

