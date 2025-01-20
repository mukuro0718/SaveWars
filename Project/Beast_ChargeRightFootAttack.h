//=====================================================
// @brief �E�����ߍU���A�N�V����
//=====================================================
#pragma once

class ActionNode;
class AttackCapsuleColliderData;
class Beast_ChargeRightFootAttack : public ActionNode
{
public:
	Beast_ChargeRightFootAttack();//�R���X�g���N�^
	~Beast_ChargeRightFootAttack();//�f�X�g���N�^

	void		Initialize	();			//������
	NodeState	Update		()override;	//�X�V
	const void	Draw		()const;	//�`��
private:
	short						attackStartCount;				//�U���J�n�t���[��
	short						attackEndCount;					//�U���I���t���[��
	short						frameCount;						//�t���[���J�E���g
	short						frameIndexUsedCapsuleDirection1;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	short						frameIndexUsedCapsuleDirection2;//�u���X�̕��������߂邽�߂̃t���[���ԍ�
	AttackCapsuleColliderData*  collider;						//�U���R���C�_�[
};

