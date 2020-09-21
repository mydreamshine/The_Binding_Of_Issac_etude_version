#pragma once

#define PI 3.141592f

#include <cmath>
#include <float.h>

//SIZE
struct fSIZE { float Width, Height, Depth; };
//Rect
struct fRECT { float Left, Top, Right, Bottom; };
//Box
struct fBOX  { float Left, Bottom, Back, Right, Top, Front ; };
//Pitch: Object_RotationX, Yaw: Object_RotationY, Roll: Object_RotationZ
struct ANGLE { float Pitch, Yaw, Roll, RotationX, RotationY, RotationZ; };
struct COLOR { float r, g, b, a; };

//Degree -> Radian
inline float Radian(float Degree) { return Degree * (PI / 180.0f); }
//Radian -> Degree
inline float Degree(float Radian) { return Radian * (180.0f / PI); }
//�Ҽ� 6°�ڸ����� ������ x�� y�� ���� ������ �Ǵ�
inline bool equal(float x, float y, float epsilon = 0.000001f) { return fabs(x - y) <= epsilon; }


//3���� ��ǥ�� �ý��� ���� ���
class Point;
class Vector;
class Plane;
class Ray;




//����
class Vector
{
public:
	float i, j, k;

	Vector(float i = 0.0f, float j = 0.0f, float k = 0.0f) : i(i), j(j), k(k) {}
	Vector(Point p);
	~Vector() {}

	Vector operator +(Vector v) { return Vector(i + v.i, j + v.j, k + v.k); }
	Vector& operator +=(Vector v) { i += v.i; j += v.j; k += v.k; return *this; }
	Vector operator -(Vector v) { return Vector(i - v.i, j - v.j, k - v.k); }
	Vector& operator -=(Vector v) { i -= v.i; j -= v.j; k -= v.k; return *this; }
	Vector operator -() { return Vector(-i, -j, -k); }
	Vector operator *(float c) { return Vector(i * c, j * c, k * c); }
	friend Vector operator *(float c, Vector v) { return v * c; }
	Vector& operator *=(Vector v) { i *= v.i; j *= v.j; k *= v.k; return *this; }
	Vector operator /(float c) { return Vector(i / c, j / c, k / c); }
	Vector& operator /=(float c) { i /= c; j /= c; k /= c; return *this; }

	float dot(Vector v) { return i * v.i + j * v.j + k * v.k; }//����
	Vector cross(Vector v) { return Vector(j * v.k - k * v.j, k * v.i - i * v.k, i * v.j - j * v.i); }//����

	float magnitude() { return sqrt(this->dot(*this)); }//���� ũ��
	friend Vector unit(Vector v);//��������
	friend void normalize(Vector& v);//���Ա�������
	friend float cosine(Vector u, Vector v);//���� u�� ���� v�� ���հ�
	bool isPerpendicularTo(Vector v) { return equal(this->dot(v), 0); }//���� v���� ������(������ 0) �Ǵ�
	bool isParallelTo(Vector v) { return equal(cosine(*this, v), 1.0); }//���� v���� ������(cos(��)�� 1) �Ǵ�
	Vector projectionOnto(Vector v) { return this->dot(unit(v)) * unit(v); }//���� v�� ����
	Vector reflectionAbout(Vector v) { return *this - 2 * projectionOnto(v); }//���� v�� �������� �ݻ�Ǵ� ����
};

//���̰� 1�� ��������
inline Vector unit(Vector v) { return v / v.magnitude(); }
//���Ա�������
inline void normalize(Vector &v) { v /= v.magnitude(); }
//���� u�� ���� v�� cosine
inline float cosine(Vector u, Vector v) { return unit(u).dot(unit(v)); }





class Point
{
public:
	float x, y, z;

	~Point() {}
	Point(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
	Point(Vector v) : x(v.i), y(v.j), z(v.k) {}

	Point operator +(Vector v) { return Point(x + v.i, y + v.j, z + v.k); }
	Point& operator +=(Vector v) { x += v.i; y += v.j; z += v.k; return *this; }
	Point& operator +=(Point p) { x += p.x; y += p.y; z += p.z; return *this; }
	Point& operator =(Vector v) { x = v.i; y = v.j; z = v.k; return *this; }
	Vector operator -(Point p) { return Vector(x - p.x, y - p.y, z - p.z); }
	Point operator *(float c) { return Point(x * c, y * c, z * c); }
	Point& operator *=(float c) { x *= c, y *= c, z *= c; return *this; }
	bool operator ==(Point p) { return equal(this->x, p.x) && equal(this->x, p.x) && equal(this->x, p.x); }

	float distanceTo(Point p) { return (p - *this).magnitude(); }//��p���� �Ÿ�
	float distanceTo(Plane P);//���P���� �Ÿ�

	bool SameTo(const Point &p);
};






//CCWü��(p1, p2, p3)�� ���� ���
class Plane
{
public:
	//ax + by + cz + d = 0
	//Vector(a,b,c): ����� ��������
	float a, b, c, d;
	Plane() : a(0.0f), b(0.0f), c(0.0f), d(0.0f) {}
	Plane(float a, float b, float c, float d) : a(a), b(b), c(c), d(d) {}
	Plane(Point p1, Point p2, Point p3) {
		Vector n = (p2 - p1).cross(p3 - p1);
		a = n.i;
		b = n.j;
		c = n.k;
		d = -(Vector(p1).dot(n));
	}
	~Plane() {}

	Plane& operator =(Plane P) { a = P.a; b = P.b; c = P.c; d = P.d; return *this; }

	Vector normal() { return Vector(a, b, c); }//����� ��������
};






//����
class Ray
{
public:
	Point origin;//������
	Vector direction;//����
	Ray(Point origin, Vector direction) : origin(origin), direction(direction) {}
	~Ray() {}

	Point operator()(float u) { return origin + u * direction; }//�������� ����
};




// Ŭ���� ������ body
inline Vector::Vector(Point p) : i(p.x), j(p.y), k(p.z) {}
inline float Point::distanceTo(Plane P) { return fabs(P.a * x + P.b * y + P.c * z + P.d) / P.normal().magnitude(); }//���P���� �Ÿ�
inline bool Point::SameTo(const Point &p) { return equal(this->x, p.x) && equal(this->y, p.y) && equal(this->z, p.z); }//p���� ��ġ ����
