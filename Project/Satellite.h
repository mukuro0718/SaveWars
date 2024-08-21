//================================================
// @brief �q���N���X
// �v���C���[�̎�����܂��
//================================================
#pragma once

class Model;
class Satellite
{
public:
	 Satellite();//�R���X�g���N�^
	~Satellite();																						 //�f�X�g���N�^

	void		Initialize(const float _angleOffset,const VECTOR _targetPosition, const VECTOR _positionOffset);	//������
	void		Update	  (const bool _isLeft, const VECTOR _targetDirection, const VECTOR _targetPosition);//�X�V
	const void	Draw	  ()const;														//�`��
private:
	Model* model;			//���f��
	VECTOR rotationAxis;	//��]��
	VECTOR positionOffset;	//���W�␳�l
	VECTOR targetDirection;
	float  angle;			//��]�l
	bool isReverse;
};

