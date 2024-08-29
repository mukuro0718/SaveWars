#pragma once
#include <math.h>

namespace GORI
{
	VECTOR GetClosestPointOnSegment(VECTOR _point, VECTOR _start, VECTOR _end)
	{
		/*�ŋߐړ_��start�������O�̈�̏ꍇ*/
		VECTOR startToPoint = VSub(_point, _start);
		VECTOR startToEnd = VSub(_end, _start);
		VECTOR startToEndNorm = VNorm(startToEnd);
		//���ς̌��ʂ��}�C�i�X�̒l��������start���ŋߐړ_�ɂȂ�
		if (VDot(startToPoint, startToEndNorm) < 0)
		{
			return _start;
		}

		/*�ŋߐړ_��start�������O�̈�̏ꍇ*/
		VECTOR endToPoint = VSub(_point, _end);
		VECTOR endToStart = VSub(_start, _end);
		VECTOR endToStartNorm = VNorm(endToStart);
		//���ς̌��ʂ��}�C�i�X�̒l��������end���ŋߐړ_�ɂȂ�
		if (VDot(endToPoint, endToStartNorm) < 0)
		{
			return _start;
		}
		//���ԗ̈�̏ꍇ
		else
		{
			float t = VDot(startToEndNorm, startToPoint);
			return VAdd(_start, VScale(startToEndNorm, t));
		}
	}
}