
#ifndef FIXEDREAL_H
#define FIXEDREAL_H

template<class T, class L, int BITS>
class FixedReal
{
private:
	T value;
	static const T SCALE = 1 << BITS;

public:
	FixedReal() : value(0)
	{
	}

	FixedReal(const FixedReal& v) : value(v.value)
	{
	}

	FixedReal(T v) : value(v << BITS)
	{
	}

	template<class E>
	FixedReal(E v) : value(v * SCALE)
	{
	}

	operator double() const
	{
		return (double)value / SCALE;
	}

	operator T() const
	{
		return value >> BITS;
	}

	template<class E>
	operator E() const
	{
		return value >> BITS;
	}

	FixedReal operator+=(const FixedReal& v)
	{
		value += v.value;
		return *this;
	}

	FixedReal operator+(const FixedReal& v) const
	{
		FixedReal l(*this);
		return l += v;
	}

	FixedReal operator*=(const FixedReal& v)
	{
		L l = value;
		l *= v.value;
		l >>= BITS;
		value = l;
		return *this;
	}

	FixedReal operator*(const FixedReal& v) const
	{
		FixedReal l(*this);
		return l *= v;
	}

	FixedReal operator/=(const FixedReal& v)
	{
		L l = value;
		l <<= BITS;
		l /= v.value;
		value = l;
		return *this;
	}

	FixedReal operator/(const FixedReal& v) const
	{
		FixedReal l(*this);
		return l /= v;
	}

	FixedReal operator-() const
	{
		FixedReal l(*this);
		l.value *= -1;
		return l;
	}

	FixedReal operator-=(const FixedReal& v)
	{
		value -= v.value;
		return *this;
	}

	FixedReal operator-(const FixedReal& v) const
	{
		FixedReal l(*this);
		return l -= v;
	}

	bool operator==(const FixedReal& v) const
	{
		return value == v.value;
	}

	bool operator!=(const FixedReal& v) const
	{
		return value != v.value;
	}

	bool operator<(const FixedReal& v) const
	{
		return value < v.value;
	}

	bool operator<=(const FixedReal& v) const
	{
		return value <= v.value;
	}

	bool operator>(const FixedReal& v) const
	{
		return value > v.value;
	}

	bool operator>=(const FixedReal& v) const
	{
		return value >= v.value;
	}

	template<class E>
	FixedReal operator+=(E v)
	{
		return *this += FixedReal(v);
	}

	template<class E>
	FixedReal operator+(E v) const
	{
		return FixedReal(*this) + FixedReal(v);
	}

	template<class E>
	FixedReal operator-=(E v)
	{
		return *this -= FixedReal(v);
	}

	template<class E>
	FixedReal operator-(E v) const
	{
		return FixedReal(*this) - FixedReal(v);
	}

	template<class E>
	FixedReal operator*=(E v)
	{
		return *this *= FixedReal(v);
	}

	template<class E>
	FixedReal operator*(E v) const
	{
		return FixedReal(*this) * FixedReal(v);
	}

	template<class E>
	FixedReal operator/=(E v)
	{
		return *this /= FixedReal(v);
	}

	template<class E>
	FixedReal operator/(E v) const
	{
		return FixedReal(*this) / FixedReal(v);
	}

	template<class E>
	bool operator==(E v) const
	{
		return *this == FixedReal(v);
	}

	template<class E>
	bool operator!=(E v) const
	{
		return *this != FixedReal(v);
	}

	template<class E>
	bool operator<(E v) const
	{
		return *this < FixedReal(v);
	}

	template<class E>
	bool operator<=(E v) const
	{
		return *this <= FixedReal(v);
	}

	template<class E>
	bool operator>(E v) const
	{
		return *this > FixedReal(v);
	}

	template<class E>
	bool operator>=(E v) const
	{
		return *this >= FixedReal(v);
	}

};

#endif // !FIXEDREAL_H
