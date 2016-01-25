#include "DXUT.h" 
#include "Custom_Math.h"
#define TOLERANCE 0.00001f


Quaternion::Quaternion()
{

	w = 1;
	i = 0;
	j = 0;
	k = 0;
}

Quaternion::Quaternion( CONST FLOAT *pf )
{

	w = pf[0];
	i = pf[1];
	j = pf[2];
	k = pf[2];
}


Quaternion::Quaternion( CONST Quaternion& q )
{
	w = q.w;
	i = q.i;
	j = q.j;
	k = q.k;
}

Quaternion::Quaternion( FLOAT thetaX, FLOAT thetaY, FLOAT thetaZ )
{
	//Qall = (Qz * Qx) * Qy 
	//Compiler order = Qy(Qz * Qx) due to parentheses
	Quaternion x;
	Quaternion y;
	Quaternion z;

	if (fabs(thetaX) > 0){x = Quaternion(cos(thetaX/2), sin(thetaX/2), 0.0f		  , 0.0f);}
	if (fabs(thetaY) > 0){y = Quaternion(cos(thetaY/2), 0.0f		   , sin(thetaY/2), 0.0f);}
	if (fabs(thetaZ) > 0){z = Quaternion(cos(thetaZ/2), 0.0f		   , 0.0f		 , sin(thetaZ/2));}

	const Quaternion qResult = y * (x * z);
	w = qResult.w;
	i = qResult.i;
	j = qResult.j;
	k = qResult.k;

	Normalise();
}

Quaternion::Quaternion( FLOAT fw, FLOAT fi, FLOAT fj, FLOAT fk )
{
	w = fw;
	i = fi;
	j = fj;
	k = fk;
}


void Quaternion::QuaternionIdentity()
{

	w = 1;
	i = 0;
	j = 0;
	k = 0;
}

// casting

Quaternion::operator FLOAT* ()
{
	return (FLOAT *) &w;
}


Quaternion::operator CONST FLOAT* () const
{
	return (CONST FLOAT *) &w;
}


// assignment operators

 Quaternion& Quaternion::operator += ( CONST Quaternion& q )
{
	w += q.w;
	i += q.i;
	j += q.j;
	k += q.k;
	return *this;
}


Quaternion& Quaternion::operator -= ( CONST Quaternion& q )
{
	w -= q.w;
	i -= q.i;
	j -= q.j;
	k -= q.k;
	return *this;
}


Quaternion& Quaternion::operator *= ( FLOAT f )
{
	w *= f;
	i *= f;
	j *= f;
	k *= f;
	return *this;
}


Quaternion& Quaternion::operator *= ( CONST Quaternion& q )
{
	w = (w * q.w) - (i * q.i) - (j * q.j) - (k * q.k);
	i = (w * q.i) + (i * q.w) + (j * q.k) - (k * q.j);
	j = (w * q.j) + (j * q.w) + (k * q.i) - (i * q.k);
	k = (w * q.k) + (k * q.w) + (i * q.j) - (j * q.i); 
	return *this;
}


Quaternion& Quaternion::operator /= ( FLOAT f )
{
	FLOAT fInv = 1.0f / f;
	w *= fInv;
	i *= fInv;
	j *= fInv;
	k *= fInv;
	return *this;
}


// binary operators

Quaternion Quaternion::operator + ( CONST Quaternion& q ) const
{
	return Quaternion(w + q.w, i + q.i, j + q.j, k + q.k);
}


Quaternion Quaternion::operator - ( CONST Quaternion& q ) const
{
	return Quaternion(w - q.w, i - q.i, j - q.j, k - q.k);
}


Quaternion Quaternion::operator * ( FLOAT f ) const
{
	return Quaternion(w * f, i * f, j * f, k * f);
}

Quaternion Quaternion::operator / ( FLOAT f ) const
{
	FLOAT fInv = 1.0f / f;
	return Quaternion(w * fInv, i * fInv, j * fInv, k * fInv);
}

