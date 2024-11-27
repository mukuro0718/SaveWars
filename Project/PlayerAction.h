//===================================================
// @�v���C���[�A�N�V�����x�[�X�N���X
//===================================================
#pragma once

class Player;
class PlayerAction abstract
{
public:
			 PlayerAction(bool _isChangeAction = false) :isChangeAction(_isChangeAction) {}//�R���X�g���N�^
	virtual ~PlayerAction()																 {}//�f�X�g���N�^
	virtual void Initialize	() abstract;				//������
	virtual void Finalize	() abstract;				//�㏈��
	virtual void Update		(Player& _player) abstract;	//�X�V
	const bool GetIsChangeAction()const { return this->isChangeAction; }//�A�N�V�����ύX�t���O�̎擾
protected:
	void UpdateRotation	(VECTOR& _nextRotation, VECTOR _nowRotation);
	void UpdateSpeed	(float& _nowSpeed, const float _maxSpeed, const VECTOR nowRotation, const VECTOR _nextRotation);
	VECTOR UpdateVelocity	(const VECTOR _rotation, const VECTOR _prevVelocity, const float _speed, const bool _isLerp);
	bool isChangeAction;//�A�N�V�����̕ύX���ł����ԂɂȂ�΂��̃t���O�𗧂Ă�

};

