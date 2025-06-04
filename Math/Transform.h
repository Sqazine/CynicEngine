#pragma once
#include "Vector3.h"
#include "Quaternion.h"

template <typename T>
class Vector3;

template <typename T>
class Vector4;

template <typename T>
class Quaternion;

template <typename T>
class DualQuaternion;

template <typename T>
class Transform
{
public:
	Vector3<T> position;
	Quaternion<T> rotation;
	Vector3<T> scale;

	Transform();
	Transform(const Vector3<T> &p, const Quaternion<T> &r, const Vector3<T> &s);

	static Transform<T> Combine(const Transform<T> &a, const Transform<T> &b);
	static Transform<T> Inverse(const Transform<T> &t);
	static Transform<T> Interpolate(const Transform<T> &a, const Transform<T> &b, float t);

	static Matrix4<T> ToMatrix4(const Transform<T> &trans);

	static Vector3<T> TransformPoint(const Transform<T> &a, const Vector3<T> &b);
	static Vector3<T> TransformVector(const Transform<T> &a, const Vector3<T> &b);

	static Vector3<T> TransformPoint(const DualQuaternion<T> &dq, const Vector3<T> &b);
	static Vector3<T> TransformVector(const DualQuaternion<T> &dq, const Vector3<T> &b);

	static DualQuaternion<T> TransformToDualQuaternion(const Transform<T> &t);
};

typedef Transform<float> Transformf;
typedef Transform<double> Transformd;
typedef Transform<int32_t> Transformi32;
typedef Transform<uint32_t> Transformu32;
typedef Transform<int16_t> Transformi16;
typedef Transform<uint16_t> Transformu16;
typedef Transform<int8_t> Transformi8;
typedef Transform<uint8_t> Transformu8;

template <typename T>
inline Transform<T>::Transform()
	: position(Vector3<T>::ZERO), rotation(Quaternion<T>::ZERO), scale(Vector3<T>(static_cast<T>(1.0f)))
{
}

template <typename T>
inline Transform<T>::Transform(const Vector3<T> &p, const Quaternion<T> &r, const Vector3<T> &s)
	: position(p), rotation(r), scale(s)
{
}
template <typename T>
inline Transform<T> Transform<T>::Combine(const Transform<T> &a, const Transform<T> &b)
{
	Transform<T> tmp;
	tmp.scale = a.scale * b.scale;
	tmp.rotation = Quaternionf::Concatenate(a.rotation, b.rotation);
	tmp.position = a.rotation * (a.scale * b.position);
	tmp.position = a.position + tmp.position;
	return tmp;
}
template <typename T>
inline Transform<T> Transform<T>::Inverse(const Transform<T> &t)
{
	Transform<T> tmp;
	tmp.rotation = Quaternion<T>::Inverse(t.rotation);
	tmp.scale.x = Math::Abs(t.scale.x) < 0.000001f ? 0.0f : 1.0f / t.scale.x;
	tmp.scale.y = Math::Abs(t.scale.y) < 0.000001f ? 0.0f : 1.0f / t.scale.y;
	tmp.scale.z = Math::Abs(t.scale.z) < 0.000001f ? 0.0f : 1.0f / t.scale.z;
	tmp.position = -(tmp.rotation * (tmp.scale * t.position));
	return tmp;
}

template <typename T>
inline Transform<T> Transform<T>::Interpolate(const Transform<T> &a, const Transform<T> &b, float t)
{
	Quaternion<T> bRot = b.rotation;
	if (Quaternion<T>::Dot(a.rotation, bRot) < 0.0f)
		bRot = -bRot;

	return Transform<T>(Vector3<T>::Lerp(a.position, b.position, t), Quaternion<T>::NLerp(a.rotation, bRot, t), Vector3<T>::Lerp(a.scale, b.scale, t));
}

template <typename T>
inline Matrix4<T> Transform<T>::ToMatrix4(const Transform<T> &trans)
{
	Matrix4<T> world = Matrix4<T>::Translate(trans.position);
	world *= Quaternion<T>::ToMatrix4(trans.rotation);
	world *= Matrix4<T>::Scale(trans.scale);
	return world;
}

template <typename T>
inline Vector3<T> Transform<T>::TransformPoint(const Transform<T> &a, const Vector3<T> &b)
{
	Vector3<T> out;
	out = a.rotation * (a.scale * b);
	out = a.position + out;
	return out;
}

template <typename T>
inline Vector3<T> Transform<T>::TransformVector(const Transform<T> &a, const Vector3<T> &b)
{
	Vector3<T> out;
	out = a.rotation * (a.scale * b);
	return out;
}

template <typename T>
inline Vector3<T> TransformPoint(const DualQuaternion<T> &dq, const Vector3<T> &b)
{
	Quaternion<T> d = Quaternion<T>::Conjuate(dq.real) * (dq.dual * 2.0f);
	Vector3<T> t = d.vec;
	return dq.real * b + t;
}

template <typename T>
inline Vector3<T> TransformVector(const DualQuaternion<T> &dq, const Vector3<T> &b)
{
	return dq.real * b;
}

template <typename T>
inline DualQuaternion<T> Transform<T>::TransformToDualQuaternion(const Transform<T> &t)
{
	Quaternion<T> d;
	d.vec = t.position;
	d.scalar = 0.0f;

	Quaternion<T> qr = t.rotation;

	Quaternion<T> qd = qr * d * 0.5f;

	return DualQuaternion<T>(qr, qd);
}