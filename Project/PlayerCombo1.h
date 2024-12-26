//=============================================================
// @brief �v���C���[�R���{�P�A�N�V�����N���X
//=============================================================
#pragma once

class PlayerAction;
class AttackSphereColliderData;
class PlayerCombo1 : public PlayerAction
{
public:
	 PlayerCombo1();	//�R���X�g���N�^
	~PlayerCombo1();	//�f�X�g���N�^
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
	float					  hitStopTime;		 //�q�b�g�X�g�b�v����
	int						  hitStopDelay;		 //�q�b�g�X�g�b�v�f�B���C
	int						  hitStopType;		 //�q�b�g�X�g�b�v�̎��
	float					  slowFactor;		 //�X���[�W��
};

