#ifndef _LC_MATH_H_
#define _LC_MATH_H_

#include <math.h>
#include <float.h>

#define LC_DTOR 0.017453f
#define LC_RTOD 57.29578f
#define LC_PI   3.141592f
#define LC_2PI  6.283185f

#define LC_RGB(r,g,b) LC_RGBA(r,g,b,255)
#define LC_RGBA(r,g,b,a) ((uint32_t)(((uint8_t) (r) | ((uint16_t) (g) << 8)) | (((uint32_t) (uint8_t) (b)) << 16) | (((uint32_t) (uint8_t) (a)) << 24)))
#define LC_RGBA_RED(rgba)   ((uint8_t)(((rgba) >>  0) & 0xff))
#define LC_RGBA_GREEN(rgba) ((uint8_t)(((rgba) >>  8) & 0xff))
#define LC_RGBA_BLUE(rgba)  ((uint8_t)(((rgba) >> 16) & 0xff))
#define LC_RGBA_ALPHA(rgba) ((uint8_t)(((rgba) >> 24) & 0xff))
#define LC_FLOATRGB(f) LC_RGB(f[0]*255, f[1]*255, f[2]*255)

template<typename T>
inline T lcMin(const T& a, const T& b)
{
	return a < b ? a : b;
}

template<typename T>
inline T lcMax(const T& a, const T& b)
{
	return a > b ? a : b;
}

template<typename T>
inline T lcClamp(const T& Value, const T& Min, const T& Max)
{
	if (Value > Max)
		return Max;
	else if (Value < Min)
		return Min;
	else
		return Value;
}

class Vector2
{
public:
	Vector2()
	{
	}

	Vector2(const float _x, const float _y)
		: x(_x), y(_y)
	{
	}

	Vector2(const Vector2& a)
		: x(a.x), y(a.y)
	{
	}

	operator const float*() const
	{
		return (const float*)this;
	}
	
	operator float*()
	{
		return (float*)this;
	}
	
	const float& operator[](int i) const
	{
		return ((float*)this)[i];
	}

	float& operator[](int i)
	{
		return ((float*)this)[i];
	}

	float x, y;
};
/*
template <class T = float>
class Vector3T {
public:
    Vector3T() { }
    
    Vector3T(const T _x, const T _y, const T _z) : x(_x), y(_y), z(_z) { }
    
    operator const T*() const
    {
        return (const float*)this;
    }
    
    operator T*()
    {
        return v;
    }
    
    const float& operator[](int i) const
    {
        return v[i];
    }
    
    float& operator[](int i)
    {
        return v[i];
    }
    
    union {
        T v[3];
        struct {
            T x, y, z;
        };
    };
};

class Vector3 : public Vector3T<float>
{
public:
    Vector3() {}
    
    Vector3(const float _x, const float _y, const float _z) : Vector3T<float>(_x, _y, _z) {}
};
 */

class Vector3
{
public:
	Vector3()
	{
	}

	Vector3(const float _x, const float _y, const float _z)
		: x(_x), y(_y), z(_z)
	{
	}

	operator const float*() const
	{
		return (const float*)this;
	}
	
	operator float*()
	{
		return (float*)this;
	}
	
	const float& operator[](int i) const
	{
		return ((float*)this)[i];
	}

	float& operator[](int i)
	{
		return ((float*)this)[i];
	}

	void Normalize();
	float Length() const;
	float LengthSquared() const;

    float x, y, z;
};

template <class T = float>
class Vector4T {
public:
    Vector4T()
    {
    }
    
    Vector4T(const T _x, const T _y, const T _z, const T _w)
    : x(_x), y(_y), z(_z), w(_w)
    {
    }
    
    operator const T*() const
    {
        return (const float*)this;
    }
    
    operator T*()
    {
        return v;
    }
    
    const float& operator[](int i) const
    {
        return v[i];
    }
    
    float& operator[](int i)
    {
        return v[i];
    }

    union {
        T v[4];
        struct {
            T x, y, z, w;
        };
    };
};

template <typename T>
inline bool operator==(const Vector4T<T>& a, const Vector4T<T>& b)
{
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z) && (a.w == b.z);
}

template <typename T>
inline bool operator!=(const Vector4T<T>& a, const Vector4T<T>& b)
{
    return (a.x != b.x) || (a.y != b.y) || (a.z != b.z) || (a.w != b.w);
}

class Vector4 : public Vector4T<float>
{
public:
	Vector4() {}

    Vector4(const float _x, const float _y, const float _z, const float _w) : Vector4T<float>(_x, _y, _z, _w) {}

    Vector4(const Vector3& _xyz, const float _w) : Vector4T<float>(_xyz.x, _xyz.y, _xyz.z, _w) {}
};

class Vector4U8 : public Vector4T<uint8_t> {
public:
    Vector4U8() { }

    Vector4U8(const uint8_t _x, const uint8_t _y, const uint8_t _z, const uint8_t _w) : Vector4T<uint8_t>(_x, _y, _z, _w) {}

    Vector4U8(uint32_t Color) : Vector4T<uint8_t>(LC_RGBA_RED(Color),
                                                  LC_RGBA_GREEN(Color), LC_RGBA_BLUE(Color), LC_RGBA_ALPHA(Color))
    {
    }

};
class Matrix44;
class Matrix33
{
public:
	Matrix33()
	{
	}

	Matrix33(const Vector3& _x, const Vector3& _y, const Vector3& _z)
	{
		r[0] = _x;
		r[1] = _y;
		r[2] = _z;
	}

	explicit Matrix33(const Matrix44& Matrix);

	operator const float*() const
	{
		return (const float*)this;
	}
	
	operator float*()
	{
		return (float*)this;
	}
	
	const Vector3& operator[](int i) const
	{
		return r[i];
	}

	Vector3& operator[](int i)
	{
		return r[i];
	}

	void Orthonormalize();

	Vector3 r[3];
};

class Matrix44
{
public:
	Matrix44()
	{
	}

	Matrix44(const Vector4& _x, const Vector4& _y, const Vector4& _z, const Vector4& _w)
	{
		r[0] = _x;
		r[1] = _y;
		r[2] = _z;
		r[3] = _w;
	}

	Matrix44(const Matrix33& Rotation, const Vector3& Translation)
	{
		r[0] = Vector4(Rotation[0][0], Rotation[0][1], Rotation[0][2], 0.0f);
		r[1] = Vector4(Rotation[1][0], Rotation[1][1], Rotation[1][2], 0.0f);
		r[2] = Vector4(Rotation[2][0], Rotation[2][1], Rotation[2][2], 0.0f);
		r[3] = Vector4(Translation, 1.0f);
	}

	Vector3 GetTranslation() const
	{
		return Vector3(r[3][0], r[3][1], r[3][2]);
	}

	void SetTranslation(const Vector3& Translation)
	{
		r[3] = Vector4(Translation[0], Translation[1], Translation[2], 1.0f);
	}

	operator const float*() const
	{
		return (const float*)this;
	}
	
	operator float*()
	{
		return (float*)this;
	}
	
	const Vector4& operator[](int i) const
	{
		return r[i];
	}

	Vector4& operator[](int i)
	{
		return r[i];
	}

	Vector4 r[4];
};

inline Vector3 operator+(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
}

inline Vector3 operator-(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
}

