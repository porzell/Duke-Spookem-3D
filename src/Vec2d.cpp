
#include "VectorMath.h"

//Constructor

//Default constructor for Vec2d.
Vec2d::Vec2d ()
{
	mX = 0;
	mY = 0;
}

//Copy constructor for Vec2d.
Vec2d::Vec2d( const Vec2d& rhs )
{
	mX = rhs.mX;
	mY = rhs.mY;
}

//Create a normalized Vec2d instance from an angle (in radians).
Vec2d::Vec2d (float angle)
{
	mX = sin(angle);
	mY = cos(angle);
}

//Create a Vec2d instance given mX and mY coordinates.
Vec2d::Vec2d (float x_coord, float y_coord)
{
	mX = x_coord;
	mY = y_coord;
}

//Get the distance to another Vec2d.
const float Vec2d::dist(Vec2d& other)
{
	return sqrt(pow((mX - other.mX),2) + pow((mY - other.mY),2));
}

//Return vector magnitude.
const float Vec2d::mag()
{
	return sqrt(pow(mX,2) + pow(mY,2));
}

//Normalizes the instance of the Vec2d object. (modifies self)
void Vec2d::normalize()
{
	//Prevent a divide-by-zero and also avoid
	//doing all the costlmY math to get magnitude.
	
	if(mX == 0 && mY == 0)
		return;

	float magnitude = mag();

	mX /= magnitude;
	mY /= magnitude;

}

//Return a normalized version of the vector.
Vec2d Vec2d::normalized()
{
	Vec2d tmp;
	
	//Prevent a divide-by-zero and also avoid
	//doing all the costlmY math to get magnitude.
	
	if(mX == 0 && mY == 0)
	{
		tmp.mX = tmp.mY = 0;
		return tmp;
	}

	float magnitude = mag();

	tmp.mX = mX/magnitude;
	tmp.mY = mY/magnitude;
	
	return tmp;
}

//Return a vector 90 degrees to parent.
Vec2d Vec2d::rot90() //const
{
	return Vec2d(mY, -mX);
}

//Set the vector to a specific magnitude.
void Vec2d::setMag(float magnitude)
{
	normalize();

	mX *= magnitude;
	mY *= magnitude;
}

//Get an angle (in degrees) represented bmY the vector.
const float Vec2d::toDegrees()
{
	return atan2(mY,mX) * (PI / 180);
}

//Get an angle (in radians) represented bmY the vector.
const float Vec2d::toRadians()
{
	return atan2(mY,mX);
}

//Check if a vector is greater than another.
bool Vec2d::operator > (Vec2d vec)
{
	return mag() > vec.mag();
}

//Check if a vector is greater than another.
bool Vec2d::operator < (Vec2d vec)
{
	return mag() < vec.mag();
}

//Check if a vector is greater than another.
bool Vec2d::operator == (const Vec2d vec)
{
	return
		(mX == vec.mX &&
		mY == vec.mY);
}

//Perform vector addition.
Vec2d Vec2d::operator+ (const Vec2d vec)
{
	Vec2d temp;

	temp.mX = mX + vec.mX;
	temp.mY = mY + vec.mY;

	return temp;
}

//Perform vector subtraction.
Vec2d Vec2d::operator- (const Vec2d vec)
{
	Vec2d temp;

	temp.mX = mX - vec.mX;
	temp.mY = mY - vec.mY;

	return temp;
}

//Perform a vector multiplication
Vec2d Vec2d::operator* (const Vec2d vec)
{
	Vec2d temp;

	temp.mX = mX * vec.mX;
	temp.mY = mY * vec.mY;

	return temp;
}

//Perform a vector division
Vec2d Vec2d::operator/ (const Vec2d vec)
{
	Vec2d temp;

	temp.mX = mX / vec.mX;
	temp.mY = mY / vec.mY;

	return temp;
}

//Perform scalar addition.
Vec2d Vec2d::operator+ (const float add)
{
	Vec2d temp;

	temp.mX = mX + add;
	temp.mY = mY + add;

	return temp;
}

//Perform scalar subtraction.
Vec2d Vec2d::operator- (const float sub)
{
	Vec2d temp;

	temp.mX = mX - sub;
	temp.mY = mY - sub;

	return temp;
}

//Perform a scalar multiplication.
Vec2d Vec2d::operator* (const float mul)
{
	Vec2d temp;

	temp.mX = mX * mul;
	temp.mY = mY * mul;

	return temp;
}

//Perform a scalar division.
Vec2d Vec2d::operator/ (const float div)
{
	Vec2d temp;

	temp.mX = mX / div;
	temp.mY = mY / div;

	return temp;
}

//Perform vector addition.
Vec2d& Vec2d::operator+= (const Vec2d vec)
{
	mX += vec.mX;
	mY += vec.mY;

	return *this;
}

//Perform vector subtraction.
Vec2d& Vec2d::operator-= (const Vec2d vec)
{
	mX -= vec.mX;
	mY -= vec.mY;

	return *this;
}

//Perform vector with scalar addition.
Vec2d& Vec2d::operator+= (const float add)
{
	mX += add;
	mY += add;

	return *this;
}

//Perform vector with scalar subtraction.
Vec2d& Vec2d::operator-= (const float sub)
{
	mX -= sub;
	mY -= sub;

	return *this;
}

//Perform a scalar multiplication.
Vec2d& Vec2d::operator*= (const float mul)
{
	mX *= mul;
	mY *= mul;

	return *this;
}

//Perform a scalar division.
Vec2d& Vec2d::operator/= (const float div)
{
	mX /= div;
	mY /= div;

	return *this;
}

//Assignment operator (Copy)
Vec2d& Vec2d::operator= (const Vec2d other)
{
	mX = other.mX;
	mY = other.mY;

	return *this;
}

//Perform a dot product.
/*float operator * (const Vec2d v1, const Vec2d v2)
{
	float dot;
	dot = (v1.mX*v2.mX) + (v1.mY*v2.mY);

	return dot;
}*/

//Perform a cross product.
float operator ^ (const Vec2d v1, const Vec2d v2)
{
	return (v1.mX * v2.mY) - (v1.mY * v2.mX);
}

//Perform a cross product.
/*Vec2d operator ^ (const Vec2d v1, const Vec2d v2)
{
	Vec2d cross;

	cross.mX = (v1.mmY * v2.z) - (v1.z * v2.mY);
	cross.mmY = (v1.z * v2.mX) - (v1.mX * v2.z);
	cross.z = (v1.mX * v2.mY) - (v1.mmY * v2.mX);

	return cross;
}*/

std::ostream& operator << (std::ostream& stream, const Vec2d vec)
{
	stream << '(' << vec.mX << ", " << vec.mY << ')';
	return stream;
}
