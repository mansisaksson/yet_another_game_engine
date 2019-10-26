#pragma once

template<int Rows, int Cols, typename TypeName>
class matrix
{
public:
	float mat[Cols][Rows];
	
	/* Operators */

	inline TypeName& operator=(const TypeName&rhs)
	{
		for (int x = 0; x < Cols; ++x)
			for (int y = 0; y < Rows; ++y)
				mat[x][y] = rhs.mat[x][y];
		
		return *this;
	}

	inline TypeName& operator-=(const TypeName& rhs)
	{
		for (int x = 0; x < Cols; ++x)
			for (int y = 0; y < Rows; ++y)
				mat[x][y] -= rhs.mat[x][y];

		return *this;
	}

	inline TypeName& operator+=(const TypeName& rhs)
	{
		for (int x = 0; x < Cols; ++x)
			for (int y = 0; y < Rows; ++y)
				mat[x][y] += rhs.mat[x][y];

		return *this;
	}

	inline TypeName& operator*=(const TypeName& rhs)
	{
		for (int x = 0; x < Cols; ++x)
			for (int y = 0; y < Rows; ++y)
				mat[x][y] *= rhs.mat[x][y];

		return *this;
	}

	inline TypeName& operator/=(const TypeName& rhs)
	{
		for (int x = 0; x < Cols; ++x)
			for (int y = 0; y < Rows; ++y)
				mat[x][y] /= rhs.mat[x][y];

		return *this;
	}

	template<typename U>
	inline TypeName operator*=(const U& scalar)
	{
		for (int x = 0; i < Cols; ++i)
			for (int y = 0; i < Rows; ++i)
				mat[x][y] *= scalar;

		return *this;
	}

	template<typename U>
	inline TypeName& operator/=(const U& scalar)
	{
		for (int x = 0; x < Cols; ++x)
			for (int y = 0; y < Rows; ++y)
				mat[x][y] /= scalar;

		return *this;
	}

	float* operator[](int idx)
	{
		assert(idx >= 0 && idx < Cols && "matrix - index out of range");
		return mat[idx];
	}

	const float* operator[](int idx) const
	{
		assert(idx >= 0 && idx < Cols && "matrix - index out of range");
		return mat[idx];
	}
};


/* Comparison operators */

template<int Rows, int Cols, typename TypeName>
inline bool operator==(const TypeName& lhs, const TypeName& rhs)
{ 
	for (int x = 0; x < Cols; ++x)
		for (int y = 0; y < Rows; ++y)
			if (mat[x][y] != rhs.mat[x][y])
				return false;

	return true;
}

template<int Rows, int Cols, typename TypeName>
inline bool operator!=(const TypeName& lhs, const TypeName& rhs) { return !operator==(lhs, rhs); }


/* Assignment operators */

template<int Rows, int Cols, typename TypeName>
inline TypeName operator+(TypeName lhs, const TypeName& rhs)
{
	lhs += rhs;
	return lhs;
}

template<int Rows, int Cols, typename TypeName>
inline TypeName operator-(TypeName lhs, const TypeName& rhs)
{
	lhs -= rhs;
	return lhs;
}

template<int Rows, int Cols, typename TypeName>
inline TypeName operator*(TypeName lhs, const TypeName& rhs)
{
	lhs *= rhs;
	return lhs;
}

template<int Rows, int Cols, typename TypeName>
inline TypeName operator/(TypeName lhs, const TypeName& rhs)
{
	lhs /= rhs;
	return lhs;
}

template<int Rows, int Cols, typename U, typename TypeName>
inline TypeName operator*(TypeName lhs, const U& scalar)
{
	lhs *= scalar;
	return lhs;
}

template<int Rows, int Cols, typename U, typename TypeName>
inline TypeName& operator/(TypeName lhs, const U& scalar)
{
	lhs /= scalar;
	return lhs;
}