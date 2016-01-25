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