inline Vector3 operator*(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline Vector3 operator/(const Vector3& a, const Vector3& b)
{
	return Vector3(a.x / b.x, a.y / b.y, a.z / b.z);
}

inline Vector3 operator*(const Vector3& a, float b)
{
	return Vector3(a.x * b, a.y * b, a.z * b);
}

inline Vector3 operator/(const Vector3& a, float b)
{
	return Vector3(a.x / b, a.y / b, a.z / b);
}

inline Vector3 operator*(float a, const Vector3& b)
{
	return Vector3(b.x * a, b.y * a, b.z * a);
}

inline Vector3 operator/(float a, const Vector3& b)
{
	return Vector3(b.x / a, b.y / a, b.z / a);
}

inline Vector3 operator-(const Vector3& a)
{
	return Vector3(-a.x, -a.y, -a.z);
}

inline Vector3& operator+=(Vector3& a, const Vector3& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;

	return a;
}

inline Vector3& operator-=(Vector3& a, const Vector3& b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;

	return a;
}

inline Vector3& operator*=(Vector3& a, const Vector3& b)
{
	a.x *= b.x;
	a.y *= b.y;
	a.z *= b.z;

	return a;
}

inline Vector3& operator/=(Vector3& a, const Vector3& b)
{
	a.x /= b.x;
	a.y /= b.y;
	a.z /= b.z;

	return a;
}

inline Vector3& operator*=(Vector3& a, float b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;

	return a;
}

inline Vector3& operator/=(Vector3& a, float b)
{
	a.x /= b;
	a.y /= b;
	a.z /= b;

	return a;
}

#define DIFF_EPSILON 0.0001

inline bool operator==(const Vector3& a, const Vector3& b)
{
//	return (fabs(a.x - b.x) < DIFF_EPSILON) && (fabs(a.y - b.y) < DIFF_EPSILON) && (fabs(a.z - b.z) < DIFF_EPSILON);
    return (a.x == b.x) && (a.y == b.y) && (a.z == b.z);
}

inline bool operator!=(const Vector3& a, const Vector3& b)
{
//    return (fabs(a.x - b.x) > DIFF_EPSILON) || (fabs(a.y - b.y) > DIFF_EPSILON) || (fabs(a.z - b.z) > DIFF_EPSILON);
    return (a.x != b.x) || (a.y != b.y) || (a.z != b.z);
}

inline void Vector3::Normalize()
{
	float InvLength = 1.0f / Length();

	x *= InvLength;
	y *= InvLength;
	z *= InvLength;
}

inline float Vector3::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}

inline float Vector3::LengthSquared() const
{
	return x * x + y * y + z * z;
}

inline float lcLength(const Vector3& a)
{
	return a.Length();
}

inline float lcLengthSquared(const Vector3& a)
{
	return a.LengthSquared();
}

inline Vector3 lcNormalize(const Vector3& a)
{
	Vector3 Ret(a);
	Ret.Normalize();
	return Ret;
}

inline void lcAlign(Vector3& t, const Vector3& a, const Vector3& b)
{
	Vector3 Vector(b - a);
	Vector.Normalize();
	Vector *=  (t - a).Length();
	t = a + Vector;
}

