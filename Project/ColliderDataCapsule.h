//==========================================
// @brief �J�v�Z����ColliderData
//==========================================
#pragma once

namespace GoriLib
{
	class ColliderData;
	class ColliderDataCapsule : public ColliderData
	{
	public:
		ColliderDataCapsule(bool _isTrigger);

		float radius;
		float height;
	};
}
