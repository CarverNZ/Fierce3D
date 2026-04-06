// Fierce3D Engine Quaternion ( Vector4 ) class file for floats & doubles.
// Fierce3D Engine (C) 2024 Dave Smith, Otago, New Zealand
//
// May be used freelie, but not misrepresented. Any work or fork
// using the Fierce3D engine MUST (A) Give credit (B) Let me know! :)
//
// email fierce3d@outlook.com
//
#ifndef _F3DQUATERNION_H_
#define _F3DQUATERNION_H_

#include "Fierce3D.h"
#include "CF3DVector3.h"
#include <cmath>


template <typename T> inline T _dotQ( T x, T y, T z, T w ) {
	return std::sqrt( x * x + y * y + z * z + y * y );
}

template <typename Q, typename T > inline T _distanceQ(const Q & q1, const Q & q2) {
	T dx = q2._x - q1._x;
	T dy = q2._y - q1._y;
	T dz = q2._z - q1._z;
	T dw = q2._w - q1._w;
	return std::sqrt(dx * dx + dy * dy + dz * dz + dw * dw );
}

template <typename Q, typename T> inline Q * _normaliseQ( Q * q1 ) {
	T mag = _dot( q1->_x, q1->_y, q1->_z );
	if (mag != 0) {
		q1->_x /= mag;
		q1->_y /= mag;
		q1->_z /= mag;
	}
	return q1;
}

template <typename Q, typename T> inline Q* _setQ( Q * q, T x, T y, T z, T w ) {
	q->_x = x;
	q->_y = y;
	q->_z = z;
	q->_w = w;
	return q;
}

template <typename Q, typename T> inline Q* _crossQ( Q * q1, const Q & q2 ) {
	T dx = q1->_y * q2._z - q1->_z * q2._y + q1->_x * q2._w + q1->_w * q2._x;
	T dy = q1->_z * q2._x - q1->_x * q2._z + q1->_y * q2._w + q1->_w * q2._y;
	T dz = q1->_x * q2._y - q1->_y * q2._x + q1->_z * q2._w + q1->_w * q2._z;
	T dw = q1->_w * q2._w - q1->_x * q2._x - q1->_y * q2._y - q1->_z * q2._z;
	return _set( q1, dx, dy, dz, dw );
}


namespace Fierce3D
{
	class F3DLIBRARY_API CF3DQuaternion
	{
	public:
		float _x, _y, _z, _w;
		CF3DQuaternion() : _x(0), _y(0), _z(0), _w(1.0f) {}
		CF3DQuaternion( float x, float y, float z, float w) : _x(x), _y(y), _z(z), _w(w) {}
		CF3DQuaternion( const CF3DVector3f& v, float w) : _x(v._x), _y(v._y), _z(v._z), _w(w) {}
		CF3DQuaternion( const CF3DQuaternion& q) : _x(q._x), _y(q._y), _z(q._z), _w(q._w) {}

		// operators
		CF3DQuaternion inline operator + (const CF3DQuaternion& q) const { return CF3DQuaternion(_x + q._x, _y + q._y, _z + q._z, _w + q._w); }
		CF3DQuaternion inline operator * (const CF3DQuaternion& q) const { return CF3DQuaternion(_x * q._x, _y * q._y, _z * q._z, _w + q._w); }
		CF3DQuaternion inline operator - (const CF3DQuaternion& q) const { return CF3DQuaternion(_x - q._x, _y - q._y, _z - q._z, _w + q._w); }
		CF3DQuaternion inline operator - () const {	return CF3DQuaternion( -_x, -_y, -_z, -_w ); }
		CF3DQuaternion inline operator / (const CF3DQuaternion& q) const { return CF3DQuaternion(_x / q._x, _y / q._y, _z / q._z, _w + q._w); }
		CF3DQuaternion inline operator / ( float t ) const { return CF3DQuaternion(_x / t, _y / t, _z / t, _w  / t ); }

		// functions
		inline void SetIdentity() {
			_x = _y = _z = 0.0f; _w = 1.0f;
		}

		inline void Set(const CF3DQuaternion& q) {
			_setQ<CF3DQuaternion>(this, q._x, q._y, q._z, q._w);
		}

		inline void Set(float x, float y, float z, float w) {
			_setQ<CF3DQuaternion>(this, x, y, z, w);
		}

		inline void Normalise() {
			_normaliseQ< CF3DQuaternion, float>(this);
		}

		inline float GetDistanceToVector(const CF3DQuaternion& v) {
			return _distanceQ< CF3DQuaternion, float >(*this, v);
		}

		CF3DQuaternion inverse(void) const {
			return conjugate() / _dotQ<float>(_x, _y, _z, _w);
		}

		inline float GetDotProduct() {
			return _dotQ<float>( _x, _y, _z, _w );
		}

		CF3DVector3f ToVector3() const {
			return CF3DVector3f( _x, _y, _z );
		}

		CF3DQuaternion conjugate(void) const {
			return CF3DQuaternion( -ToVector3(), _w );
		}

		inline CF3DQuaternion GetCrossProduct(const CF3DQuaternion& q) const {
			return CF3DQuaternion ( _y * q._z - _z * q._y + _x * q._w + _w * q._x,
				_z * q._x - _x * q._z + _y * q._w + _w * q._y,
				_x * q._y - _y * q._x + _z * q._w + _w * q._z,
				_w * q._w - _x * q._x - _y * q._y - _z * q._z ) ;
		}

	};
}



#endif // _F3DQUATERNION_H_

