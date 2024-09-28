//=============================================
// @brief VECTOR To Useful
// �\����VECTOR���g���₷���悤�ɁA�@�\��ǉ�����
//=============================================
#pragma once

namespace Gori
{
	static constexpr VECTOR ORIGIN = { 0.0f,0.0f,0.0f };
	static constexpr VECTOR UP_VEC = { 0.0f,1.0f,0.0f };
}
const VECTOR Convert(const std::vector<float> _value);
VECTOR operator +(const VECTOR _a, const VECTOR _b);
VECTOR operator -(const VECTOR _a, const VECTOR _b);
VECTOR operator *(const VECTOR _a, const float _b);
