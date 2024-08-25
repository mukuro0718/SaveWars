#include <DxLib.h>
#include "UseSTL.h"
#include "Transform.h"
#include "DeleteInstance.h"
#include "Animation.h"
#include "Model.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
Model::Model(const int _modelHandle)
	: modelHandle(-1)
	, transform(nullptr)
{
	this->transform = new Transform();
	this->animation = new Animation();
	this->modelHandle = MV1DuplicateModel(_modelHandle);
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
Model::~Model()
{
	MV1DeleteModel(this->modelHandle);
	DeleteMemberInstance(this->transform);
	DeleteMemberInstance(this->animation);
}

/// <summary>
/// ���W�̎擾
/// </summary>
const VECTOR Model::GetPosition()const
{
	return this->transform->GetPosition();
}

/// <summary>
/// ��]���̎擾
/// </summary>
const VECTOR Model::GetRotation()const
{
	return this->transform->GetRotation();
}

/// <summary>
/// �g�嗦�̎擾
/// </summary>
const VECTOR Model::GetScale()const
{
	return this->transform->GetScale();
}

/// <summary>
/// ���W�̐ݒ�
/// </summary>
void Model::SetPosition(const VECTOR _position)
{
	this->transform->SetPosition(_position);
	MV1SetPosition(this->modelHandle, this->transform->GetPosition());
}

/// <summary>
/// ��]���̐ݒ�
/// </summary>
void Model::SetRotation(const VECTOR _rotation)
{
	this->transform->SetRotation(_rotation);
	MV1SetRotationXYZ(this->modelHandle, this->transform->GetRotation());
}
void Model::SetRotationOfRadian(const VECTOR _rotation)
{
	this->transform->SetRotation(_rotation);
	MV1SetRotationXYZ(this->modelHandle, this->transform->GetRotation());
}

/// <summary>
/// �g�嗦�̐ݒ�
/// </summary>
void Model::SetScale(const VECTOR _scale)
{
	this->transform->SetScale(_scale);
	MV1SetScale(this->modelHandle, this->transform->GetScale());
}

/// <summary>
/// �g�����X�t�H�[���̐ݒ�
/// </summary>
void Model::SetTransform(const VECTOR _position, const VECTOR _rotation, const VECTOR _scale)
{
	this->transform->SetPosition(_position);
	this->transform->SetRotation(_rotation);
	this->transform->SetScale	(_scale);

	MV1SetPosition	 (this->modelHandle, this->transform->GetPosition());
	MV1SetRotationXYZ(this->modelHandle, this->transform->GetRotation());
	MV1SetScale		 (this->modelHandle, this->transform->GetScale());

}

/// <summary>
/// �`��
/// </summary>
const void Model::Draw()const
{
	MV1DrawModel(this->modelHandle);
}

/// <summary>
/// �A�j���[�V�����̒ǉ�
/// </summary>
void Model::AddAnimation(const std::vector<string> _animationHandle, const std::vector<int> _animationIndex)
{
	/*�A�j���[�V�����̒ǉ�*/
	for (int i = 0; i < _animationIndex.size(); i++)
	{
		int animationHandle = MV1LoadModel(_animationHandle[i].c_str());
		this->animation->Add(animationHandle, _animationIndex[i]);
	}

	/*�A�j���[�V�����̃A�^�b�`*/
	this->animation->Attach(&this->modelHandle);
}

/// <summary>
/// �A�j���[�V�����̍Đ�
/// </summary>
void Model::PlayAnimation(const int _nowAnimation, const float _animationPlayTime)
{
	VECTOR position = this->transform->GetPosition();
	this->animation->Play(&this->modelHandle, position,_nowAnimation, _animationPlayTime);
	this->transform->SetPosition(position);
}

/// <summary>
/// �A�j���[�V�����ύX���t���O�̎擾
/// </summary>
const bool Model::GetIsChangeAnim() const
{
	return this->animation->GetIsChangeAnim();
}