inline float lcDot(const Vector3& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float lcDot3(const Vector4& a, const Vector3& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float lcDot3(const Vector3& a, const Vector4& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float lcDot3(const Vector4& a, const Vector4& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline float lcDot(const Vector4& a, const Vector4& b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
}

inline Vector3 lcCross(const Vector3& a, const Vector3& b)
{
	return Vector3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x);
}

inline Vector4 operator+(const Vector4& a, const Vector4& b)
{
	return Vector4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

inline Vector4 operator-(const Vector4& a, const Vector4& b)
{
	return Vector4(a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w);
}

inline Vector4 operator*(const Vector4& a, float f)
{
	return Vector4(a.x * f, a.y * f, a.z * f, a.w * f);
}

inline Vector4 operator*(const Vector4& a, const Vector4& b)
{
	return Vector4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

inline Vector4 operator/(const Vector4& a, float f)
{
	return Vector4(a.x / f, a.y / f, a.z / f, a.w / f);
}

inline Vector4 operator/(const Vector4& a, const Vector4& b)
{
	return Vector4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

inline Vector4& operator+=(Vector4& a, const Vector4& b)
{
	a.x += b.x;
	a.y += b.y;
	a.z += b.z;
	a.w += b.w;

	return a;
}

inline Vector4& operator-=(Vector4& a, const Vector4& b)
{
	a.x -= b.x;
	a.y -= b.y;
	a.z -= b.z;
	a.w -= b.w;

	return a;
}

inline Vector4& operator*=(Vector4& a, float b)
{
	a.x *= b;
	a.y *= b;
	a.z *= b;
	a.w *= b;

	return a;
}

inline Vector4& operator/=(Vector4& a, float b)
{
	a.x /= b;
	a.y /= b;
	a.z /= b;
	a.w /= b;

	return a;
}

inline Vector3 Vector3FromColor(uint32_t Color)
{
	Vector3 v(LC_RGBA_RED(Color), LC_RGBA_GREEN(Color), LC_RGBA_BLUE(Color));
	v /= 255.0f;
	return v;
}

inline Vector4 Vector4FromColor(uint32_t Color)
{
	Vector4 v(LC_RGBA_RED(Color), LC_RGBA_GREEN(Color), LC_RGBA_BLUE(Color), LC_RGBA_ALPHA(Color));
	v /= 255.0f;
	return v;
}

inline uint32_t lcColorFromVector3(const Vector3& Color)
{
	return LC_RGB(Color[0] * 255, Color[1] * 255, Color[2] * 255);
}

inline Vector3 lcMul(const Vector3& a, const Matrix33& b)
{
	return b.r[0] * a[0] + b.r[1] * a[1] + b.r[2] * a[2];
}

inline Vector3 lcMul31(const Vector3& a, const Matrix44& b)
{
	Vector4 v = b.r[0] * a[0] + b.r[1] * a[1] + b.r[2] * a[2] + b.r[3];

	return Vector3(v[0], v[1], v[2]);
}

inline Vector3 lcMul31(const Vector4& a, const Matrix44& b)
{
	Vector4 v = b.r[0] * a[0] + b.r[1] * a[1] + b.r[2] * a[2] + b.r[3];

	return Vector3(v[0], v[1], v[2]);
}

inline Vector3 lcMul30(const Vector3& a, const Matrix44& b)
{
	Vector4 v = b.r[0] * a[0] + b.r[1] * a[1] + b.r[2] * a[2];

	return Vector3(v[0], v[1], v[2]);
}

inline Vector3 lcMul30(const Vector4& a, const Matrix44& b)
{
	Vector4 v = b.r[0] * a[0] + b.r[1] * a[1] + b.r[2] * a[2];

	return Vector3(v[0], v[1], v[2]);
}

inline Vector4 lcMul4(const Vector4& a, const Matrix44& b)
{
	return b.r[0] * a[0] + b.r[1] * a[1] + b.r[2] * a[2] + b.r[3] * a[3];
}

//
// multiplication order is _SWAPPED_, to make it behave like GLKMatrix4Multiply
//
inline Matrix33 lcMul(const Matrix33& b, const Matrix33& a)
{
	Vector3 Col0(b.r[0][0], b.r[1][0], b.r[2][0]);
	Vector3 Col1(b.r[0][1], b.r[1][1], b.r[2][1]);
	Vector3 Col2(b.r[0][2], b.r[1][2], b.r[2][2]);

	Vector3 Ret0(lcDot(a.r[0], Col0), lcDot(a.r[0], Col1), lcDot(a.r[0], Col2));
	Vector3 Ret1(lcDot(a.r[1], Col0), lcDot(a.r[1], Col1), lcDot(a.r[1], Col2));
	Vector3 Ret2(lcDot(a.r[2], Col0), lcDot(a.r[2], Col1), lcDot(a.r[2], Col2));

	return Matrix33(Ret0, Ret1, Ret2);
}

inline Matrix44 lcMul(const Matrix44& b, const Matrix44& a)
{
	Matrix44 Result;

	Result.r[0] = b.r[0] * a[0].x + b.r[1] * a[0].y + b.r[2] * a[0].z + b.r[3] * a[0].w;
	Result.r[1] = b.r[0] * a[1].x + b.r[1] * a[1].y + b.r[2] * a[1].z + b.r[3] * a[1].w;
	Result.r[2] = b.r[0] * a[2].x + b.r[1] * a[2].y + b.r[2] * a[2].z + b.r[3] * a[2].w;
	Result.r[3] = b.r[0] * a[3].x + b.r[1] * a[3].y + b.r[2] * a[3].z + b.r[3] * a[3].w;

	return Result;
}

inline Matrix33::Matrix33(const Matrix44& Matrix)
{
	r[0] = Vector3(Matrix.r[0].x, Matrix.r[0].y, Matrix.r[0].z);
	r[1] = Vector3(Matrix.r[1].x, Matrix.r[1].y, Matrix.r[1].z);
	r[2] = Vector3(Matrix.r[2].x, Matrix.r[2].y, Matrix.r[2].z);
}

inline void Matrix33::Orthonormalize()
{
	r[0] = lcNormalize(r[0]);
	r[1] = lcNormalize(r[1] - lcDot(r[1], r[0]) * r[0]);
	r[2] = r[2] - lcDot(r[2], r[0]) * r[0];
	r[2] -= lcDot(r[2], r[1]) * r[1];
	r[2] = lcNormalize(r[2]);
}

inline Matrix33 lcMatrix33Identity()
{
	Matrix33 m;

	m.r[0] = Vector3(1.0f, 0.0f, 0.0f);
	m.r[1] = Vector3(0.0f, 1.0f, 0.0f);
	m.r[2] = Vector3(0.0f, 0.0f, 1.0f);

	return m;
}

inline Matrix33 lcMatrix33RotationX(const float Radians)
{
	float s, c;

	s = sinf(Radians);
	c = cosf(Radians);

	Matrix33 m;

	m.r[0] = Vector3(1.0f, 0.0f, 0.0f);
	m.r[1] = Vector3(0.0f,    c,    s);
	m.r[2] = Vector3(0.0f,   -s,    c);

	return m;
}

inline Matrix33 lcMatrix33RotationY(const float Radians)
{
	float s, c;

	s = sinf(Radians);
	c = cosf(Radians);

	Matrix33 m;

	m.r[0] = Vector3(   c, 0.0f,   -s);
	m.r[1] = Vector3(0.0f, 1.0f, 0.0f);
	m.r[2] = Vector3(   s, 0.0f,    c);

	return m;
}

inline Matrix33 lcMatrix33RotationZ(const float Radians)
{
	float s, c;

	s = sinf(Radians);
	c = cosf(Radians);

	Matrix33 m;

	m.r[0] = Vector3(   c,    s, 0.0f);
	m.r[1] = Vector3(  -s,    c, 0.0f);
	m.r[2] = Vector3(0.0f, 0.0f, 1.0f);

	return m;
}

inline Matrix33 lcMatrix33FromAxisAngle(const Vector3& Axis, const float Radians)
{
	float s, c, mag, xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s = sinf(Radians);
	c = cosf(Radians);
	mag = Axis.Length();

	if (mag == 0.0f)
		return lcMatrix33Identity();

	Vector3 Normal = Axis * (1.0f / mag);

	xx = Normal[0] * Normal[0];
	yy = Normal[1] * Normal[1];
	zz = Normal[2] * Normal[2];
	xy = Normal[0] * Normal[1];
	yz = Normal[1] * Normal[2];
	zx = Normal[2] * Normal[0];
	xs = Normal[0] * s;
	ys = Normal[1] * s;
	zs = Normal[2] * s;
	one_c = 1.0f - c;

	Matrix33 m;

	m.r[0] = Vector3((one_c * xx) + c, (one_c * xy) + zs, (one_c * zx) - ys);
	m.r[1] = Vector3((one_c * xy) - zs, (one_c * yy) + c, (one_c * yz) + xs);
	m.r[2] = Vector3((one_c * zx) + ys, (one_c * yz) - xs, (one_c * zz) + c);

	return m;
}

inline Matrix33 lcMatrix33AffineInverse(const Matrix33& m)
{
	Matrix33 Inv;

	Inv.r[0] = Vector3(m.r[0][0], m.r[1][0], m.r[2][0]);
	Inv.r[1] = Vector3(m.r[0][1], m.r[1][1], m.r[2][1]);
	Inv.r[2] = Vector3(m.r[0][2], m.r[1][2], m.r[2][2]);

	return Inv;
}

inline Matrix33 lcMatrix33FromEulerAngles(const Vector3& Radians)
{
	float CosYaw, SinYaw, CosPitch, SinPitch, CosRoll, SinRoll;

	CosRoll = cosf(Radians[0]);
	SinRoll = sinf(Radians[0]);
	CosPitch = cosf(Radians[1]);
	SinPitch = sinf(Radians[1]);
	CosYaw = cosf(Radians[2]);
	SinYaw = sinf(Radians[2]);

	Matrix33 m;

	m.r[0] = Vector3(CosYaw * CosPitch, SinYaw * CosPitch, -SinPitch);
	m.r[1] = Vector3(CosYaw * SinPitch * SinRoll - SinYaw * CosRoll, CosYaw * CosRoll + SinYaw * SinPitch * SinRoll, CosPitch * SinRoll);
	m.r[2] = Vector3(CosYaw * SinPitch * CosRoll + SinYaw * SinRoll, SinYaw * SinPitch * CosRoll - CosYaw * SinRoll, CosPitch * CosRoll);

	return m;
}

inline Matrix44 Matrix44Identity()
{
	Matrix44 m;

	m.r[0] = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	m.r[1] = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	m.r[2] = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	m.r[3] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

	return m;
}

inline Matrix44 Matrix44Translation(const Vector3& Translation)
{
	Matrix44 m;

	m.r[0] = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	m.r[1] = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	m.r[2] = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	m.r[3] = Vector4(Translation[0], Translation[1], Translation[2], 1.0f);

	return m;
}

inline Matrix44 Matrix44RotationX(const float Radians)
{
	float s, c;

	s = sinf(Radians);
	c = cosf(Radians);

	Matrix44 m;

	m.r[0] = Vector4(1.0f, 0.0f, 0.0f, 0.0f);
	m.r[1] = Vector4(0.0f,    c,    s, 0.0f);
	m.r[2] = Vector4(0.0f,   -s,    c, 0.0f);
	m.r[3] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

	return m;
}

inline Matrix44 Matrix44RotationY(const float Radians)
{
	float s, c;

	s = sinf(Radians);
	c = cosf(Radians);

	Matrix44 m;

	m.r[0] = Vector4(   c, 0.0f,   -s, 0.0f);
	m.r[1] = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	m.r[2] = Vector4(   s, 0.0f,    c, 0.0f);
	m.r[3] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

	return m;
}

inline Matrix44 Matrix44RotationZ(const float Radians)
{
	float s, c;

	s = sinf(Radians);
	c = cosf(Radians);

	Matrix44 m;

	m.r[0] = Vector4(   c,    s, 0.0f, 0.0f);
	m.r[1] = Vector4(  -s,    c, 0.0f, 0.0f);
	m.r[2] = Vector4(0.0f, 0.0f, 1.0f, 0.0f);
	m.r[3] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

	return m;
}

inline Matrix44 Matrix44Scale(const Vector3& Scale)
{
	Matrix44 m;

	m.r[0] = Vector4(Scale.x, 0.0f, 0.0f, 0.0f);
	m.r[1] = Vector4(0.0f, Scale.y, 0.0f, 0.0f);
	m.r[2] = Vector4(0.0f, 0.0f, Scale.z, 0.0f);
	m.r[3] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

	return m;
}

//GLK_INLINE GLKMatrix4 GLKMatrix4MakeLookAt(float eyeX, float eyeY, float eyeZ,
//                                           float centerX, float centerY, float centerZ,
//                                           float upX, float upY, float upZ)
//{
//    GLKVector3 ev = { eyeX, eyeY, eyeZ };
//    GLKVector3 cv = { centerX, centerY, centerZ };
//    GLKVector3 uv = { upX, upY, upZ };
//    GLKVector3 n = GLKVector3Normalize(GLKVector3Add(ev, GLKVector3Negate(cv)));
//    GLKVector3 u = GLKVector3Normalize(GLKVector3CrossProduct(uv, n));
//    GLKVector3 v = GLKVector3CrossProduct(n, u);
//    
//    GLKMatrix4 m = { u.v[0], v.v[0], n.v[0], 0.0f,
//        u.v[1], v.v[1], n.v[1], 0.0f,
//        u.v[2], v.v[2], n.v[2], 0.0f,
//        GLKVector3DotProduct(GLKVector3Negate(u), ev),
//        GLKVector3DotProduct(GLKVector3Negate(v), ev),
//        GLKVector3DotProduct(GLKVector3Negate(n), ev),
//        1.0f };
//    
//    return m;
//}
inline Matrix44 Matrix44LookAt(const Vector3& Eye, const Vector3& Target, const Vector3& Up)
{
	Vector3 x, y, z;

	z = lcNormalize(Eye - Target);
	x = lcNormalize(lcCross(Up, z));
	y = lcNormalize(lcCross(z, x));

	Matrix44 m;

	m.r[0] = Vector4(x[0], y[0], z[0], 0.0f);
	m.r[1] = Vector4(x[1], y[1], z[1], 0.0f);
	m.r[2] = Vector4(x[2], y[2], z[2], 0.0f);
	m.r[3] = m.r[0] * -Eye[0] + m.r[1] * -Eye[1] + m.r[2] * -Eye[2];
	m.r[3][3] = 1.0f;

	return m;
}

//GLK_INLINE GLKMatrix4 GLKMatrix4MakePerspective(float fovyRadians, float aspect, float nearZ, float farZ)
//{
//    float cotan = 1.0f / tanf(fovyRadians / 2.0f);
//    
//    GLKMatrix4 m = { cotan / aspect, 0.0f, 0.0f, 0.0f,
//        0.0f, cotan, 0.0f, 0.0f,
//        0.0f, 0.0f, (farZ + nearZ) / (nearZ - farZ), -1.0f,
//        0.0f, 0.0f, (2.0f * farZ * nearZ) / (nearZ - farZ), 0.0f };
//    
//    return m;
//}
inline Matrix44 Matrix44Perspective(float fov, float aspect, float znear, float zfar)
{
    float cotan = 1.0f / tanf(fov / 2);
    Matrix44 m(Vector4(cotan / aspect, 0.0, 0.0, 0.0),
               Vector4(0.0, cotan, 0.0, 0.0),
               Vector4(0.0, 0.0, (zfar + znear) / (znear - zfar), -1.0),
               Vector4(0.0, 0.0, (2.0 * zfar * znear) / (znear - zfar), 0.0));
    return m;
}

inline Matrix44 Matrix44Ortho(float Left, float Right, float Bottom, float Top, float Near, float Far)
{
	Matrix44 m;

    m.r[0] = Vector4(2.0f / (Right-Left), 0.0f, 0.0f, 0.0f);
    m.r[1] = Vector4(0.0f, 2.0f / (Top-Bottom), 0.0f, 0.0f);
    m.r[2] = Vector4(0.0f, 0.0f, -2.0f / (Far-Near), 0.0f);
	m.r[3] = Vector4(-(Right+Left) / (Right-Left), -(Top+Bottom) / (Top-Bottom), -(Far+Near) / (Far-Near), 1.0f);

	return m;
}

inline Matrix44 Matrix44FromAxisAngle(const Vector3& Axis, const float Radians)
{
	float s, c, mag, xx, yy, zz, xy, yz, zx, xs, ys, zs, one_c;

	s = sinf(Radians);
	c = cosf(Radians);
	mag = Axis.Length();

	if (mag == 0.0f)
		return Matrix44Identity();

	Vector3 Normal = Axis * (1.0f / mag);

	xx = Normal[0] * Normal[0];
	yy = Normal[1] * Normal[1];
	zz = Normal[2] * Normal[2];
	xy = Normal[0] * Normal[1];
	yz = Normal[1] * Normal[2];
	zx = Normal[2] * Normal[0];
	xs = Normal[0] * s;
	ys = Normal[1] * s;
	zs = Normal[2] * s;
	one_c = 1.0f - c;

	Matrix44 m;

	m.r[0] = Vector4((one_c * xx) + c, (one_c * xy) + zs, (one_c * zx) - ys, 0.0f);
	m.r[1] = Vector4((one_c * xy) - zs, (one_c * yy) + c, (one_c * yz) + xs, 0.0f);
	m.r[2] = Vector4((one_c * zx) + ys, (one_c * yz) - xs, (one_c * zz) + c, 0.0f);
	m.r[3] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

	return m;
}

inline Vector4 Matrix44ToAxisAngle(const Matrix44& m)
{
	Vector3 Rows[3];
	Rows[0] = lcNormalize(Vector3(m.r[0][0], m.r[0][1], m.r[0][2]));
	Rows[1] = lcNormalize(Vector3(m.r[1][0], m.r[1][1], m.r[1][2]));
	Rows[2] = lcNormalize(Vector3(m.r[2][0], m.r[2][1], m.r[2][2]));

	// Determinant should be 1 for rotation matrices.
	float Determinant = Rows[0][0] * Rows[1][1] * Rows[2][2] + Rows[0][1] * Rows[1][2] * Rows[2][0] +
	                    Rows[0][2] * Rows[1][0] * Rows[2][1] - Rows[0][0] * Rows[1][2] * Rows[2][1] - 
	                    Rows[0][1] * Rows[1][0] * Rows[2][2] - Rows[0][2] * Rows[1][1] * Rows[2][0];

	if (Determinant < 0.0f)
		Rows[0] *= -1.0f;

	float Trace = Rows[0][0] + Rows[1][1] + Rows[2][2];
	float Cos = 0.5f * (Trace - 1.0f);
	Vector4 rot;

	rot[3] = acosf(lcClamp(Cos, -1.0f, 1.0f));  // in [0,PI]

	if (rot[3] > 0.01f)
	{
		if (fabsf(LC_PI - rot[3]) > 0.01f)
		{
			rot[0] = Rows[1][2] - Rows[2][1];
			rot[1] = Rows[2][0] - Rows[0][2];
			rot[2] = Rows[0][1] - Rows[1][0];

			float inv = 1.0f / sqrtf(rot[0]*rot[0] + rot[1]*rot[1] + rot[2]*rot[2]);

			rot[0] *= inv;
			rot[1] *= inv;
			rot[2] *= inv;
		}
		else
		{
			// angle is PI
			float HalfInverse;
			if (Rows[0][0] >= Rows[1][1])
			{
				// r00 >= r11
				if (Rows[0][0] >= Rows[2][2])
				{
					// r00 is maximum diagonal term
					rot[0] = 0.5f * sqrtf(Rows[0][0] - Rows[1][1] - Rows[2][2] + 1.0f);
					HalfInverse = 0.5f / rot[0];
					rot[1] = HalfInverse * Rows[1][0];
					rot[2] = HalfInverse * Rows[2][0];
				}
				else
				{
					// r22 is maximum diagonal term
					rot[2] = 0.5f * sqrtf(Rows[2][2] - Rows[0][0] - Rows[1][1] + 1.0f);
					HalfInverse = 0.5f / rot[2];
					rot[0] = HalfInverse * Rows[2][0];
					rot[1] = HalfInverse * Rows[2][1];
				}
			}
			else
			{
				// r11 > r00
				if (Rows[1][1] >= Rows[2][2])
				{
					// r11 is maximum diagonal term
					rot[1] = 0.5f * sqrtf(Rows[1][1] - Rows[0][0] - Rows[2][2] + 1.0f);
					HalfInverse  = 0.5f / rot[1];
					rot[0] = HalfInverse * Rows[1][0];
					rot[2] = HalfInverse * Rows[2][1];
				}
				else
				{
					// r22 is maximum diagonal term
					rot[2] = 0.5f * sqrtf(Rows[2][2] - Rows[0][0] - Rows[1][1] + 1.0f);
					HalfInverse = 0.5f / rot[2];
					rot[0] = HalfInverse * Rows[2][0];
					rot[1] = HalfInverse * Rows[2][1];
				}
			}
		}
	}
	else
	{
		// The angle is 0 and the matrix is the identity.
		rot[0] = 0.0f;
		rot[1] = 0.0f;
		rot[2] = 1.0f;
	}

	return rot;
}

inline Matrix44 Matrix44FromEulerAngles(const Vector3& Radians)
{
	float CosYaw, SinYaw, CosPitch, SinPitch, CosRoll, SinRoll;

	CosRoll = cosf(Radians[0]);
	SinRoll = sinf(Radians[0]);
	CosPitch = cosf(Radians[1]);
	SinPitch = sinf(Radians[1]);
	CosYaw = cosf(Radians[2]);
	SinYaw = sinf(Radians[2]);

	Matrix44 m;

	m.r[0] = Vector4(CosYaw * CosPitch, SinYaw * CosPitch, -SinPitch, 0.0f);
	m.r[1] = Vector4(CosYaw * SinPitch * SinRoll - SinYaw * CosRoll, CosYaw * CosRoll + SinYaw * SinPitch * SinRoll, CosPitch * SinRoll, 0.0f);
	m.r[2] = Vector4(CosYaw * SinPitch * CosRoll + SinYaw * SinRoll, SinYaw * SinPitch * CosRoll - CosYaw * SinRoll, CosPitch * CosRoll, 0.0f);
	m.r[3] = Vector4(0.0f, 0.0f, 0.0f, 1.0f);

	return m;
}

inline Vector3 Matrix44ToEulerAngles(const Matrix44& RotMat)
{
	float SinPitch, CosPitch, SinRoll, CosRoll, SinYaw, CosYaw;

	SinPitch = -RotMat.r[0][2];
	CosPitch = sqrtf(1 - SinPitch*SinPitch);

	if (fabsf(CosPitch) > 0.0005f)
	{
		SinRoll = RotMat.r[1][2] / CosPitch;
		CosRoll = RotMat.r[2][2] / CosPitch;
		SinYaw = RotMat.r[0][1] / CosPitch;
		CosYaw = RotMat.r[0][0] / CosPitch;
	} 
	else
	{
		SinRoll = -RotMat.r[2][1];
		CosRoll = RotMat.r[1][1];
		SinYaw = 0.0f;
		CosYaw = 1.0f;
	}

	Vector3 Rot(atan2f(SinRoll, CosRoll), atan2f(SinPitch, CosPitch), atan2f(SinYaw, CosYaw));

	if (Rot[0] < 0) Rot[0] += LC_2PI;
	if (Rot[1] < 0) Rot[1] += LC_2PI;
	if (Rot[2] < 0) Rot[2] += LC_2PI;

	return Rot;
}

inline Matrix44 Matrix44Transpose(const Matrix44& m)
{
	Matrix44 t;

	t.r[0] = Vector4(m[0][0], m[1][0], m[2][0], m[3][0]);
	t.r[1] = Vector4(m[0][1], m[1][1], m[2][1], m[3][1]);
	t.r[2] = Vector4(m[0][2], m[1][2], m[2][2], m[3][2]);
	t.r[3] = Vector4(m[0][3], m[1][3], m[2][3], m[3][3]);

	return t;
}

inline Matrix44 Matrix44AffineInverse(const Matrix44& m)
{
	Matrix44 Inv;

	Inv.r[0] = Vector4(m.r[0][0], m.r[1][0], m.r[2][0], m.r[0][3]);
	Inv.r[1] = Vector4(m.r[0][1], m.r[1][1], m.r[2][1], m.r[1][3]);
	Inv.r[2] = Vector4(m.r[0][2], m.r[1][2], m.r[2][2], m.r[2][3]);

	Vector3 Trans = -lcMul30(m.r[3], Inv);
	Inv.r[3] = Vector4(Trans[0], Trans[1], Trans[2], 1.0f);

	return Inv;
}

// Inverse code from the GLU library.
inline Matrix44 Matrix44Inverse(const Matrix44& m)
{
#define SWAP_ROWS(a, b) { float *_tmp = a; (a)=(b); (b)=_tmp; }
#define MAT(m,col,row) m.r[row][col]

	float wtmp[4][8];
	float m0, m1, m2, m3, s;
	float *r0, *r1, *r2, *r3;

    r0 = wtmp[0]; r1 = wtmp[1]; r2 = wtmp[2]; r3 = wtmp[3];

    r0[0] = MAT(m,0,0); r0[1] = MAT(m,0,1);
    r0[2] = MAT(m,0,2); r0[3] = MAT(m,0,3);
    r0[4] = 1.0; r0[5] = r0[6] = r0[7] = 0.0;

    r1[0] = MAT(m,1,0); r1[1] = MAT(m,1,1);
    r1[2] = MAT(m,1,2); r1[3] = MAT(m,1,3);
    r1[5] = 1.0; r1[4] = r1[6] = r1[7] = 0.0;

    r2[0] = MAT(m,2,0); r2[1] = MAT(m,2,1);
    r2[2] = MAT(m,2,2); r2[3] = MAT(m,2,3);
    r2[6] = 1.0; r2[4] = r2[5] = r2[7] = 0.0;

    r3[0] = MAT(m,3,0); r3[1] = MAT(m,3,1);
    r3[2] = MAT(m,3,2); r3[3] = MAT(m,3,3);
    r3[7] = 1.0; r3[4] = r3[5] = r3[6] = 0.0;

	// choose pivot - or die
	if (fabs(r3[0])>fabs(r2[0])) SWAP_ROWS(r3, r2);
	if (fabs(r2[0])>fabs(r1[0])) SWAP_ROWS(r2, r1);
	if (fabs(r1[0])>fabs(r0[0])) SWAP_ROWS(r1, r0);
//	if (0.0 == r0[0])  return GL_FALSE;

	// eliminate first variable
	m1 = r1[0]/r0[0]; m2 = r2[0]/r0[0]; m3 = r3[0]/r0[0];
	s = r0[1]; r1[1] -= m1 * s; r2[1] -= m2 * s; r3[1] -= m3 * s;
	s = r0[2]; r1[2] -= m1 * s; r2[2] -= m2 * s; r3[2] -= m3 * s;
	s = r0[3]; r1[3] -= m1 * s; r2[3] -= m2 * s; r3[3] -= m3 * s;
	s = r0[4];
	if (s != 0.0) { r1[4] -= m1 * s; r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r0[5];
	if (s != 0.0) { r1[5] -= m1 * s; r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r0[6];
	if (s != 0.0) { r1[6] -= m1 * s; r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r0[7];
	if (s != 0.0) { r1[7] -= m1 * s; r2[7] -= m2 * s; r3[7] -= m3 * s; }

	// choose pivot - or die
	if (fabs(r3[1])>fabs(r2[1])) SWAP_ROWS(r3, r2);
	if (fabs(r2[1])>fabs(r1[1])) SWAP_ROWS(r2, r1);
//	if (0.0 == r1[1])  return GL_FALSE;

	// eliminate second variable
	m2 = r2[1]/r1[1]; m3 = r3[1]/r1[1];
	r2[2] -= m2 * r1[2]; r3[2] -= m3 * r1[2];
	r2[3] -= m2 * r1[3]; r3[3] -= m3 * r1[3];
	s = r1[4]; if (0.0 != s) { r2[4] -= m2 * s; r3[4] -= m3 * s; }
	s = r1[5]; if (0.0 != s) { r2[5] -= m2 * s; r3[5] -= m3 * s; }
	s = r1[6]; if (0.0 != s) { r2[6] -= m2 * s; r3[6] -= m3 * s; }
	s = r1[7]; if (0.0 != s) { r2[7] -= m2 * s; r3[7] -= m3 * s; }

	// choose pivot - or die
	if (fabs(r3[2])>fabs(r2[2])) SWAP_ROWS(r3, r2);
//	if (0.0 == r2[2])  return GL_FALSE;

	// eliminate third variable
	m3 = r3[2]/r2[2];
    r3[3] -= m3 * r2[3]; r3[4] -= m3 * r2[4];
    r3[5] -= m3 * r2[5]; r3[6] -= m3 * r2[6];
	r3[7] -= m3 * r2[7];

	// last check
//	if (0.0 == r3[3]) return GL_FALSE;

	s = 1.0f/r3[3];              // now back substitute row 3
	r3[4] *= s; r3[5] *= s; r3[6] *= s; r3[7] *= s;

	m2 = r2[3];                 // now back substitute row 2
	s  = 1.0f/r2[2];
    r2[4] = s * (r2[4] - r3[4] * m2); r2[5] = s * (r2[5] - r3[5] * m2);
    r2[6] = s * (r2[6] - r3[6] * m2); r2[7] = s * (r2[7] - r3[7] * m2);
	m1 = r1[3];
    r1[4] -= r3[4] * m1; r1[5] -= r3[5] * m1;
    r1[6] -= r3[6] * m1; r1[7] -= r3[7] * m1;
	m0 = r0[3];
    r0[4] -= r3[4] * m0; r0[5] -= r3[5] * m0;
    r0[6] -= r3[6] * m0; r0[7] -= r3[7] * m0;

	m1 = r1[2];                 // now back substitute row 1
	s  = 1.0f/r1[1];
    r1[4] = s * (r1[4] - r2[4] * m1); r1[5] = s * (r1[5] - r2[5] * m1);
    r1[6] = s * (r1[6] - r2[6] * m1); r1[7] = s * (r1[7] - r2[7] * m1);
	m0 = r0[2];
    r0[4] -= r2[4] * m0; r0[5] -= r2[5] * m0;
    r0[6] -= r2[6] * m0; r0[7] -= r2[7] * m0;

	m0 = r0[1];                 // now back substitute row 0
	s  = 1.0f/r0[0];
    r0[4] = s * (r0[4] - r1[4] * m0); r0[5] = s * (r0[5] - r1[5] * m0);
    r0[6] = s * (r0[6] - r1[6] * m0); r0[7] = s * (r0[7] - r1[7] * m0);

	Vector4 Row0(r0[4], r1[4], r2[4], r3[4]);
	Vector4 Row1(r0[5], r1[5], r2[5], r3[5]);
	Vector4 Row2(r0[6], r1[6], r2[6], r3[6]);
	Vector4 Row3(r0[7], r1[7], r2[7], r3[7]);
	
	Matrix44 out(Row0, Row1, Row2, Row3);
	
	return out;

#undef MAT
#undef SWAP_ROWS
}

inline Vector4 lcQuaternionRotationX(float Radians)
{
	return Vector4(sinf(Radians / 2.0f), 0, 0, cosf(Radians / 2.0f));
}

inline Vector4 lcQuaternionRotationY(float Radians)
{
	return Vector4(0, sinf(Radians / 2.0f), 0, cosf(Radians / 2.0f));
}

inline Vector4 lcQuaternionRotationZ(float Radians)
{
	return Vector4(0, 0, sinf(Radians / 2.0f), cosf(Radians / 2.0f));
}

inline Vector4 lcQuaternionFromAxisAngle(const Vector4& a)
{
	float s = sinf(a[3] / 2.0f);
	return Vector4(a[0] * s, a[1] * s, a[2] * s, cosf(a[3] / 2.0f));
}

inline Vector4 lcQuaternionToAxisAngle(const Vector4& a)
{
	float Len = lcDot3(a, a);

	if (Len > 0.00001f)
	{
		float f = 1.0f / sqrtf(Len);
		return Vector4(a[0] * f, a[1] * f, a[2] * f, acosf(a[3]) * 2.0f);
	}
	else
	{
		return Vector4(0, 0, 1, 0);
	}
}

inline Vector4 lcQuaternionMultiply(const Vector4& a, const Vector4& b)
{
	float x =  a[0] * b[3] + a[1] * b[2] - a[2] * b[1] + a[3] * b[0];
	float y = -a[0] * b[2] + a[1] * b[3] + a[2] * b[0] + a[3] * b[1];
	float z =  a[0] * b[1] - a[1] * b[0] + a[2] * b[3] + a[3] * b[2];
	float w = -a[0] * b[0] - a[1] * b[1] - a[2] * b[2] + a[3] * b[3];

	return Vector4(x, y, z, w);
}

inline Vector3 lcQuaternionMul(const Vector3& a, const Vector4& b)
{
	// Faster to transform to a matrix and multiply.
	float Tx  = 2.0f*b[0];
	float Ty  = 2.0f*b[1];
	float Tz  = 2.0f*b[2];
	float Twx = Tx*b[3];
	float Twy = Ty*b[3];
	float Twz = Tz*b[3];
	float Txx = Tx*b[0];
	float Txy = Ty*b[0];
	float Txz = Tz*b[0];
	float Tyy = Ty*b[1];
	float Tyz = Tz*b[1];
	float Tzz = Tz*b[2];

	Vector3 Rows[3];
	Rows[0] = Vector3(1.0f-(Tyy+Tzz), Txy+Twz, Txz-Twy);
	Rows[1] = Vector3(Txy-Twz, 1.0f-(Txx+Tzz), Tyz+Twx);
	Rows[2] = Vector3(Txz+Twy, Tyz-Twx, 1.0f-(Txx+Tyy));

	return Vector3(Rows[0]*a[0] + Rows[1]*a[1] + Rows[2]*a[2]);
}

// Convert world coordinates to screen coordinates.
inline Vector3 lcProjectPoint(const Vector3& Point, const Matrix44& ModelView, const Matrix44& Projection, const int Viewport[4])
{
	Vector4 Tmp;

	Tmp = lcMul4(Vector4(Point[0], Point[1], Point[2], 1.0f), ModelView);
	Tmp = lcMul4(Tmp, Projection);

	// Normalize.
	Tmp /= Tmp[3];

	// Screen coordinates.
	return Vector3(Viewport[0] + (1 + Tmp[0]) * Viewport[2] / 2, Viewport[1] + (1 + Tmp[1]) * Viewport[3] / 2, (1 + Tmp[2]) / 2);
}

inline Vector3 lcUnprojectPoint(const Vector3& Point, const Matrix44& ModelView, const Matrix44& Projection, const int Viewport[4])
{
	// Calculate the screen to model transform.
	Matrix44 Transform = Matrix44Inverse(lcMul(ModelView, Projection));

	Vector4 Tmp;

	// Convert the point to homogeneous coordinates.
	Tmp[0] = (Point[0] - Viewport[0]) * 2.0f / Viewport[2] - 1.0f;
	Tmp[1] = (Point[1] - Viewport[1]) * 2.0f / Viewport[3] - 1.0f;
	Tmp[2] = Point[2] * 2.0f - 1.0f;
	Tmp[3] = 1.0f;

	Tmp = lcMul4(Tmp, Transform);

	if (Tmp[3] != 0.0f)
		Tmp /= Tmp[3];

	return Vector3(Tmp[0], Tmp[1], Tmp[2]);
}

inline void lcUnprojectPoints(Vector3* Points, int NumPoints, const Matrix44& ModelView, const Matrix44& Projection, const int Viewport[4])
{
	// Calculate the screen to model transform.
	Matrix44 Transform = Matrix44Inverse(lcMul(ModelView, Projection));

	for (int i = 0; i < NumPoints; i++)
	{
		Vector4 Tmp;

		// Convert the point to homogeneous coordinates.
		Tmp[0] = (Points[i][0] - Viewport[0]) * 2.0f / Viewport[2] - 1.0f;
		Tmp[1] = (Points[i][1] - Viewport[1]) * 2.0f / Viewport[3] - 1.0f;
		Tmp[2] = Points[i][2] * 2.0f - 1.0f;
		Tmp[3] = 1.0f;

		Tmp = lcMul4(Tmp, Transform);

		if (Tmp[3] != 0.0f)
			Tmp /= Tmp[3];

		Points[i] = Vector3(Tmp[0], Tmp[1], Tmp[2]);
	}
}

inline void lcGetFrustumPlanes(const Matrix44& WorldView, const Matrix44& Projection, Vector4 Planes[6])
{
	Matrix44 WorldProj = lcMul(WorldView, Projection);

	Planes[0][0] = (WorldProj[0][0] - WorldProj[0][3]) * -1;
	Planes[0][1] = (WorldProj[1][0] - WorldProj[1][3]) * -1;
	Planes[0][2] = (WorldProj[2][0] - WorldProj[2][3]) * -1;
	Planes[0][3] = (WorldProj[3][0] - WorldProj[3][3]) * -1;
	Planes[1][0] =  WorldProj[0][0] + WorldProj[0][3];
	Planes[1][1] =  WorldProj[1][0] + WorldProj[1][3];
	Planes[1][2] =  WorldProj[2][0] + WorldProj[2][3];
	Planes[1][3] =  WorldProj[3][0] + WorldProj[3][3];
	Planes[2][0] = (WorldProj[0][1] - WorldProj[0][3]) * -1;
	Planes[2][1] = (WorldProj[1][1] - WorldProj[1][3]) * -1;
	Planes[2][2] = (WorldProj[2][1] - WorldProj[2][3]) * -1;
	Planes[2][3] = (WorldProj[3][1] - WorldProj[3][3]) * -1;
	Planes[3][0] =  WorldProj[0][1] + WorldProj[0][3];
	Planes[3][1] =  WorldProj[1][1] + WorldProj[1][3];
	Planes[3][2] =  WorldProj[2][1] + WorldProj[2][3];
	Planes[3][3] =  WorldProj[3][1] + WorldProj[3][3];
	Planes[4][0] = (WorldProj[0][2] - WorldProj[0][3]) * -1;
	Planes[4][1] = (WorldProj[1][2] - WorldProj[1][3]) * -1;
	Planes[4][2] = (WorldProj[2][2] - WorldProj[2][3]) * -1;
	Planes[4][3] = (WorldProj[3][2] - WorldProj[3][3]) * -1;
	Planes[5][0] =  WorldProj[0][2] + WorldProj[0][3];
	Planes[5][1] =  WorldProj[1][2] + WorldProj[1][3];
	Planes[5][2] =  WorldProj[2][2] + WorldProj[2][3];
	Planes[5][3] =  WorldProj[3][2] + WorldProj[3][3];

	for (int i = 0; i < 6; i++)
	{
		Vector3 Normal(Planes[i][0], Planes[i][1], Planes[i][2]);
		float Length = Normal.Length();
		Planes[i] /= -Length;
	}
}

inline Vector3 lcZoomExtents(const Vector3& Position, const Matrix44& WorldView, const Matrix44& Projection, const Vector3* Points, int NumPoints)
{
	if (!NumPoints)
		return Position;

	Vector4 Planes[6];
	lcGetFrustumPlanes(WorldView, Projection, Planes);

	Vector3 Front(WorldView[0][2], WorldView[1][2], WorldView[2][2]);

	// Calculate the position that is as close as possible to the model and has all pieces visible.
	float SmallestDistance = FLT_MAX;

	for (int p = 0; p < 4; p++)
	{
		Vector3 Plane(Planes[p][0], Planes[p][1], Planes[p][2]);
		float ep = lcDot(Position, Plane);
		float fp = lcDot(Front, Plane);

		for (int j = 0; j < NumPoints; j++)
		{
			// Intersect the camera line with the plane that contains this point, NewEye = Eye + u * (Target - Eye)
			float u = (ep - lcDot(Points[j], Plane)) / fp;

			if (u < SmallestDistance)
				SmallestDistance = u;
		}
	}

	return Position - (Front * SmallestDistance);
}

inline void lcClosestPointsBetweenLines(const Vector3& Line1a, const Vector3& Line1b, const Vector3& Line2a, const Vector3& Line2b, Vector3* Intersection1, Vector3* Intersection2)
{
	Vector3 u1 = Line1b - Line1a;
	Vector3 u2 = Line2b - Line2a;
	Vector3 p21 = Line2a - Line1a;
	Vector3 m = lcCross(u2, u1);
	float m2 = lcDot(m, m);

	if (m2 < 0.00001f)
	{
		if (Intersection1)
			*Intersection1 = Line1a;
		if (Intersection2)
			*Intersection2 = Line2a;
		return;
	}

	Vector3 r = lcCross(p21, m / m2);

	if (Intersection1)
	{
		float t1 = lcDot(r, u2);
		*Intersection1 = Line1a + t1 * u1;
	}

	if (Intersection2)
	{
		float t2 = lcDot(r, u1);
		*Intersection2 = Line2a + t2 * u2;
	}
}

// Calculate the intersection of a line segment and a plane and returns false
// if they are parallel or the intersection is outside the line segment.
inline bool lcLinePlaneIntersection(Vector3* Intersection, const Vector3& Start, const Vector3& End, const Vector4& Plane)
{
	Vector3 Dir = End - Start;
	Vector3 PlaneNormal(Plane[0], Plane[1], Plane[2]);

	float t1 = lcDot(PlaneNormal, Start) + Plane[3];
	float t2 = lcDot(PlaneNormal, Dir);

	if (t2 == 0.0f)
		return false;

	float t = -t1 / t2;

	*Intersection = Start + t * Dir;

	if ((t < 0.0f) || (t > 1.0f))
		return false;

	return true;
}

inline bool lcLineTriangleMinIntersection(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& Start, const Vector3& End, float* MinDist, Vector3* Intersection)
{
	// Calculate the polygon plane.
	Vector3 PlaneNormal = lcCross(p1 - p2, p3 - p2);
	float PlaneD = -lcDot(PlaneNormal, p1);

	// Check if the line is parallel to the plane.
	Vector3 Dir = End - Start;

	float t1 = lcDot(PlaneNormal, Start) + PlaneD;
	float t2 = lcDot(PlaneNormal, Dir);

	if (t2 == 0)
		return false;

	float t = -(t1 / t2);

	if (t < 0)
		return false;

	// Intersection of the plane and line segment.
	*Intersection = Start - (t1 / t2) * Dir;

	float Dist = lcLength(Start - *Intersection);

	if (Dist > *MinDist)
		return false;

	// Check if we're inside the triangle.
	Vector3 pa1, pa2, pa3;
	pa1 = lcNormalize(p1 - *Intersection);
	pa2 = lcNormalize(p2 - *Intersection);
	pa3 = lcNormalize(p3 - *Intersection);

	float a1, a2, a3;
	a1 = lcDot(pa1, pa2);
	a2 = lcDot(pa2, pa3);
	a3 = lcDot(pa3, pa1);

	float total = (acosf(a1) + acosf(a2) + acosf(a3)) * LC_RTOD;

	if (fabs(total - 360) <= 0.001f)
	{
		*MinDist = Dist;
		return true;
	}

	return false;
}

// Sutherland-Hodgman method of clipping a polygon to a plane.
inline void lcPolygonPlaneClip(Vector3* InPoints, int NumInPoints, Vector3* OutPoints, int* NumOutPoints, const Vector4& Plane)
{
	Vector3 *s, *p, i;

	*NumOutPoints = 0;
	s = &InPoints[NumInPoints-1];

	for (int j = 0; j < NumInPoints; j++)
	{
		p = &InPoints[j];

		if (lcDot3(*p, Plane) + Plane[3] <= 0)
		{
			if (lcDot3(*s, Plane) + Plane[3] <= 0)
			{
				// Both points inside.
				OutPoints[*NumOutPoints] = *p;
				*NumOutPoints = *NumOutPoints + 1;
			}
			else
			{
				// Outside, inside.
				lcLinePlaneIntersection(&i, *s, *p, Plane);

				OutPoints[*NumOutPoints] = i;
				*NumOutPoints = *NumOutPoints + 1;
				OutPoints[*NumOutPoints] = *p;
				*NumOutPoints = *NumOutPoints + 1;
			}
		}
		else
		{
			if (lcDot3(*s, Plane) + Plane[3] <= 0)
			{
				// Inside, outside.
				lcLinePlaneIntersection(&i, *s, *p, Plane);

				OutPoints[*NumOutPoints] = i;
				*NumOutPoints = *NumOutPoints + 1;
			}
		}

		s = p;
	}
}

// Return true if a polygon intersects a set of planes.
inline bool lcTriangleIntersectsPlanes(Vector3* p1, Vector3* p2, Vector3* p3, const Vector4 Planes[6])
{
	const int NumPlanes = 6;
	Vector3* Points[3] = { p1, p2, p3 };
	int Outcodes[3] = { 0, 0, 0 }, i;
	int NumPoints = 3;

	// First do the Cohen-Sutherland out code test for trivial rejects/accepts.
	for (i = 0; i < NumPoints; i++)
	{
		//Vector3 Pt(Points[i][0], Points[i][1], Points[i][2]);
        Vector3 &Pt = *Points[i];
		for (int j = 0; j < NumPlanes; j++)
		{
			if (lcDot3(Pt, Planes[j]) + Planes[j][3] > 0)
				Outcodes[i] |= 1 << j;
		}
	}

	// Polygon completely outside a plane.
	if ((Outcodes[0] & Outcodes[1] & Outcodes[2]) != 0)
		return false;

	// If any vertex has an out code of all zeros then we intersect the volume.
	if (!Outcodes[0] || !Outcodes[1] || !Outcodes[2])
		return true;

	// Buffers for clipping the polygon.
	Vector3 ClipPoints[2][8];
	int NumClipPoints[2];
	int ClipBuffer = 0;

	NumClipPoints[0] = NumPoints;
    ClipPoints[0][0] = *p1;//Vector3(p1[0], p1[1], p1[2]);
    ClipPoints[0][1] = *p2;//Vector3(p2[0], p2[1], p2[2]);
    ClipPoints[0][2] = *p3;//Vector3(p3[0], p3[1], p3[2]);

	// Now clip the polygon against the planes.
	for (i = 0; i < NumPlanes; i++)
	{
		lcPolygonPlaneClip(ClipPoints[ClipBuffer], NumClipPoints[ClipBuffer], ClipPoints[ClipBuffer^1], &NumClipPoints[ClipBuffer^1], Planes[i]);
		ClipBuffer ^= 1;

		if (!NumClipPoints[ClipBuffer])
			return false;
	}

	return true;
}

// Return true if a ray intersects a bounding box, and calculates the distance from the start of the ray (adapted from Graphics Gems).
inline bool lcBoundingBoxRayIntersectDistance(const Vector3& Min, const Vector3& Max, const Vector3& Start, const Vector3& End, float* Dist, Vector3* Intersection)
{
	bool MiddleQuadrant[3];
	bool Inside = true;
	float CandidatePlane[3];
	float MaxT[3];
	int i;

	// Find candidate planes.
	for (i = 0; i < 3; i++)
	{
		if (Start[i] < Min[i])
		{
			MiddleQuadrant[i] = false;
			CandidatePlane[i] = Min[i];
			Inside = false;
		}
		else if (Start[i] > Max[i])
		{
			MiddleQuadrant[i] = false;
			CandidatePlane[i] = Max[i];
			Inside = false;
		}
		else
		{
			MiddleQuadrant[i] = true;
		}
	}

	// Ray origin inside box.
	if (Inside)
	{
		*Dist = 0;

		if (*Intersection)
			*Intersection = Start;

		return true;
	}

	// Calculate T distances to candidate planes.
	Vector3 Dir = End - Start;

	for (i = 0; i < 3; i++)
	{
		if (!MiddleQuadrant[i] && Dir[i] != 0.0f)
			MaxT[i] = (CandidatePlane[i] - Start[i]) / Dir[i];
		else
			MaxT[i] = -1.0f;
	}

	// Get largest of the MaxT's for final choice of intersection.
	int WhichPlane = 0;
	for (i = 1; i < 3; i++)
		if (MaxT[WhichPlane] < MaxT[i])
			WhichPlane = i;

	// Check final candidate actually inside box.
	if (MaxT[WhichPlane] < 0.0f)
		return false;

	Vector3 Point;

	for (i = 0; i < 3; i++)
	{
		if (WhichPlane != i)
		{
			Point[i] = Start[i] + MaxT[WhichPlane] * Dir[i];
			if (Point[i] < Min[i] || Point[i] > Max[i])
				return false;
		}
		else
			Point[i] = CandidatePlane[i];
	}

	*Dist = lcLength(Point - Start);

	if (*Intersection)
		*Intersection = Point;

	return true;
}

inline bool lcSphereRayMinIntersectDistance(const Vector3& Center, float Radius, const Vector3& Start, const Vector3& End, float* Dist)
{
	Vector3 Dir = Center - Start;
	float LengthSquaredDir = lcLengthSquared(Dir);
	float RadiusSquared = Radius * Radius;

	if (LengthSquaredDir < RadiusSquared)
	{
		// Ray origin inside sphere.
		*Dist = 0;
		return true;
	}
	else
	{
		Vector3 RayDir = End - Start;
		float t = lcDot(Dir, RayDir) / lcLengthSquared(RayDir);

		// Ray points away from sphere.
		if (t < 0)
			return false;

		float c = (RadiusSquared - LengthSquaredDir) / lcLengthSquared(RayDir) + (t * t);
		if (c > 0)
		{
			*Dist = t - sqrtf(c);
			return true;
		}

		return false;
	}
}

/*
float LinePointMinDistance(const Vector3& Point, const Vector3& Start, const Vector3& End)
{
	Vector3 Dir = End - Start;

	float t1 = Dot3(Start - Point, Dir);
	float t2 = LengthSquared(Dir);

	float t = -t1 / t2;

	if (t < 0.0f)
		t = 0.0f;
	else if (t > 1.0f)
		t = 1.0f;

	Vector3 Closest = Start + t * Dir;

	return Length(Closest - Point);
}
*/
// Returns true if the axis aligned box intersects the volume defined by planes.
inline bool lcBoundingBoxIntersectsVolume(const Vector3& Min, const Vector3& Max, const Vector4 Planes[6])
{
	const int NumPlanes = 6;
	Vector3 Points[8] =
	{
		Points[0] = Vector3(Min[0], Min[1], Min[2]),
		Points[1] = Vector3(Min[0], Max[1], Min[2]),
		Points[2] = Vector3(Max[0], Max[1], Min[2]),
		Points[3] = Vector3(Max[0], Min[1], Min[2]),
		Points[4] = Vector3(Min[0], Min[1], Max[2]),
		Points[5] = Vector3(Min[0], Max[1], Max[2]),
		Points[6] = Vector3(Max[0], Max[1], Max[2]),
		Points[7] = Vector3(Max[0], Min[1], Max[2])
	};

	// Start by testing trivial reject/accept cases.
	int Outcodes[8];
	int i;

	for (i = 0; i < 8; i++)
	{
		Outcodes[i] = 0;

		for (int j = 0; j < NumPlanes; j++)
		{
			if (lcDot3(Points[i], Planes[j]) + Planes[j][3] > 0)
				Outcodes[i] |= 1 << j;
		}
	}

	int OutcodesOR = 0, OutcodesAND = 0x3f;

	for (i = 0; i < 8; i++)
	{
		OutcodesAND &= Outcodes[i];
		OutcodesOR |= Outcodes[i];
	}

	// All corners outside the same plane.
	if (OutcodesAND != 0)
		return false;

	// All corners inside the volume.
	if (OutcodesOR == 0)
		return true;

	int Indices[36] = 
	{
		0, 1, 2,
		0, 2, 3,
		7, 6, 5,
		7, 5, 4,
		0, 1, 5,
		0, 5, 4,
		2, 3, 7,
		2, 7, 6,
		0, 3, 7,
		0, 7, 4,
		1, 2, 6,
		1, 6, 5
	};

	for (int Idx = 0; Idx < 36; Idx += 3)
		if (lcTriangleIntersectsPlanes(&Points[Indices[Idx]*3], &Points[Indices[Idx+1]*3], &Points[Indices[Idx+2]*3], Planes))
			return true;

	return false;
}
/*
bool SphereIntersectsVolume(const Vector3& Center, float Radius, const Vector4* Planes, int NumPlanes)
{
	for (int j = 0; j < NumPlanes; j++)
		if (Dot3(Center, Planes[j]) + Planes[j][3] > Radius)
			return false;

	return true;
}*/

inline Vector3 vectorForPolar(float r, float angx, float angy)
{
    float z = r * cosf(angy) * cosf(angx);
    float x = r * sinf(angy) * cosf(angx);
    float y = r * sinf(angx);
    return Vector3(x, y, z);
}


#endif // _LC_MATH_H_
