/* The MIT License:

Copyright (c) 2008-2012 Ivan Gagis <igagis@gmail.com>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */

// Home page: http://morda.googlecode.com

/**
 * @author Ivan Gagis <igagis@gmail.com>
 */

#pragma once

#include <cmath> //TODO: ?
#include <cstdlib> //TODO: ?
#include <algorithm> //TODO: ?

#ifdef DEBUG
#	include <iostream>
#endif

#include <ting/debug.hpp> //TODO: ?
#include <ting/types.hpp> //TODO: ?
#include <ting/math.hpp> //TODO: ?
#include <ting/Exc.hpp>//TODO: ?

//TODO: add throw() where needed

namespace morda{



/**
 * @brief 4x4 matrix template class.
 * Note, that this matrix class stores elements in memory column by column.
 * This is the same way as OpenGL matrices are stored in memory.
 * This means easy use of this class with OpenGL.
 */
template <typename T> class Matrix4{
	//TODO: rewrite using Vec4
	//OpenGL compatible matrix elements array, if T is float or double
	T m[4 * 4]; //matrix components 0-3 1st column, 4-7 2nd column, 8-11 3rd column, 12-15 4th column
public:


	
	/**
	 * @brief Default constructor.
	 * NOTE: it does not initialize the matrix with any values.
	 * Matrix elements are undefined after the matrix is created with this constructor.
	 */
	inline Matrix4(){}//Default constructor.



	//NOTE: copy constructor will be generated by compiler

	

	/**
	 * @brief returns pointer to specified column.
	 * Returns pointer to array of 4 elements which forms a matrix column specified by argument.
	 * Thus, it is possible to access matrix elements using double [] operator as follows:
	 * @code
	 * Matrix4 m;
	 * m[0][0] = 1;//assign 1 to element at row 0 column 0
	 * m[3][2] = 3;//assign 3 to element at row 2 column 3
	 * float elem = m[4][3];//assign value at row 3 column 4 of the matrix to variable 'elem'
	 * @endcode
	 * @param col - column number.
	 * @return pointer to array of 4 elements which forms the requested column of the matirx.
	 */
	//TODO: rewrite using Vec4
	inline T* operator[](unsigned col){
		ASSERT(col < 4)
		return &this->m[col * 4];
	}

	/**
	 * @brief returns pointer to specified column.
	 * Const variant of operator[].
	 * @param col - column number.
	 * @return pointer to array of 4 elements which forms the requested column of the matirx.
	 */
	//TODO: rewrite using Vec4
	inline const T* operator[](unsigned col)const{
		ASSERT(col < 4)
		return &this->m[col * 4];
	}

	//TODO: doxygen
	//Multiply by Vector3 (M * V). i.e. transform vector with transformation matrix
	Vector3<T> operator*(const Vector3<T>& vec)const{
		return Vector3<T>(
					this->m[0] * vec[0] + this->m[4] * vec[1] + this->m[8] * vec[2] + this->m[12],
					this->m[1] * vec[0] + this->m[5] * vec[1] + this->m[9] * vec[2] + this->m[13],
					this->m[2] * vec[0] + this->m[6] * vec[1] + this->m[10] * vec[2] + this->m[14]
				);
	}

	
	
	//NOTE: operator=() will be generated by compiler



	/**
	 * @brief Transpose matrix.
	 */
	Matrix4& Transpose(){
		std::swap(this->m[1], this->m[4]);
		std::swap(this->m[2], this->m[8]);
		std::swap(this->m[6], this->m[9]);
		std::swap(this->m[3], this->m[12]);
		std::swap(this->m[7], this->m[13]);
		std::swap(this->m[11], this->m[14]);
		return (*this);
	}



	/**
	 * @brief Multiply by matrix from the right.
	 * Multiply this matrix by matrix M from the right, i.e. m  = m * M
	 * @param M - matrix to multiply by.
	 * @return reference to this matrix object.
	 */
	Matrix4& RightMultMatrix(const Matrix4 &M){
		//TODO: rewrite to use Matrix4 instead of T tmpM[16]
		T tmpM[16];
		for(unsigned i = 0; i < 4; ++i){
			tmpM[4*i]  =m[0]*M.m[4*i]+m[4]*M.m[4*i+1]+m[8]*M.m[4*i+2]+ m[12]*M.m[4*i+3];
			tmpM[4*i+1]=m[1]*M.m[4*i]+m[5]*M.m[4*i+1]+m[9]*M.m[4*i+2]+ m[13]*M.m[4*i+3];
			tmpM[4*i+2]=m[2]*M.m[4*i]+m[6]*M.m[4*i+1]+m[10]*M.m[4*i+2]+m[14]*M.m[4*i+3];
			tmpM[4*i+3]=m[3]*M.m[4*i]+m[7]*M.m[4*i+1]+m[11]*M.m[4*i+2]+m[15]*M.m[4*i+3];
		}
		memcpy(this->m, tmpM, sizeof(this->m));
		//*this=tmp;
		return (*this);
	}



	/**
	 * @brief Multiply by matrix from the left.
	 * Multiply this matrix by matrix M from the left, i.e. m  = M * m
	 * @param M - matrix to multiply by.
	 * @return reference to this matrix object.
	 */
	Matrix4& LeftMultMatrix(const Matrix4& M){
		//TODO: rewrite to use Matrix4 instead of T tmpM[16]
		T tmpM[16];
		for(unsigned i = 0; i < 4; ++i){
			tmpM[4*i]  =m[4*i]*M.m[0]+m[4*i+1]*M.m[4]+m[4*i+2]*M.m[8]+ m[4*i+3]*M.m[12];
			tmpM[4*i+1]=m[4*i]*M.m[1]+m[4*i+1]*M.m[5]+m[4*i+2]*M.m[9]+ m[4*i+3]*M.m[13];
			tmpM[4*i+2]=m[4*i]*M.m[2]+m[4*i+1]*M.m[6]+m[4*i+2]*M.m[10]+m[4*i+3]*M.m[14];
			tmpM[4*i+3]=m[4*i]*M.m[3]+m[4*i+1]*M.m[7]+m[4*i+2]*M.m[11]+m[4*i+3]*M.m[15];
		}
		memcpy(this->m, tmpM, sizeof(this->m));
		return (*this);
	}



	/**
	 * @brief Initialize this matrix with identity matrix.
	 */
	Matrix4& Identity(){
		this->m[0] = 1;    this->m[4] = 0;    this->m[8] = 0;    this->m[12] = 0;
		this->m[1] = 0;    this->m[5] = 1;    this->m[9] = 0;    this->m[13] = 0;
		this->m[2] = 0;    this->m[6] = 0;    this->m[10] = 1;   this->m[14] = 0;
		this->m[3] = 0;    this->m[7] = 0;    this->m[11] = 0;   this->m[15] = 1;
		return (*this);
	}



	/**
	 * @brief Multiply current matrix by scale matrix.
	 * Multiplies this matrix by Scale matrix from the right (M = M * S).
	 * @param scale - vector of scaling factors in x, y and z directons.
	 * @return reference to this Matrix instance.
	 */
	Matrix4& Scale(const Vector3<T>& scale){
		//calculate first column
		this->m[0] *= scale[0];
		this->m[1] *= scale[0];
		this->m[2] *= scale[0];
		this->m[3] *= scale[0];

		//calculate second column
		this->m[4] *= scale[1];
		this->m[5] *= scale[1];
		this->m[6] *= scale[1];
		this->m[7] *= scale[1];

		//calculate third column
		this->m[8] *= scale[2];
		this->m[9] *= scale[2];
		this->m[10] *= scale[2];
		this->m[11] *= scale[2];

		//NOTE: 4th column remains unchanged
		return (*this);
	}



	/**
	 * @brief Multiply current matrix by scale matrix.
	 * Multiplies this matrix by Scale matrix from the right (M = M * S).
	 * @param scale - vector of scaling factors in x and y directions, scaing factor in z direction is 1.
	 * @return reference to this Matrix instance.
	 */
	Matrix4& Scale(const Vector2<T>& scale){
		//calculate first column
		this->m[0] *= scale[0];
		this->m[1] *= scale[0];
		this->m[2] *= scale[0];
		this->m[3] *= scale[0];

		//calculate second column
		this->m[4] *= scale[1];
		this->m[5] *= scale[1];
		this->m[6] *= scale[1];
		this->m[7] *= scale[1];

		//NOTE: 3rd and 4th columns remain unchanged
		return (*this);
	}



	/**
	 * @brief Multiply current matrix by scale matrix.
	 * Multiplies this matrix by Scale matrix from the right (M = M * S).
	 * @param x - scaling factor in x directon.
	 * @param y - scaling factor in y directon.
	 * @param z - scaling factor in z directon.
	 * @return reference to this Matrix instance.
	 */
	Matrix4& Scale(T x, T y, T z){
		return this->Scale(Vector3<T>(x, y, z));
	}



	/**
	 * @brief Multiply current matrix by scale matrix.
	 * Multiplies this matrix by Scale matrix from the right (M = M * S).
	 * Scaling factor in z direction is 1.
	 * @param x - scaling factor in x directon.
	 * @param y - scaling factor in y directon.
	 * @return reference to this Matrix instance.
	 */
	Matrix4& Scale(T x, T y){
		return this->Scale(Vector2<T>(x, y));
	}



	/**
	 * @brief Multiply current matrix by scale matrix.
	 * Multiplies this matrix by Scale matrix from the right (M = M * S).
	 * @param scale - scaling factor to be applied in all 3 directon (x, y and z).
	 * @return reference to this Matrix instance.
	 */
	Matrix4& Scale(T scale){
		return this->Scale(Vector3<T>(scale, scale, scale));
	}



	/**
	 * @brief Multiply this matrix by translation matrix.
	 * Multiplies this matrix by Translation matrix from the right (M = M * T)
	 * @param t - translation vector.
	 * @return reference to this matrix object.
	 */
	Matrix4& Translate(const Vector3<T>& t){
		//NOTE: 1st, 2nd and 3rd columns remain unchanged

		//calculate fourth column
		this->m[12] = this->m[0] * t[0] + this->m[4] * t[1] + this->m[8] * t[2] + this->m[12];
		this->m[13] = this->m[1] * t[0] + this->m[5] * t[1] + this->m[9] * t[2] + this->m[13];
		this->m[14] = this->m[2] * t[0] + this->m[6] * t[1] + this->m[10] * t[2] + this->m[14];
		this->m[15] = this->m[3] * t[0] + this->m[7] * t[1] + this->m[11] * t[2] + this->m[15];

		return (*this);
	}



	/**
	 * @brief Multiply this matrix by translation matrix.
	 * Multiplies this matrix by Translation matrix from the right (M = M * T).
	 * Translation only occurs in x-y plane, no translation in z direction,
	 * i.e. z component of translation vector is assumed being 0.
	 * @param t - translation vector.
	 * @return reference to this matrix object.
	 */
	Matrix4& Translate(const Vector2<T>& t){
		//NOTE: 1st, 2nd and 3rd columns remain unchanged

		//calculate fourth column
		this->m[12] = this->m[0] * t[0] + this->m[4] * t[1] + this->m[12];
		this->m[13] = this->m[1] * t[0] + this->m[5] * t[1] + this->m[13];
		this->m[14] = this->m[2] * t[0] + this->m[6] * t[1] + this->m[14];
		this->m[15] = this->m[3] * t[0] + this->m[7] * t[1] + this->m[15];

		return (*this);
	}


	
	/**
	 * @brief Multiply this matrix by translation matrix.
	 * Multiplies this matrix by Translation matrix from the right (M = M * T).
	 * @param x - x component of translation vector.
	 * @param y - y component of translation vector.
	 * @param z - z component of translation vector.
	 * @return reference to this matrix object.
	 */
	Matrix4& Translate(T x, T y, T z){
		return this->Translate(Vector3<T>(x, y, z));
	}
	
	

	/**
	 * @brief Multiply this matrix by translation matrix.
	 * Multiplies this matrix by Translation matrix from the right (M = M * T).
	 * Translation only occurs in x-y plane, no translation in z direction,
	 * i.e. z component of translation vector is assumed being 0.
	 * @param x - x component of translation vector.
	 * @param y - y component of translation vector.
	 * @return reference to this matrix object.
	 */
	Matrix4& Translate(T x, T y){
		return this->Translate(Vector2<T>(x, y));
	}



	/**
	 * @brief Multiply this matrix by rotation matrix.
	 * Multiplies this matrix by Rotation matrix from the right (M = M * R).
	 * @param q - quaternion, representing the rotation.
	 * @return reference to this matrix object.
	 */
	inline Matrix4& Rotate(const Quaternion<T>& q);//implementation see below



	/**
	 * @brief Multiply this matrix by rotation matrix.
	 * Multiplies this matrix by Rotation matrix from the right (M = M * R).
	 * @param rot - vector, representing the rotation. The vector direction
	 *              defines the axis of rotation, vector length defines
	 *              the angle of rotation in radians.
	 * @return reference to this matrix object.
	 */
	inline Matrix4& Rotate(const Vector3<T>& rot){
		return this->Rotate(Quaternion<T>(rot));
	}



	/**
	 * @brief Multiply this matrix by rotation matrix.
	 * Multiplies this matrix by Rotation matrix from the right (M = M * R).
	 * Rotation is done around (0, 0, 1) axis by given number of radians.
	 * Positive direction of rotation is determined by a right-hand rule.
	 * @param rot - the angle of rotation in radians.
	 * @return reference to this matrix object.
	 */
	inline Matrix4& Rotate(T rot){
		return this->Rotate(Vector3<T>(0, 0, rot));
	}


	
#ifdef DEBUG
	friend std::ostream& operator<<(std::ostream& s, const Matrix4<T>& mat){
		s << "\n";
		s << "/" << mat[0][0] << " " << mat[1][0] << " " << mat[2][0] << " " << mat[3][0] << "\\" << std::endl;
		s << "|" << mat[0][1] << " " << mat[1][1] << " " << mat[2][1] << " " << mat[3][1] << "|" << std::endl;
		s << "|" << mat[0][2] << " " << mat[1][2] << " " << mat[2][2] << " " << mat[3][2] << "|" << std::endl;
		s << "\\" << mat[0][3] << " " << mat[1][3] << " " << mat[2][3] << " " << mat[3][3] << "/";
		return s;
	};
#endif
};//~class Matrix4



//=================================
// inline functions implementation
//=================================

template <class T> inline Matrix4<T>& Matrix4<T>::Rotate(const Quaternion<T>& q){
	Matrix4<T> rm;
	q.CreateMatrix4(rm);
	this->RightMultMatrix(rm);
	return (*this);
}



//=====================
// Convenient typedefs
//=====================

typedef Matrix4<float> Matr4f;
STATIC_ASSERT(sizeof(Matr4f) == sizeof(float) * 4 * 4)
typedef Matrix4<double> Matr4d;
STATIC_ASSERT(sizeof(Matr4d) == sizeof(double) * 4 * 4)



}//~namespace
