//==========================================
// �J�����N���X
//==========================================
#pragma once


class Camera
{
public:
	 Camera();//�R���X�g���N�^
	~Camera();//�f�X�g���N�^

	void Initialize();		//������
	void Update();			//�X�V
	const void Draw()const;	//�`��

	/*getter*/
	const VECTOR GetNowPosition() { return this->nowPosition; }//���݂̍��W
	const VECTOR GetNowTarget() { return this->nowTarget; }//���݂̒����_
	const VECTOR GetDirection() { return this->direction; }//���݂̒����_
private:
	/*���������֐�*/
	void   UpdateDirection	();																//�J�����̌����̍X�V
	void   UpdateAngle		();																//��]���̍X�V
	void   UpdateLength		();																//�����̍X�V
	void   UpdateTarget		();																//�����_�̍X�V
	void   FixAngle			();																//��]�l�̏C��
	void   UpdatePosition	();																//�J�������W�̍X�V
	void   UpdateFoV		();																//Fov�̍X�V
	float  Lerp				(const float _start, const float _end, const float _percent);	//��[�Պ֐�
	VECTOR Lerp				(const VECTOR _start, const VECTOR _end, const VECTOR _percent);//��[�Պ֐�
	float  DegToRad			(const float _deg) { return _deg * (DX_PI_F / 180.0f); }		//�x�������W�A���ɕϊ�

	/*�����o�ϐ�*/
	VECTOR nextPosition;	//���̍��W
	VECTOR nowPosition;		//���̍��W
	VECTOR nextTarget;		//���̒����_
	VECTOR nowTarget;		//���̒����_
	VECTOR direction;		//�J�������璍���_�̕���
	VECTOR firstDirection;	//�J�����̏�������
	//HACK:�����_����length�������������ʒu�ɃJ�����̍��W��ݒ肷�邽�߁A����direction�𔽓]�������l��ێ�����
	float  fov;				//field of view
	float  length;			//����
	float  horizonAngle;	//������]�l
	float  verticalAngle;	//������]�l
};

