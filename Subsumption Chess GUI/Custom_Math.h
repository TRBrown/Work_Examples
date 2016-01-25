#ifndef CUSTOM_MATH_H
#define CUSTOM_MATH_H
#pragma warning(disable:4995)

#include "d3dx9.h" 
#include "d3d9.h"
//#include <string>
//#include <vector>


//===============================================================
// Math Constants

const float INFINITY = FLT_MAX;
const float EPSILON  = 0.001f;
//========================================

// ******************************
// Quaternion struct and methods
// ******************************
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

//
// Axis Aligned Bounding Box
//
struct AABB 
{
	D3DXVECTOR3 minPt;
	D3DXVECTOR3 maxPt;
	
	// Initialize to an infinitely small bounding box.
	AABB();

	AABB(D3DXVECTOR3 v3Min, D3DXVECTOR3 v3Max);

	D3DXVECTOR3 center()const;

	D3DXVECTOR3 extent()const;

	void xform(const D3DXMATRIX& M, AABB& out);

	float rayIntersect(	const D3DXVECTOR3 &rayOrg, const D3DXVECTOR3 &rayDelta,
						D3DXVECTOR3 *returnNormal = 0, float *checkT = 0) const;

};

float	vec3Magnitude (D3DXVECTOR3 position1, D3DXVECTOR3 position2);
float	vec4Magnitude(D3DXVECTOR4 position1, D3DXVECTOR4 position2);

// ******************************************
// Ray / Axis-Aligned Bounding Box Overlap 
// ******************************************

enum CLASSIFICATION
{ MMM, MMP, MPM, MPP, PMM, PMP, PPM, PPP, POO, MOO, OPO, OMO, OOP, OOM,
OMM,OMP,OPM,OPP,MOM,MOP,POM,POP,MMO,MPO,PMO,PPO};

struct RayAABB
{	
	//common variables
	float x, y, z;		// ray origin	
	float i, j, k;		// ray direction	
	float ii, ij, ik;	// inverses of direction components

	// ray slope
	int classification;
	float ibyj, jbyi, kbyj, jbyk, ibyk, kbyi; //slope
	float c_xy, c_xz, c_yx, c_yz, c_zx, c_zy;	
};

void make_ray(D3DXVECTOR3 Origin, D3DXVECTOR3 Direction, RayAABB *r);

bool slope(RayAABB *r, AABB *b);

#endif

