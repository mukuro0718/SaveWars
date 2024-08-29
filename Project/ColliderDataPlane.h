//==========================================
// @brief 平面のColliderData
//==========================================
#pragma once

namespace GoriLib
{
	class ColliderData;
	class ColliderDataPlane : public ColliderData
	{
	public:
		ColliderDataPlane(bool _isTrigger);

		VECTOR norm;//法線
	};
}
