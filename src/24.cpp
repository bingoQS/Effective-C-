class Rational {
public:
	Rational(int numerator = 0, int demominator = 1) :numerat(numerator), denominat(demominator) {}
	int numerator() const{ return numerat; }
	int denominator() const { return denominat; }

	const Rational operator*(const Rational& rhs)const {
		return rhs;
	}

private:
	int numerat;
	int denominat;
};