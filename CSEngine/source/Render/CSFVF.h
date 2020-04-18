#pragma once
struct F_UI
{
	inline void set(float X, float Y, const BearColor& Color, float U, float V)
	{
		x = X;
		y = Y;
		r = Color.R32F;
		g = Color.G32F;
		b = Color.B32F;
		a = Color.A32F;
		u = U;
		v = V;
	}
	float x, y;
	float r, g, b, a;
	float u, v;
};