Quaternion Quaternion::operator * ( CONST Quaternion& q ) const
{
	float tw = (w * q.w) - (i * q.i) - (j * q.j) - (k * q.k);
	float ti = (w * q.i) + (i * q.w) + (j * q.k) - (k * q.j);
	float tj = (w * q.j) + (j * q.w) + (k * q.i) - (i * q.k);
	float tk = (w * q.k) + (k * q.w) + (i * q.j) - (j * q.i); 	
	return Quaternion(tw, ti, tj, tk);
}


BOOL Quaternion::operator == ( CONST Quaternion& q ) const
{
	return w == q.w && i == q.i && j == q.j && k == q.k;
}


BOOL Quaternion::operator != ( CONST Quaternion& q ) const
{
	return w != q.w && i != q.i && j != q.j && k != q.k;
}

// unary operators
Quaternion Quaternion::operator + () const
{
	return *this;
}

Quaternion Quaternion::operator - () const
{
	return Quaternion(-w, -i, -j, -k);
}

Quaternion Quaternion::Conjugate ()
{
	return Quaternion(w, -i, -j, -k);
}

float Quaternion::Magnitude ()
{
	return sqrt(pow(w,2) + pow(i,2) + pow(j,2)+ pow(k,2));
}

D3DXVECTOR3 Quaternion::RotationV3 ( CONST D3DXVECTOR3& v) const
{
	Quaternion p = Quaternion(0, v.x, v.y, v.z);
	Quaternion q1 = *this * p;
	Quaternion q2 = *this;
	Quaternion position = q1 * q2.Conjugate();

	return D3DXVECTOR3(position.i, position.j, position.k);
}

void Quaternion::Normalise()
{
	// Don't normalize if we don't have to
	float mag = Quaternion::Magnitude();
	if (  mag!=0.f && (fabs(mag - 1.0f) > TOLERANCE)) //Tolerance defined
	{
		float magsqrt = sqrt(mag);
		w /= magsqrt;
		i /= magsqrt;
		j /= magsqrt;
		k /= magsqrt;
	}
}

// Initialize to an infinitely small bounding box.
AABB::AABB()
	: minPt(INFINITY, INFINITY, INFINITY),
	  maxPt(-INFINITY, -INFINITY, -INFINITY){}

AABB::AABB(D3DXVECTOR3 v3Min, D3DXVECTOR3 v3Max) 
	: minPt(v3Min),
	  maxPt(v3Max){}
D3DXVECTOR3 AABB::center()const
{
	return (minPt+maxPt)*0.5f;
}

D3DXVECTOR3 AABB::extent()const
{
	return (maxPt-minPt)*0.5f;
}

void AABB::xform(const D3DXMATRIX& M, AABB& out)
{
	// Convert to center/extent representation.
	D3DXVECTOR3 c = center();
	D3DXVECTOR3 e = extent();

	// Transform center in usual way.
	D3DXVec3TransformCoord(&c, &c, &M);

	// Transform extent.
	D3DXMATRIX absM;
	D3DXMatrixIdentity(&absM);
	absM(0,0) = fabsf(M(0,0)); absM(0,1) = fabsf(M(0,1)); absM(0,2) = fabsf(M(0,2));
	absM(1,0) = fabsf(M(1,0)); absM(1,1) = fabsf(M(1,1)); absM(1,2) = fabsf(M(1,2));
	absM(2,0) = fabsf(M(2,0)); absM(2,1) = fabsf(M(2,1)); absM(2,2) = fabsf(M(2,2));
	D3DXVec3TransformNormal(&e, &e, &absM);

	// Convert back to AABB representation.
	out.minPt = c - e;
	out.maxPt = c + e;
}

//---------------------------------------------------------------------------
// AABB3::rayIntersect
//
// Parametric intersection with a ray.  Returns parametric point
// of intersection in range 0...1 or a really big number (>1) if no
// intersection.
//
// From "Fast Ray-Box Intersection," by Woo in Graphics Gems I,
// page 395.
//
// See 12.9.11

