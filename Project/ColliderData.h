#pragma once

namespace GoriLib
{
	class ColliderData abstract
	{
	public:
		/*�����蔻����*/
		enum class Kind
		{
			SPHERE,
			CAPSULE,
			PLANE,
			CUBE,
		};

		/*�R���X�g���N�^*/
		ColliderData(Kind _kind, bool isTrigger)
		{
			this->kind = _kind;
			this->isTrigger = isTrigger;
		}

		virtual ~ColliderData() {}

		/*�����蔻���ʎ擾*/
		Kind GetKind()const { return this->kind; }

		/*�g���K�[���ǂ���*/
		bool IsTrigger()const { return this->isTrigger; }

	private:
		Kind kind;
		bool isTrigger;
	};
}