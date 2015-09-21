#pragma once
#ifndef _VEC2D_H
#define _VEC2D_H

#define PI 3.1415926f

#include <math.h>
#include <iostream>

#include "Trackable.h"

class Vec2d : public Trackable
{
	public:
		//Constructors:
		//Vec2d () {};
		Vec2d();

		Vec2d( const Vec2d& rhs );

		Vec2d(float angle);
		Vec2d (float x,float y);

		//Accessors:
		inline float X() const { return mX; };
		inline float Y() const { return mY; };

		//Mutators:
		inline void set(float x, float y) { mX = x; mY = y; };
		inline void setX(float x) { mX = x; };
		inline void setY(float y) { mY = y; };

		inline void add(float x, float y) { mX += x; mY += y; };
		inline void addX(float x) { mX += x; };
		inline void addY(float y) { mY += y; };

		//Functions:

		const float dist(Vec2d&);

		const float mag();

		void normalize();
		Vec2d normalized();

		Vec2d rot90(); //const;

		inline void setMag(float magnitude);

		const float toDegrees();
		const float toRadians();

		//Operators:
		bool operator > (Vec2d);	//Vector comparison
		bool operator < (Vec2d);	//Vector comparison
		bool operator == (Vec2d);	//Vector comparison

		Vec2d operator + (Vec2d);	//Vector addition
		Vec2d operator - (Vec2d);	//Vector subtraction
		Vec2d operator * (Vec2d);	//Vector multiplication
		Vec2d operator / (Vec2d);	//Vector division.
		Vec2d operator + (float);	//Scalar-to-vector addition
		Vec2d operator - (float);	//Scalar-to-vector subtraction
		Vec2d operator * (float);	//Scalar multiplication
		Vec2d operator / (float);	//Scalar division

		Vec2d& operator += (const Vec2d);	//Vector addition
		Vec2d& operator -= (const Vec2d);	//Vector subtraction
		Vec2d& operator += (float);	//Scalar-to-vector addition
		Vec2d& operator -= (float);	//Scalar-to-vector subtraction
		Vec2d& operator *= (const float);	//Scalar multiplication
		Vec2d& operator /= (const float);	//Scalar division

		Vec2d& operator = (const Vec2d);	//Assignment (Copy)

		//friend float operator * (Vec2d v1, Vec2d v2);	//Dot product
		friend float operator ^ (Vec2d v1, Vec2d v2);	//Cross product
		//friend Vec2d operator ^ (Vec2d v1, Vec2d v2);	//Cross product

		friend std::ostream& operator << (std::ostream& stream, const Vec2d vec);

	private:
		float mX,mY;
};

#endif