float	AABB::rayIntersect(	const D3DXVECTOR3 &rayOrg,		// orgin of the ray
							const D3DXVECTOR3 &rayDelta,	// length and direction of the ray
							D3DXVECTOR3 *returnNormal,		// optionally, the normal is returned
							float *checkT
							) const {

		// We'll return this huge number if no intersection

		const float kNoIntersection = 1e30f;

		// Check for point inside box, trivial reject, and determine parametric
		// distance to each front face

		bool inside = true;

		float xt, xn;
		if (rayOrg.x < minPt.x) 
		{
			xt = minPt.x - rayOrg.x; // get distance & direction from Org to point
			if (xt > rayDelta.x) return kNoIntersection;  // distance is longer than ray
			xt /= rayDelta.x;
			inside = false;
			xn = -1.0f;
		} else if (rayOrg.x > maxPt.x) 
		{
			xt = maxPt.x - rayOrg.x;
			if (xt < rayDelta.x) return kNoIntersection;
			xt /= rayDelta.x;
			inside = false;
			xn = 1.0f;
		} else {
			xt = -1.0f;
		}

		float yt, yn;
		if (rayOrg.y < minPt.y) 
		{
			yt = minPt.y - rayOrg.y;
			if (yt > rayDelta.y) return kNoIntersection;
			yt /= rayDelta.y;
			inside = false;
			yn = -1.0f;
		} else if (rayOrg.y > maxPt.y) 
		{
			yt = maxPt.y - rayOrg.y;
			if (yt < rayDelta.y) return kNoIntersection;
			yt /= rayDelta.y;
			inside = false;
			yn = 1.0f;
		} else {
			yt = -1.0f;
		}

		float zt, zn;
		if (rayOrg.z < minPt.z) 
		{
			zt = minPt.z - rayOrg.z;
			if (zt > rayDelta.z) return kNoIntersection;
			zt /= rayDelta.z;
			inside = false;
			zn = -1.0f;
		} else if (rayOrg.z > maxPt.z) 
		{
			zt = maxPt.z - rayOrg.z;
			if (zt < rayDelta.z) return kNoIntersection;
			zt /= rayDelta.z;
			inside = false;
			zn = 1.0f;
		} else {
			zt = -1.0f;
		}

		// Inside box?

		if (inside) 
		{
			if (returnNormal != NULL) 
			{
				*returnNormal = -rayDelta;
				D3DXVec3Normalize(returnNormal,returnNormal);
			}
			return 0.0f;
		}

		// Select farthest plane - this is
		// the plane of intersection.

		int which = 0;
		float t = xt;
		if (yt > t) 
		{
			which = 1;
			t = yt;
		}
		if (zt > t) 
		{
			which = 2;
			t = zt;
		}

		if (checkT != NULL) *checkT = t;

		switch (which) {

		case 0: // intersect with yz plane
			{
				float y = rayOrg.y + rayDelta.y*t;
				if (y < minPt.y || y > maxPt.y) return 2.1;
				float z = rayOrg.z + rayDelta.z*t;
				if (z < minPt.z || z > maxPt.z) return 2;

				if (returnNormal != NULL) {
					returnNormal->x = xn;
					returnNormal->y = 0.0f;
					returnNormal->z = 0.0f;
				}

			} break;

		case 1: // intersect with xz plane
			{
				float x = rayOrg.x + rayDelta.x*t;
				if (x < minPt.x || x > maxPt.x) return 2.1;
				float z = rayOrg.z + rayDelta.z*t;
				if (z < minPt.z || z > maxPt.z) return 2;

				if (returnNormal != NULL) {
					returnNormal->x = 0.0f;
					returnNormal->y = yn;
					returnNormal->z = 0.0f;
				}

			} break;

		case 2: // intersect with xy plane
			{
				float x = rayOrg.x + rayDelta.x*t;
				if (x < minPt.x || x > maxPt.x) return 2.1;
				float y = rayOrg.y + rayDelta.y*t;
				if (y < minPt.y || y > maxPt.y) return 2;

				if (returnNormal != NULL) {
					returnNormal->x = 0.0f;
					returnNormal->y = 0.0f;
					returnNormal->z = zn;
				}

			} break;
		}

		// Return parametric point of intersection

		return t;

}

