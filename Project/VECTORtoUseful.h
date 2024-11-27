//=============================================
// @brief VECTOR To Useful
// �\����VECTOR���g���₷���悤�ɁA�@�\��ǉ�����
//=============================================
#pragma once

namespace Gori
{
	static constexpr VECTOR ORIGIN = { 0.0f,0.0f,0.0f };
	static constexpr VECTOR UP_VEC = { 0.0f,1.0f,0.0f };
	static constexpr float PI_2 = (2.0f * DX_PI_F);
	inline const VECTOR Convert(const std::vector<float> _value);
	inline const float LerpFloat(const float _start, const float _end, const float _percent)
	{
		return _start + _percent * (_end - _start);
	}
	inline const VECTOR LerpVECTOR(const VECTOR _start, const VECTOR _end, const VECTOR _percent)
	{
		VECTOR out = ORIGIN;
		out.x = LerpFloat(_start.x, _end.x, _percent.x);
		out.y = LerpFloat(_start.y, _end.y, _percent.y);
		out.z = LerpFloat(_start.z, _end.z, _percent.z);
		return out;
	}
	const float LerpAngle(const float _start, const float _end, const float _percent)
	{
		float start = _start;
		float end = _end;
		float out = 0.0f;
		float start_base = _start;
		float end_base = _end;

		//�I���n�_���}�C�i�X��������2�΂𑫂�
		if (end < 0.0f) end += PI_2;

		/*�J�n�n�_�ƏI���n�_�̍������߂�*/
		float diff = start - end;
		//���������}�C�i�X��������-1��������
		if (diff < 0.0f) diff *= -1.0f;

		/*�]������߂�*/
		float excess = PI_2 - diff;

		/*���Ɨ]������߂āA���̂ق����傫�����Base�̂ق��Ōv�Z*/
		if (diff > excess)
		{
			//�X�^�[�g�x�[�X��360�x�ɕϊ�����Ă���̂ŁA�� ~ -�΂͈̔͂ɖ߂�
			if (_start >= DX_PI_F)
			{
				start_base -= PI_2;
			}
			out = LerpFloat(start_base, end_base, _percent);
		}

		/*�]��̂ق����傫����΂��̂܂܌v�Z*/
		else
		{
			out = LerpFloat(start, end, _percent);
		}

		/*360�x�͈̔͂𒴂��Ȃ��悤�ɂ���*/
		if (out <= 0.0f)
		{
			out += PI_2;
		}
		else if (out >= PI_2)
		{
			out -= PI_2;
		}
		return out;
	}
	const VECTOR LerpAngle(const VECTOR _start, const VECTOR _end, const VECTOR _percent)
	{
		VECTOR out;

		out.x = LerpAngle(_start.x, _end.x, _percent.x);
		out.y = LerpAngle(_start.y, _end.y, _percent.y);
		out.z = LerpAngle(_start.z, _end.z, _percent.z);

		VECTOR rotation = VGet(0.0f, out.y, 0.0f);
		return rotation;
	}
}
VECTOR operator +(const VECTOR _a, const VECTOR _b);
VECTOR operator -(const VECTOR _a, const VECTOR _b);
VECTOR operator *(const VECTOR _a, const float _b);
