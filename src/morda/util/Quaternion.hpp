/**
 * @author Ivan Gagis <igagis@gmail.com>
 */

#pragma once


#ifdef DEBUG
#	include <iostream>
#endif

#include <cmath>

#include <utki/debug.hpp>

#include "../config.hpp"


namespace morda{



//forward declarations
template <class T> class Vector3;
template <class T> class Matrix4;



/**
 * @brief Quaternion template class.
 */
template <typename T> class Quaternion{
public:
	/**
	 * @brief x component.
	 */
	T x;

	/**
	 * @brief y component.
	 */
	T y;
	
	/**
	 * @brief z component.
	 */
	T z;

	/**
	 * @brief w component.
	 */
	T w;



	/**
	 * @brief Create quaternion with given components.
	 * @param x - x component.
	 * @param y - y component.
	 * @param z - z component.
	 * @param w - w component.
	 */
	Quaternion(T x, T y, T z, T w)noexcept :
			x(x),
			y(y),
			z(z),
			w(w)
	{}



	/**
	 * @brief Construct rotation quaternion.
	 * Constructs a quaternion representing rotation (unit quaternion).
	 * Rotation is given by 3 dimensional vector, whose direction defines the
	 * axis about which rotation is done and its magnitude defines the angle of
	 * rotation in radians.
	 * @param rot - vector which defines the rotation.
	 */
	Quaternion(const Vector3<T>& rot)noexcept;



	/**
	 * @brief Default constructor.
	 * Note, that it does not initialize quaternion components,
	 * right after creation the components are undefined.
	 */
	Quaternion()noexcept{}



	/**
	 * @brief Complex conjugate of this quaternion.
	 * Note, complex conjugate of quaternion (x, y, z, w) is (-x, -y, -z, w).
	 * @return quaternion instance which is a complex conjugate of this quaternion.
	 */
	Quaternion operator!()const noexcept{
		return Quaternion(-this->x, -this->y, -this->z, this->w);
	}



	/**
	 * @brief Add quaternion and assign.
	 * Adds specified quaternion to this quaternion.
     * @param q - quaternion to add to this quaternion.
     * @return Reference to this quaternion object.
     */
	Quaternion& operator+=(const Quaternion& q)noexcept{
		this->x += q.x;
		this->y += q.y;
		this->z += q.z;
		this->w += q.w;
		return (*this);
	}



	/**
	 * @brief Addition of quaternions.
	 * Calculates sum of this quaternion and another specified quaternion.
     * @param q - quaternion to add.
     * @return A quaternion object representing sum of quaternions.
     */
	Quaternion operator+(const Quaternion& q)const noexcept{
		return (Quaternion(*this) += q);
	}



	//NOTE: operator=() will be generated by compiler



	/**
	 * @brief Multiply by scalar and assign.
	 * Multiplies this quaternion by scalar and assigns the result to this quaternion instance.
	 * @param s - scalar value to multiply by.
	 * @return reference to this quaternion instance.
	 */
	Quaternion& operator*=(T s)noexcept{
		this->x *= s;
		this->y *= s;
		this->z *= s;
		this->w *= s;
		return (*this);
	}



	/**
	 * @brief Multiply by scalar.
	 * @param s - scalar value to multiply by.
	 * @return resulting quaternion instance.
	 */
	Quaternion operator*(T s)const noexcept{
		return (Quaternion(*this) *= s);
	}



	/**
	 * @brief Divide by scalar and assign.
	 * Divide this quaternion by scalar and assigns the result to this quaternion instance.
	 * @param s - scalar value to divide by.
	 * @return reference to this quaternion instance.
	 */
	Quaternion& operator/=(T s)noexcept{
		this->x /= s;
		this->y /= s;
		this->z /= s;
		this->w /= s;
		return (*this);
	}



	/**
	 * @brief Divide by scalar.
	 * @param s - scalar value to divide by.
	 * @return resulting quaternion instance.
	 */
	Quaternion operator/(T s)const noexcept{
		return (Quaternion(*this) /= s);
	}



	/**
	 * @brief Dot product of quaternions.
	 * Dot product of two quaternions (x1, y1, z1, w1) and
	 * (x2, y2, z2, w2) is a scalar calculated as follows
	 * x1 * x2 + y1 * y2 + z1 * z2 + w1 * w2
	 * @return result of the dot product.
	 */
	T operator*(const Quaternion& q)const noexcept{
		return this->x * q.x + this->y * q.y + this->z * q.z + this->w * q.w;
	}



	/**
	 * @brief Multiply by quaternion and assign.
	 * Multiplies this quaternion by another quaternion from the right
	 * (quaternions multiplication is not associative) and assigns the
	 * result to this quaternion instance.
	 * @param q - quaternion to multiply by.
	 * @return reference to this quaternion instance.
	 */
	Quaternion& operator%=(const Quaternion& q)noexcept{
		T a = (this->w + this->x) * (q.w + q.x);
		T b = (this->z - this->y) * (q.y - q.z);
		T c = (this->x - this->w) * (q.y + q.z);
		T d = (this->y + this->z) * (q.x - q.w);
		T e = (this->x + this->z) * (q.x + q.y);
		T f = (this->x - this->z) * (q.x - q.y);
		T g = (this->w + this->y) * (q.w - q.z);
		T h = (this->w - this->y) * (q.w + q.z);

		this->x = a - (e + f + g + h) * 0.5f;
		this->y = -c + (e - f + g - h) * 0.5f;
		this->z = -d + (e - f - g + h) * 0.5f;
		this->w = b + (-e - f + g + h) * 0.5f;
		return (*this);
	}



	/**
	 * @brief Multiply by quaternion.
	 * Multiplies this quaternion by another quaternion from the right
	 * (quaternions multiplication is not associative).
	 * @param q - quaternion to multiply by.
	 * @return resulting quaternion instance.
	 */
	//multiplication of quaternions
	Quaternion operator%(const Quaternion& q)const noexcept{
		return (Quaternion(*this) %= q);
	}



	/**
	 * @brief Initialize with identity quaternion.
	 * Note, identity quaternion is (0, 0, 0, 1).
	 * It is a unit quaternion representing no rotation.
	 * @return reference to this quaternion instance.
	 */
	Quaternion& Identity()noexcept{
		this->x = T(0);
		this->y = T(0);
		this->z = T(0);
		this->w = T(1);
		return *this;
	}



	/**
	 * @brief Complex conjugate this quaternion.
	 * Note, complex conjugate of quaternion (x, y, z, w) is (-x, -y, -z, w).
	 * @return reference to this quaternion instance.
	 */
	Quaternion& Conjugate()noexcept{
		return (*this = this->operator!());
	}



	/**
	 * @brief Negate this quaternion.
	 * Note, negating quaternion means changing the sign of its every component.
	 * @return reference to this quaternion instance.
	 */
	Quaternion& Negate()noexcept{
		this->x = -this->x;
		this->y = -this->y;
		this->z = -this->z;
		this->w = -this->w;
		return *this;
	}



	/**
	 * @brief Calculate power 2 from quaternion magnitude.
	 * @return power 2 from magnitude.
	 */
	T MagPow2()const noexcept{
		return (*this) * (*this);
	}



	/**
	 * @brief Calculate quaternion magnitude.
	 * @return quaternion magnitude.
	 */
	T Magnitude()const noexcept{
		return std::sqrt(this->MagPow2());
	}



	/**
	 * @brief Normalize quaternion.
	 * Note, after normalization, the quaternion becomes a unit quaternion.
	 * If it is a quaternion of zero magnitude, then the result is undefined.
	 * @return reference to this quaternion instance.
	 */
	Quaternion& Normalize()noexcept{
		return (*this) /= Magnitude();
	}



	/**
	 * @brief Initialize rotation.
	 * Initializes this quaternion to a unit quaternion defining a rotation.
	 * A rotation is given by normalized axis vector and angle. Direction of rotation is determined by right-hand rule.
     * @param axisX - X component of rotation axis.
	 * @param axisY - Y component of rotation axis.
	 * @param axisZ - Z component of rotation axis.
     * @param angle - rotation angle.
     * @return Reference to this quaternion object.
     */
	Quaternion& InitRot(T axisX, T axisY, T axisZ, T angle)noexcept{
		T sina2 = ::sin(angle / 2);
		this->w = ::cos(angle / 2);
		this->x = axisX * sina2;
		this->y = axisY * sina2;
		this->z = axisZ * sina2;
		return *this;
	}
	
	
	
	/**
	 * @brief Initialize rotation.
	 * Initializes this quaternion to a unit quaternion defining a rotation.
	 * A rotation is given by normalized axis vector and angle. Direction of rotation is determined by right-hand rule.
     * @param axis - rotation axis, a normalized vector.
     * @param angle - rotation angle.
     * @return Reference to this quaternion object.
     */
	Quaternion& InitRot(const Vector3<T>& axis, T angle)noexcept;
	
	
	
	/**
	 * @brief Initialize rotation.
	 * Initializes this quaternion to a unit quaternion defining a rotation.
	 * A rotation is given by rotation vector, where the magnitude of the vector
	 * is a rotation angle in radians and vector direction defines the rotation axis.
	 * Direction of rotation is determined by right-hand rule.
     * @param rot - rotation vector.
     * @return Reference to this quaternion object.
     */
	Quaternion& InitRot(const Vector3<T>& rot)noexcept;



	/**
	 * @brief Convert this quaternion to 4x4 matrix.
	 * Assuming that this quaternion is a unit quaternion, converts this quaternion
	 * to a rotation matrix.
     * @return Rotation matrix.
     */
	Matrix4<T> ToMatrix4()const noexcept;


	
	/**
	 * @brief Spherical linear interpolation.
	 * Calculates spherical linear interpolation (SLERP) between two quaternions,
	 * the first quaternion is this one and the second is passed as argument.
	 * The result of SLERP is quaternion itself.
	 * SLERP(q1, q2, t) = q1 * sin((1 - t) * alpha) / sin(alpha) + q2 * sin(t * alpha) / sin(alpha),
	 * where cos(alpha) = (q1, q2) (dot product of unit quaternions q1 and q2).
	 * Quaternions q1 and q2 are assumed to be unit quaternions and the resulting quaternion is also a unit quaternion.
     * @param quat - quaternion to interpolate to.
     * @param t - interpolation parameter, value from [0 : 1].
     * @return Resulting quaternion of SLERP(this, quat, t).
     */
	Quaternion Slerp(const Quaternion& quat, T t)const noexcept{
		//Since quaternions are normalized the cosine of the angle alpha
		//between quaternions is equal to their dot product.
		T cosalpha = (*this) * quat;

		T sign;
		
		//If the dot product is less than 0, the angle alpha between quaternions
		//is greater than 90 degrees. Then we negate second quaternion to make alpha
		//to be less than 90 degrees. It is possible since normalized quaternions
		//q and -q represent the same rotation.
		if(cosalpha < T(0)){
			//Negate the second quaternion and the result of the dot product (i.e. cos(alpha))
			sign = -1;
			cosalpha = -cosalpha;
		}else{
			sign = 1;
		}

		//interpolation done by the following general formula:
		//RESULT = this * sc1(t) + quat * sc2(t).
		//Where sc1, sc2 called interpolation scales.
		T sc1, sc2;//Define variables for scales for interpolation

		//Check if the angle alpha between the 2 quaternions is big enough
		//to make SLERP. If alpha is small then we do a simple linear
		//interpolation between quaternions instead of SLERP!
		//It is also used to avoid divide by zero since sin(0) is 0.
		//We made threshold for cos(alpha) > 0.9f (if cos(alpha) == 1 then alpha is 0).
		if(cosalpha > T(0.9f)){
			//Get the angle alpha between the 2 quaternions, and then store the sin(alpha)
			T alpha = std::acos(cosalpha);
			T sinalpha = ::sin(alpha);

			//Calculate the scales for q1 and q2, according to the angle and it's sine value
			sc1 = ::sin((1 - t) * alpha) / sinalpha;
			sc2 = ::sin(t * alpha) / sinalpha;
		}else{
			sc1 = (1 - t);
			sc2 = t;
		}

		// Calculate the x, y, z and w values for the interpolated quaternion.
		return (*this) * sc1 + quat * (sc2 * sign);
	}

#ifdef DEBUG  
	friend std::ostream& operator<<(std::ostream& s, const Quaternion<T>& quat){
		s << "(" << quat.x << ", " << quat.y << ", " << quat.z << ", " << quat.w << ")";
		return s;
	}
#endif  
};//~class Quaternion



}//~namespace



