//=========================================================
// @brief �����A�Փ˔���ɕK�v�ȃf�[�^
//=========================================================
#pragma once

namespace GoriLib
{
	class Collidable;
	class Rigidbody
	{
	public:
		Rigidbody();//�R���X�g���N�^
		void Initialize(bool _useGravity = false);//������

		/*getter/setter*/
		const	VECTOR& GetPosition	()const { return this->position; }
		const	VECTOR& GetRotation	()const { return this->rotation; }
		const	VECTOR& GetScale	()const { return this->scale; }
		const	VECTOR& GetDirection()const { return this->direction; }
		const	VECTOR& GetVelocity	()const { return this->velocity; }
				bool	UseGravity	()const { return this->useGravity; }

		void SetPosition	(const VECTOR& _set) { this->position = _set; }
		void SetRotation	(const VECTOR& _set) { this->rotation = _set; }
		void SetScale		(const VECTOR& _set) { this->scale = _set; }
		void SetVelocity	(const VECTOR& _set);
		void SetUseGravity	(bool _set)			 { this->useGravity = _set; }
	private:
		VECTOR	position;	//���W
		VECTOR  rotation;	//��]��
		VECTOR  scale;		//�g�嗦
		VECTOR	direction;	//����
		VECTOR	velocity;	//�ړ���
		bool	useGravity;	//�d�͂��g�p���邩
	};

}