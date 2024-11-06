//==========================================
// �J�����N���X
//==========================================
#pragma once

class ColliderData;
class Rigidbody;
class Camera
{
public:
	 Camera();//�R���X�g���N�^
	~Camera();//�f�X�g���N�^

		  void Initialize();		//������
		  void Update	 ();		//�X�V
	const void Draw		 ()const;	//�`��

	/*getter*/
	const VECTOR GetNowPosition	()const;						//���݂̍��W�i�g�p���Ă��Ȃ��j
	const VECTOR GetNowTarget	() { return this->nowTarget; }	//���݂̒����_�i�g�p���Ă��Ȃ��j
	const VECTOR GetDirection	() { return this->direction; }	//���݂̒����_�i�v���C���[��TPS�ő��삷��Ƃ��̂ݕK�v�j
private:
	/*���������֐�*/
	void   UpdateDirection	();																//�����̍X�V
	void   UpdateAngle		();																//���W�̍X�V
	void   UpdateVelocity	();																//�ړ��x�N�g��
	void   UpdateTarget		();																//�����_�̍X�V
	void   UpdateLength		();																//���W�̏C��
	void   UpdateFoV		();																//Fov�̍X�V
	float  Lerp				(const float _start, const float _end, const float _percent);	//��[�Պ֐�
	VECTOR Lerp				(const VECTOR _start, const VECTOR _end, const VECTOR _percent);//��[�Պ֐�
	float  DegToRad			(const float _deg) { return _deg * (DX_PI_F / 180.0f); }		//�x�������W�A���ɕϊ�

	/*�����o�ϐ�*/
	ColliderData*	collider;		//�R���C�_�[
	VECTOR			nextTarget;		//���̒����_
	VECTOR			nowTarget;		//���̒����_
	VECTOR			direction;		//�J�������璍���_�̕���
	float			fov;			//field of view
	float			length;			//����
	float			yow;			//���[
	float			pitch;			//�s�b�`
	//HACK:�����_����length�������������ʒu�ɃJ�����̍��W��ݒ肷�邽�߁A����direction�𔽓]�������l��ێ�����
};
