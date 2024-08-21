//=============================================
// @brief �J�����}�l�[�W���[�N���X
//=============================================
#pragma once
#include "Singleton.h"

class Camera;
class CameraManager : public Singleton<CameraManager>
{
public:
	friend class Singleton<CameraManager>;

	void Initialize();		//������
	void Update	   ();		//�X�V
	const void Draw()const;	//�`��

	/*getter*/
	const VECTOR GetNowCameraPosition()const;//���݂̃J�����̍��W
	const VECTOR GetNowCameraTarget()const;//���݂̃J�����̒����_
	const VECTOR GetCameraDirection()const;//���݂̃J�����̒����_
private:
	/*���������֐�*/
	~CameraManager();//�f�X�g���N�^
	 CameraManager();//�R���X�g���N�^

	/*�����o�ϐ�*/
	Camera* camera;
};

