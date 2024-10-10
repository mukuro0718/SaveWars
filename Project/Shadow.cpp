#include <DxLib.h>
#include <math.h>
#include "UseSTL.h"
#include "Shadow.h"
#include "LoadingAsset.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Shadow::Shadow()
	: imageHandle(0)
{
	auto& asset = LoadingAsset::GetInstance();
	this->imageHandle = asset.GetImage(LoadingAsset::ImageType::SHADOW);
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
Shadow::~Shadow()
{
}
/// <summary>
/// �ŏI����
/// </summary>
void Shadow::Final()
{
}
/// <summary>
/// �`��
/// </summary>
void Shadow::Draw(const int _stageModelHandle, const VECTOR _targetPos, const float _shadowHeight, const float _shadowSize)
{
	MV1_COLL_RESULT_POLY_DIM groundPoly;//�v���C���[�����̒n�ʃ|���S��
	MV1_COLL_RESULT_POLY* spherePolyNum;//�������̃|���S���̐�
	VERTEX3D Vertex[3];//���_�f�[�^
	VECTOR pushBackVector;//�����߂���
	int stageModelHandle = _stageModelHandle;//�X�e�[�W���f���n���h��
	VECTOR targetPos = _targetPos;//�e���������ڕW�̍��W
	float shadowHeight = _shadowHeight;//�e�̍���
	float shadowSize = _shadowSize;//�e�̃T�C�Y

	// ���C�e�B���O�𖳌��ɂ���
	SetUseLighting(FALSE);

	// �y�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(TRUE);

	// �e�N�X�`���A�h���X���[�h�� CLAMP �ɂ���( �e�N�X�`���̒[����͒[�̃h�b�g�����X���� )
	SetTextureAddressMode(DX_TEXADDRESS_CLAMP);

	// �v���C���[�̒����ɑ��݂���n�ʂ̃|���S�����擾
	groundPoly = MV1CollCheck_Capsule(_stageModelHandle, -1, _targetPos, VAdd(_targetPos, VGet(0.0f, -shadowHeight, 0.0f)), shadowSize);

	// ���_�f�[�^�ŕω��������������Z�b�g
	Vertex[0].dif = GetColorU8(255, 255, 255, 255);
	Vertex[0].spc = GetColorU8(0, 0, 0, 0);
	Vertex[0].su = 0.0f;
	Vertex[0].sv = 0.0f;
	Vertex[1] = Vertex[0];
	Vertex[2] = Vertex[0];

	// ���̒����ɑ��݂���|���S���̐������J��Ԃ�
	spherePolyNum = groundPoly.Dim;
	for (int i = 0; i < groundPoly.HitNum; i++, spherePolyNum++)
	{
		// �|���S���̍��W�͒n�ʃ|���S���̍��W
		Vertex[0].pos = spherePolyNum->Position[0];
		Vertex[1].pos = spherePolyNum->Position[1];
		Vertex[2].pos = spherePolyNum->Position[2];

		// ������Ǝ����グ�ďd�Ȃ�Ȃ��悤�ɂ���
		pushBackVector = VScale(spherePolyNum->Normal, 0.5f);
		Vertex[0].pos = VAdd(Vertex[0].pos, pushBackVector);
		Vertex[1].pos = VAdd(Vertex[1].pos, pushBackVector);
		Vertex[2].pos = VAdd(Vertex[2].pos, pushBackVector);

		// �|���S���̕s�����x��ݒ肷��
		Vertex[0].dif.a = 0;
		Vertex[1].dif.a = 0;
		Vertex[2].dif.a = 0;
		if (spherePolyNum->Position[0].y > targetPos.y - shadowHeight)
			Vertex[0].dif.a = 128 * (1.0f - fabs(spherePolyNum->Position[0].y - targetPos.y) / shadowHeight);

		if (spherePolyNum->Position[1].y > targetPos.y - shadowHeight)
			Vertex[1].dif.a = 128 * (1.0f - fabs(spherePolyNum->Position[1].y - targetPos.y) / shadowHeight);

		if (spherePolyNum->Position[2].y > targetPos.y - shadowHeight)
			Vertex[2].dif.a = 128 * (1.0f - fabs(spherePolyNum->Position[2].y - targetPos.y) / shadowHeight);

		// �t�u�l�͒n�ʃ|���S���ƃv���C���[�̑��΍��W���犄��o��
		Vertex[0].u = (spherePolyNum->Position[0].x - targetPos.x) / (shadowSize * 2.0f) + 0.5f;
		Vertex[0].v = (spherePolyNum->Position[0].z - targetPos.z) / (shadowSize * 2.0f) + 0.5f;
		Vertex[1].u = (spherePolyNum->Position[1].x - targetPos.x) / (shadowSize * 2.0f) + 0.5f;
		Vertex[1].v = (spherePolyNum->Position[1].z - targetPos.z) / (shadowSize * 2.0f) + 0.5f;
		Vertex[2].u = (spherePolyNum->Position[2].x - targetPos.x) / (shadowSize * 2.0f) + 0.5f;
		Vertex[2].v = (spherePolyNum->Position[2].z - targetPos.z) / (shadowSize * 2.0f) + 0.5f;

		// �e�|���S����`��
		DrawPolygon3D(Vertex, 1, this->imageHandle, TRUE);
	}

	// ���o�����n�ʃ|���S�����̌�n��
	MV1CollResultPolyDimTerminate(groundPoly);

	// ���C�e�B���O��L���ɂ���
	SetUseLighting(TRUE);

	// �y�o�b�t�@�𖳌��ɂ���
	SetUseZBuffer3D(FALSE);

	//const int VERTEX_NUM = 4;
	//const unsigned short VERTEX_INDEX[6] = { 0,1,2,0,2,3 };
	//VERTEX3D vertex[VERTEX_NUM];//���_�f�[�^
	//VECTOR targetPos = _targetPos;//�e���������ڕW�̍��W
	//float stageHeight = _stageHeight;//�e�̍���
	//float shadowSize = _shadowSize;//�e�̃T�C�Y


	//// �|���S���̍��W�͒n�ʃ|���S���̍��W
	//vertex[0].pos = VGet(-50.0f - shadowSize, stageHeight, 0.0f + shadowSize);
	//vertex[1].pos = VGet(-50.0f + shadowSize, stageHeight, 0.0f + shadowSize);
	//vertex[2].pos = VGet(-50.0f + shadowSize, stageHeight, 0.0f - shadowSize);
	//vertex[3].pos = VGet(-50.0f - shadowSize, stageHeight, 0.0f - shadowSize);
	//for (int i = 0; i < 4; i++)
	//{
	//	vertex[i].norm = VGet(0.0f, 1.0f, 0.0f);
	//	vertex[i].dif = GetColorU8(255, 255, 255, 255);
	//	vertex[i].spc = GetColorU8(255, 255, 255, 255);
	//	vertex[i].su = 0.0f;
	//	vertex[i].sv = 0.0f;
	//}
	//vertex[0].u = 0.0f;
	//vertex[0].v = 0.0f;
	//vertex[1].u = 1.0f;
	//vertex[1].v = 0.0f;
	//vertex[2].u = 1.0f;
	//vertex[2].v = 1.0f;
	//vertex[3].u = 0.0f;
	//vertex[3].v = 1.0f;
	//// �e�|���S����`��
	//DrawPolygonIndexed3D(vertex, VERTEX_NUM, VERTEX_INDEX, 2, this->imageHandle, TRUE);
}