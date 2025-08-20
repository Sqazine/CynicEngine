#pragma once
#include "Vector3.h"
#include "Quaternion.h"
namespace CynicEngine
{
	template <typename T>
	class Vector2;

	template <typename T>
	class Vector4;

	template <typename T>
	class Transform2
	{
	public:
		Vector2<T> position;
		T rotation
			Vector2<T>
				scale;

		Transform2();
		Transform2(const Vector3<T> &p, const Quaternion<T> &r, const Vector3<T> &s);

		static Matrix4<T> ToMatrix4(const Transform2<T> &trans);
	};

	typedef Transform2<float> Transform2f;
	typedef Transform2<double> Transform2d;
	typedef Transform2<int32_t> Transform2i32;
	typedef Transform2<uint32_t> Transform2u32;
	typedef Transform2<int16_t> Transform2i16;
	typedef Transform2<uint16_t> Transform2u16;
	typedef Transform2<int8_t> Transform2i8;
	typedef Transform2<uint8_t> Transform2u8;

	//TODO : not finished yet
}