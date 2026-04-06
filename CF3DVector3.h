// Fierce3D Engine Vector3 class file for floats & doubles.
// For Vector4 check out CF3DQuaternion
// Fierce3D Engine (C) 2024 Dave Smith, Otago, New Zealand
//
// May be used freelie, but not misrepresented. Any work or fork
// using the Fierce3D engine MUST (A) Give credit (B) Let me know! :)
//
// email fierce3d@outlook.com
//

#ifndef _F3DVECTOR3_H_
#define _F3DVECTOR3_H_

#include "Fierce3D.h"
#include <cmath>


template <typename T> inline T _dot(T x, T y, T z)
{
	return (x * x + y * y + z * z);
}

template <typename T> inline T _dot(const T& v1, const T& v2)
{
	return (v1._x * v2._x + v1._y * v2._y + v1._z * v2._z);
}


template <typename T, typename V> inline T _distance(const V& v1, const V& v2)
{
	T dx = v2._x - v1._x;
	T dy = v2._y - v1._y;
	T dz = v2._z - v1._z;
	return std::sqrt(dx * dx + dy * dy + dz * dz);
}


template <typename T, typename V> inline V* _normalise(V* v1)
{
	T mag = _dot(v1->_x, v1->_y, v1->_z);
	if (mag != 0) {
		v1->_x /= mag;
		v1->_y /= mag;
		v1->_z /= mag;
	}
	return v1;
}


template <typename VECTOR, typename T> inline VECTOR* _set(VECTOR * v, T x, T y, T z)
{
	v->_x = x;
	v->_y = y;
	v->_z = z;
	return v;
}


template <typename VECTOR, typename T> inline VECTOR _getcrossproduct(const VECTOR* v1, const VECTOR* v2)
{
	T dx = v1->_y * v2->_z - v2->_y * v1->_z; // v1._x - v1._x;
	T dy = v1->_z * v2->_x - v2->_z * v1->_x; // v1._y - v1._y;
	T dz = v1->_x * v2->_y - v2->_x * v1->_y; // v1._z - v1._z;
	return VECTOR(dx, dy, dz);
}


namespace Fierce3D
{
	class F3DLIBRARY_API CF3DVector3f
	{
	public:
		float _x, _y, _z;
		CF3DVector3f() : _x(0.0f), _y(0.0f), _z(0.0f) {}
		CF3DVector3f( float x, float y, float z ) : _x(x), _y(y), _z(z) {}
		CF3DVector3f( const CF3DVector3f& v ) : _x(v._x), _y(v._y), _z(v._z) {}
		// operators
		CF3DVector3f operator + (const CF3DVector3f& v) const { return CF3DVector3f( _x + v._x, _y + v._y, _z + v._z ); }
		CF3DVector3f operator * (const CF3DVector3f& v) const { return CF3DVector3f( _x * v._x, _y * v._y, _z * v._z ); }
		CF3DVector3f operator - (const CF3DVector3f& v) const { return CF3DVector3f( _x - v._x, _y - v._y, _z - v._z ); }
		CF3DVector3f operator - () const { return CF3DVector3f(-_x, -_y, -_z ); }
		CF3DVector3f operator / (const CF3DVector3f& v) const { return CF3DVector3f( _x / v._x, _y / v._y, _z / v._z ); }
		
		// functions
		void  Set(const CF3DVector3f& v) { 
			_x = v._x;
			_y = v._y;
			_z = v._z;
		}

		void  Set(float x, float y, float z) { 
			_x = x;
			_y = y;
			_z = z;
		}
		
		void  Normalise() { 
			_normalise<float>(this); 
		}
		
		float GetDistanceToVector( const CF3DVector3f& v ) { 
			return _distance<float>(*this, v); 
		}
		
		float GetDotProduct() { 
			return _dot<float>(_x, _y, _z); 
		}
		
		void GetCrossProduct( const CF3DVector3f& v ) const
		{ 
			_getcrossproduct<CF3DVector3f, float>(this, &v );
		}
	};


	class F3DLIBRARY_API CF3DVector3d
	{
	public:
		double _x, _y, _z;
		CF3DVector3d() : _x(0.0), _y(0.0), _z(0.0) {}
		CF3DVector3d( double x, double y, double z) : _x(x), _y(y), _z(z) {}
		// operators
		CF3DVector3d operator + (const CF3DVector3d& v) const { return CF3DVector3d(_x + v._x, _y + v._y, _z + v._z); }
		CF3DVector3d operator * (const CF3DVector3d& v) const { return CF3DVector3d(_x * v._x, _y * v._y, _z * v._z); }
		CF3DVector3d operator - (const CF3DVector3d& v) const { return CF3DVector3d(_x - v._x, _y - v._y, _z - v._z); }
		CF3DVector3d operator / (const CF3DVector3d& v) const { return CF3DVector3d(_x / v._x, _y / v._y, _z / v._z); }
		// functions
		void  Set(const CF3DVector3d& v) {
			_x = v._x;
			_y = v._y;
			_z = v._z;
		}
		void  Set(double x, double y, double z) {
			_x = x;
			_y = y;
			_z = z;
		}
		void  Normalise() {
			_normalise<double>(this);
		}
		double GetDistanceToVector(const CF3DVector3d& v) {
			return _distance<double>(*this, v);
		}
		double GetDotProduct() {
			return _dot<double>(_x, _y, _z);
		}
		void GetCrossProduct(const CF3DVector3d& v) const
		{
			_getcrossproduct<CF3DVector3d, double>(this, &v);
		}

	};
}


#endif // _F3DVECTOR3_H_
