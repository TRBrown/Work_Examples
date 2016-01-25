#ifndef CUSTOM_MATH_H
#define CUSTOM_MATH_H
#pragma warning(disable:4995)

#include "d3dx9.h" 
#include "d3d9.h"
//#include <string>
//#include <vector>


//
// Quaternion struct and methods
//
typedef struct Quaternion
{
public:
	
	float w;
	float i;
	float j;
	float k;
	
	Quaternion();// Default Q
	Quaternion( CONST FLOAT * );// Q from array [4]
	Quaternion( CONST Quaternion& ); // Q from Q
	Quaternion( FLOAT thetaX, FLOAT thetaY, FLOAT thetaZ );//Q from Euler Angles
	Quaternion( FLOAT fw, FLOAT fi, FLOAT fj, FLOAT fk );//Q from  Floats
	
	void QuaternionIdentity();
	
		// casting
	operator FLOAT* ();
	operator CONST FLOAT* () const;

	// assignment operators
	Quaternion& operator += ( CONST Quaternion& );
	Quaternion& operator -= ( CONST Quaternion& );
	Quaternion& operator *= ( FLOAT );
	Quaternion& operator *= ( CONST Quaternion& q );
	Quaternion& operator /= ( FLOAT );

	// binary operators
	Quaternion operator + ( CONST Quaternion& ) const;
	Quaternion operator - ( CONST Quaternion& ) const;
	Quaternion operator * ( FLOAT ) const;
	Quaternion operator * ( CONST Quaternion& q ) const;
	Quaternion operator / ( FLOAT ) const;

	BOOL operator == ( CONST Quaternion& ) const;
	BOOL operator != ( CONST Quaternion& ) const;

	// unary operators
	Quaternion  operator + () const;
	Quaternion  operator - () const;

	//methods
	Quaternion	Conjugate ();
	float		Magnitude ();
	D3DXVECTOR3 RotationV3 ( CONST D3DXVECTOR3& ) const;
	void		Normalise();

} Quaternion, *LPQuaternion;


#endif