#include "Vector3.hpp"
#include "Matrix4.hpp"



namespace morda{

//=================================
// functions implementation
//=================================

template <class T> Quaternion<T>::Quaternion(const Vector3<T>& rot)noexcept{
	this->InitRot(rot);
}



template <class T> Quaternion<T>& Quaternion<T>::InitRot(const Vector3<T>& rot)noexcept{
	T mag = rot.Magnitude();
	if(mag != 0){
		this->InitRot(rot.x / mag, rot.y / mag, rot.z / mag, mag);
	}else{
		this->Identity();
	}
	return *this;
}



template <class T> Quaternion<T>& Quaternion<T>::InitRot(const Vector3<T>& axis, T angle)noexcept{
	return this->InitRot(axis.x, axis.y, axis.z, angle);
}



template <class T> Matrix4<T> Quaternion<T>::ToMatrix4()const noexcept{
	return Matrix4<T>(*this);
}



//=====================
// Convenient typedefs
//=====================

typedef Quaternion<float> Quatf;
static_assert(sizeof(Quatf) == sizeof(float) * 4, "size mismatch");

typedef Quaternion<double> Quatd;
static_assert(sizeof(Quatd) == sizeof(double) * 4, "size mismatch");

typedef Quaternion<real> Quatr;
static_assert(sizeof(Quatr) == sizeof(real) * 4, "size mismatch");



}//~namespace