float vec3Magnitude(D3DXVECTOR3 position1, D3DXVECTOR3 position2)
{
	D3DXVECTOR3 tmpVec = position1 - position2;
	return sqrt(pow(tmpVec.x, 2) + pow(tmpVec.y, 2) + pow(tmpVec.z, 2));
}

float vec4Magnitude(D3DXVECTOR4 position1, D3DXVECTOR4 position2)
{
	D3DXVECTOR4 tmpVec = position1 - position2;
	return sqrt(pow(tmpVec.x, 2) + pow(tmpVec.y, 2) + pow(tmpVec.z, 2));
}

// ******************************************
// Ray / Axis-Aligned Bounding Box Overlap 
// ******************************************

void make_ray(D3DXVECTOR3 Origin, D3DXVECTOR3 Direction, RayAABB *r)
{
	//common variables
	r->x = Origin.x;
	r->y = Origin.y;
	r->z = Origin.z;
	r->i = Direction.x;
	r->j = Direction.y;
	r->k = Direction.z;

	r->ii = 1.0f/Direction.x;
	r->ij = 1.0f/Direction.y;
	r->ik = 1.0f/Direction.z;

	//ray slope
	r->ibyj = r->i * r->ij;
	r->jbyi = r->j * r->ii;
	r->jbyk = r->j * r->ik;
	r->kbyj = r->k * r->ij;
	r->ibyk = r->i * r->ik;
	r->kbyi = r->k * r->ii;
	r->c_xy = r->y - r->jbyi * r->x;
	r->c_xz = r->z - r->kbyi * r->x;
	r->c_yx = r->x - r->ibyj * r->y;
	r->c_yz = r->z - r->kbyj * r->y;
	r->c_zx = r->x - r->ibyk * r->z;
	r->c_zy = r->y - r->jbyk * r->z;	

	//ray slope classification
	if(Direction.x < 0)
	{
		if(Direction.y < 0)
		{
			if(Direction.z < 0)
			{
				r->classification = MMM;
			}
			else if(Direction.z > 0){
				r->classification = MMP;
			}
			else//(Direction.z >= 0)
			{
				r->classification = MMO;
			}
		}
		else//(Direction.y >= 0)
		{
			if(Direction.z < 0)
			{
				r->classification = MPM;
				if(Direction.y==0)
					r->classification = MOM;
			}
			else//(Direction.z >= 0)
			{
				if((Direction.y==0) && (Direction.z==0))
					r->classification = MOO;	
				else if(Direction.z==0)
					r->classification = MPO;
				else if(Direction.y==0)
					r->classification = MOP;
				else
					r->classification = MPP;
			}
		}
	}
	else//(Direction.x >= 0)
	{
		if(Direction.y < 0)
		{
			if(Direction.z < 0)
			{
				r->classification = PMM;
				if(Direction.x==0)
					r->classification = OMM;
			}
			else//(Direction.z >= 0)
			{				
				if((Direction.x==0) && (Direction.z==0))
					r->classification = OMO;
				else if(Direction.z==0)
					r->classification = PMO;
				else if(Direction.x==0)
					r->classification = OMP;
				else
					r->classification = PMP;
			}
		}
		else//(Direction.y >= 0)
		{
			if(Direction.z < 0)
			{
				if((Direction.x==0) && (Direction.y==0))
					r->classification = OOM;
				else if(Direction.x==0)
					r->classification = OPM;
				else if(Direction.y==0)
					r->classification = POM;
				else
					r->classification = PPM;
			}
			else//(Direction.z > 0)
			{
				if(Direction.x==0)
				{
					if(Direction.y==0)
						r->classification = OOP;
					else if(Direction.z==0)
						r->classification = OPO;
					else
						r->classification = OPP;
				}
				else
				{
					if((Direction.y==0) && (Direction.z==0))
						r->classification = POO;
					else if(Direction.y==0)
						r->classification = POP;
					else if(Direction.z==0)
						r->classification = PPO;
					else
						r->classification = PPP;
				}
			}			
		}
	}
}

