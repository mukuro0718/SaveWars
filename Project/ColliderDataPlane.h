//==========================================
// @brief 平面(今回は円形にするので半径も持つ)のColliderData
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
		float radius;
	};
}