bool slope(RayAABB *r, AABB *b){

	switch (r->classification)
	{
	case MMM:

		if ((r->x < b->minPt.x) || (r->y < b->minPt.y) || (r->z < b->minPt.z)
			|| (r->jbyi * b->minPt.x - b->maxPt.y + r->c_xy > 0)
			|| (r->ibyj * b->minPt.y - b->maxPt.x + r->c_yx > 0)
			|| (r->jbyk * b->minPt.z - b->maxPt.y + r->c_zy > 0)
			|| (r->kbyj * b->minPt.y - b->maxPt.z + r->c_yz > 0)
			|| (r->kbyi * b->minPt.x - b->maxPt.z + r->c_xz > 0)
			|| (r->ibyk * b->minPt.z - b->maxPt.x + r->c_zx > 0)
			)
			return false;

		return true;

	case MMP:

		if ((r->x < b->minPt.x) || (r->y < b->minPt.y) || (r->z > b->maxPt.z)
			|| (r->jbyi * b->minPt.x - b->maxPt.y + r->c_xy > 0)
			|| (r->ibyj * b->minPt.y - b->maxPt.x + r->c_yx > 0)
			|| (r->jbyk * b->maxPt.z - b->maxPt.y + r->c_zy > 0)
			|| (r->kbyj * b->minPt.y - b->minPt.z + r->c_yz < 0)
			|| (r->kbyi * b->minPt.x - b->minPt.z + r->c_xz < 0)
			|| (r->ibyk * b->maxPt.z - b->maxPt.x + r->c_zx > 0)
			)
			return false;

		return true;

	case MPM:

		if ((r->x < b->minPt.x) || (r->y > b->maxPt.y) || (r->z < b->minPt.z)
			|| (r->jbyi * b->minPt.x - b->minPt.y + r->c_xy < 0) 
			|| (r->ibyj * b->maxPt.y - b->maxPt.x + r->c_yx > 0)
			|| (r->jbyk * b->minPt.z - b->minPt.y + r->c_zy < 0) 
			|| (r->kbyj * b->maxPt.y - b->maxPt.z + r->c_yz > 0)
			|| (r->kbyi * b->minPt.x - b->maxPt.z + r->c_xz > 0)
			|| (r->ibyk * b->minPt.z - b->maxPt.x + r->c_zx > 0)
			)
			return false;

		return true;

	case MPP:

		if ((r->x < b->minPt.x) || (r->y > b->maxPt.y) || (r->z > b->maxPt.z)
			|| (r->jbyi * b->minPt.x - b->minPt.y + r->c_xy < 0) 
			|| (r->ibyj * b->maxPt.y - b->maxPt.x + r->c_yx > 0)
			|| (r->jbyk * b->maxPt.z - b->minPt.y + r->c_zy < 0)
			|| (r->kbyj * b->maxPt.y - b->minPt.z + r->c_yz < 0)
			|| (r->kbyi * b->minPt.x - b->minPt.z + r->c_xz < 0)
			|| (r->ibyk * b->maxPt.z - b->maxPt.x + r->c_zx > 0)
			)
			return false;

		return true;

	case PMM:

		if ((r->x > b->maxPt.x) || (r->y < b->minPt.y) || (r->z < b->minPt.z)
			|| (r->jbyi * b->maxPt.x - b->maxPt.y + r->c_xy > 0)
			|| (r->ibyj * b->minPt.y - b->minPt.x + r->c_yx < 0)
			|| (r->jbyk * b->minPt.z - b->maxPt.y + r->c_zy > 0)
			|| (r->kbyj * b->minPt.y - b->maxPt.z + r->c_yz > 0)
			|| (r->kbyi * b->maxPt.x - b->maxPt.z + r->c_xz > 0)
			|| (r->ibyk * b->minPt.z - b->minPt.x + r->c_zx < 0)
			)
			return false;

		return true;

	case PMP:

		if ((r->x > b->maxPt.x) || (r->y < b->minPt.y) || (r->z > b->maxPt.z)
			|| (r->jbyi * b->maxPt.x - b->maxPt.y + r->c_xy > 0)
			|| (r->ibyj * b->minPt.y - b->minPt.x + r->c_yx < 0)
			|| (r->jbyk * b->maxPt.z - b->maxPt.y + r->c_zy > 0)
			|| (r->kbyj * b->minPt.y - b->minPt.z + r->c_yz < 0)
			|| (r->kbyi * b->maxPt.x - b->minPt.z + r->c_xz < 0)
			|| (r->ibyk * b->maxPt.z - b->minPt.x + r->c_zx < 0)
			)
			return false;

		return true;

	case PPM:

		if ((r->x > b->maxPt.x) || (r->y > b->maxPt.y) || (r->z < b->minPt.z)
			|| (r->jbyi * b->maxPt.x - b->minPt.y + r->c_xy < 0)
			|| (r->ibyj * b->maxPt.y - b->minPt.x + r->c_yx < 0)
			|| (r->jbyk * b->minPt.z - b->minPt.y + r->c_zy < 0) 
			|| (r->kbyj * b->maxPt.y - b->maxPt.z + r->c_yz > 0)
			|| (r->kbyi * b->maxPt.x - b->maxPt.z + r->c_xz > 0)
			|| (r->ibyk * b->minPt.z - b->minPt.x + r->c_zx < 0)
			)
			return false;

		return true;

	case PPP:

		if ((r->x > b->maxPt.x) || (r->y > b->maxPt.y) || (r->z > b->maxPt.z)
			|| (r->jbyi * b->maxPt.x - b->minPt.y + r->c_xy < 0)
			|| (r->ibyj * b->maxPt.y - b->minPt.x + r->c_yx < 0)
			|| (r->jbyk * b->maxPt.z - b->minPt.y + r->c_zy < 0)
			|| (r->kbyj * b->maxPt.y - b->minPt.z + r->c_yz < 0)
			|| (r->kbyi * b->maxPt.x - b->minPt.z + r->c_xz < 0)
			|| (r->ibyk * b->maxPt.z - b->minPt.x + r->c_zx < 0)
			)
			return false;

		return true;

	case OMM:

		if((r->x < b->minPt.x) || (r->x > b->maxPt.x)
			|| (r->y < b->minPt.y) || (r->z < b->minPt.z)
			|| (r->jbyk * b->minPt.z - b->maxPt.y + r->c_zy > 0)
			|| (r->kbyj * b->minPt.y - b->maxPt.z + r->c_yz > 0)
			)
			return false;

		return true;

	case OMP:

		if((r->x < b->minPt.x) || (r->x > b->maxPt.x)
			|| (r->y < b->minPt.y) || (r->z > b->maxPt.z)
			|| (r->jbyk * b->maxPt.z - b->maxPt.y + r->c_zy > 0)
			|| (r->kbyj * b->minPt.y - b->minPt.z + r->c_yz < 0)
			)
			return false;

		return true;

	case OPM:

		if((r->x < b->minPt.x) || (r->x > b->maxPt.x)
			|| (r->y > b->maxPt.y) || (r->z < b->minPt.z)
			|| (r->jbyk * b->minPt.z - b->minPt.y + r->c_zy < 0) 
			|| (r->kbyj * b->maxPt.y - b->maxPt.z + r->c_yz > 0)
			)
			return false;

		return true;

	case OPP:

		if((r->x < b->minPt.x) || (r->x > b->maxPt.x)
			|| (r->y > b->maxPt.y) || (r->z > b->maxPt.z)
			|| (r->jbyk * b->maxPt.z - b->minPt.y + r->c_zy < 0)
			|| (r->kbyj * b->maxPt.y - b->minPt.z + r->c_yz < 0)
			)
			return false;

		return true;

	case MOM:

		if((r->y < b->minPt.y) || (r->y > b->maxPt.y)
			|| (r->x < b->minPt.x) || (r->z < b->minPt.z) 
			|| (r->kbyi * b->minPt.x - b->maxPt.z + r->c_xz > 0)
			|| (r->ibyk * b->minPt.z - b->maxPt.x + r->c_zx > 0)
			)
			return false;

		return true;

	case MOP:

		if((r->y < b->minPt.y) || (r->y > b->maxPt.y)
			|| (r->x < b->minPt.x) || (r->z > b->maxPt.z) 
			|| (r->kbyi * b->minPt.x - b->minPt.z + r->c_xz < 0)
			|| (r->ibyk * b->maxPt.z - b->maxPt.x + r->c_zx > 0)
			)
			return false;

		return true;

	case POM:

		if((r->y < b->minPt.y) || (r->y > b->maxPt.y)
			|| (r->x > b->maxPt.x) || (r->z < b->minPt.z)
			|| (r->kbyi * b->maxPt.x - b->maxPt.z + r->c_xz > 0)
			|| (r->ibyk * b->minPt.z - b->minPt.x + r->c_zx < 0)
			)
			return false;

		return true;

	case POP:

		if((r->y < b->minPt.y) || (r->y > b->maxPt.y)
			|| (r->x > b->maxPt.x) || (r->z > b->maxPt.z)
			|| (r->kbyi * b->maxPt.x - b->minPt.z + r->c_xz < 0)
			|| (r->ibyk * b->maxPt.z - b->minPt.x + r->c_zx < 0)
			)
			return false;

		return true;

	case MMO:

		if((r->z < b->minPt.z) || (r->z > b->maxPt.z)
			|| (r->x < b->minPt.x) || (r->y < b->minPt.y) 
			|| (r->jbyi * b->minPt.x - b->maxPt.y + r->c_xy > 0)
			|| (r->ibyj * b->minPt.y - b->maxPt.x + r->c_yx > 0)
			)
			return false;

		return true;

	case MPO:

		if((r->z < b->minPt.z) || (r->z > b->maxPt.z)
			|| (r->x < b->minPt.x) || (r->y > b->maxPt.y) 
			|| (r->jbyi * b->minPt.x - b->minPt.y + r->c_xy < 0) 
			|| (r->ibyj * b->maxPt.y - b->maxPt.x + r->c_yx > 0)
			)
			return false;

		return true;

	case PMO:

		if((r->z < b->minPt.z) || (r->z > b->maxPt.z)
			|| (r->x > b->maxPt.x) || (r->y < b->minPt.y) 
			|| (r->jbyi * b->maxPt.x - b->maxPt.y + r->c_xy > 0)
			|| (r->ibyj * b->minPt.y - b->minPt.x + r->c_yx < 0)  
			)
			return false;

		return true;

	case PPO:

		if((r->z < b->minPt.z) || (r->z > b->maxPt.z)
			|| (r->x > b->maxPt.x) || (r->y > b->maxPt.y)
			|| (r->jbyi * b->maxPt.x - b->minPt.y + r->c_xy < 0)
			|| (r->ibyj * b->maxPt.y - b->minPt.x + r->c_yx < 0)
			)
			return false;

		return true;

	case MOO:

		if((r->x < b->minPt.x)
			|| (r->y < b->minPt.y) || (r->y > b->maxPt.y)
			|| (r->z < b->minPt.z) || (r->z > b->maxPt.z)
			)
			return false;

		return true;

	case POO:

		if((r->x > b->maxPt.x)
			|| (r->y < b->minPt.y) || (r->y > b->maxPt.y)
			|| (r->z < b->minPt.z) || (r->z > b->maxPt.z)
			)
			return false;

		return true;

	case OMO:

		if((r->y < b->minPt.y)
			|| (r->x < b->minPt.x) || (r->x > b->maxPt.x)
			|| (r->z < b->minPt.z) || (r->z > b->maxPt.z)
			)
			return false;

	case OPO:

		if((r->y > b->maxPt.y)
			|| (r->x < b->minPt.x) || (r->x > b->maxPt.x)
			|| (r->z < b->minPt.z) || (r->z > b->maxPt.z)
			)
			return false;

	case OOM:

		if((r->z < b->minPt.z)
			|| (r->x < b->minPt.x) || (r->x > b->maxPt.x)
			|| (r->y < b->minPt.y) || (r->y > b->maxPt.y)
			)
			return false;

	case OOP:

		if((r->z > b->maxPt.z)
			|| (r->x < b->minPt.x) || (r->x > b->maxPt.x)
			|| (r->y < b->minPt.y) || (r->y > b->maxPt.y)
			)
			return false;

		return true;

	}

	return false;
}