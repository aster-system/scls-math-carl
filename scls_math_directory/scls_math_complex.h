/*
 * scls_math_complex.h
 *
 *  Created on: 7 déc. 2025
 *      Author: mmatt
 */

#ifndef SCLS_MATH_COMPLEX
#define SCLS_MATH_COMPLEX


//*********
//
// The Complex part
//
//*********

#include "scls_math_formula.h"
#include "scls_math_numbers.h"
#include "scls_math_polynomial.h"

// The namespace "scls" is used to simplify the all.
namespace scls {

    class Complex : public __Field_Element {
	    // Class representating a complex number
    public:
        //*********
        //
        // Complex simple methods
        //
        //*********

        // Simple fraction constructor
        Complex(__Fraction_Base real_part, __Fraction_Base imaginary) : __Field_Element(), a_imaginary(imaginary), a_real(real_part) {};
        Complex(Fraction real_part, Fraction imaginary) : __Field_Element(), a_imaginary(imaginary), a_real(real_part) {};
        Complex(long long real_part, long long imaginary_part) : Complex(Fraction(real_part), Fraction(imaginary_part)) {};
        Complex(long long real_part, Fraction imaginary_part) : Complex(Fraction(real_part), imaginary_part) {};
        Complex(Fraction real_part, long long imaginary_part) : Complex(real_part, Fraction(imaginary_part)) {};
        Complex(__Fraction_Base real_part) : Complex(real_part, __Fraction_Base(0)){};
        Complex(Fraction real_part) : Complex(real_part, Fraction(0)) {};
        Complex(double real_part) : Complex(Fraction::from_double(real_part)) {};
        Complex(int real_part) : Complex(Fraction(real_part)) {};
        Complex(const Complex& to_copy) : Complex(to_copy.real(), to_copy.imaginary()) {};

        // Clone
        virtual void algebra_clone(Algebra_Element* e) const;
        virtual std::shared_ptr<Algebra_Element> algebra_clone() const;
        virtual std::shared_ptr<Algebra_Element> new_algebra_element() const;
        virtual std::shared_ptr<Algebra_Element> new_algebra_element(std::string content) const;

        // Type of the object
        virtual std::string algebra_type() const;

        // Return if the element is a precise algebric element
        virtual bool is_multiplication_neutral() const{return a_real == 1 && a_imaginary == 0;};

        // Operates this element with another one
        virtual void operate(Algebra_Element* other, std::string operation){};

        // Returns a fraction from a double
        static Complex from_double(double result){return __Fraction_Base::from_double(result);};
        // Returns the module of the complex
        inline scls::Fraction module() const{return a_real * a_real + a_imaginary * a_imaginary; };
        // Normalize the number
        inline void normalize(int limit){a_imaginary.normalize(limit);a_real.normalize(limit);};
        // Converts the number to a double
        inline double to_double(){return a_real.to_double();};

        // Getters and setter
        inline Fraction imaginary() const {return a_imaginary;};
        inline Fraction real() const {return a_real;};
        inline void set_imaginary(Fraction new_imaginary) {a_imaginary=new_imaginary;};
        inline void set_real(Fraction new_real) {a_real=new_real;};

        // Returns the Complex to a simple std::string
        virtual std::string to_mathml(Textual_Math_Settings* settings) const;
        std::string to_std_string_simple(unsigned int max_number_size, Textual_Math_Settings* settings) const;
        std::string to_std_string_simple(Textual_Math_Settings* settings) const {return to_std_string_simple(-1, settings);};
        virtual std::string to_std_string(Textual_Math_Settings* settings) const{return to_std_string_simple(-1, settings);};

        //*********
        //
        // Operator methods
        //
        //*********

        // Returns the conjugate of this Complex
        inline Complex conjugate() const {return Complex(a_real, a_imaginary * -1);};

        // Returns the inverse of this element
        virtual std::shared_ptr<__Field_Element> inverse(){return std::make_shared<Complex>(Complex(1) / (*this));};
        virtual std::shared_ptr<__Field_Element> opposite(){return std::make_shared<Complex>(*this * -1);};

        // Function to do operations with Complex
        // Adds an another Complex to this Complex
        void _add(Complex const& obj) {a_imaginary += obj.imaginary();a_real += obj.real();};
        Complex _add_without_modification(Complex const& obj) const {Complex new_complex = Complex(real() + obj.real(), imaginary() + obj.imaginary());return new_complex;};

        // Divides the Complex with a Complex
        void _divide(Complex const& obj);
        Complex _divide_without_modification(Complex const& obj) const;

        // Returns if this Complex is equal to another
        bool _equal(Complex const& obj) const {return obj.real() == real() && obj.imaginary() == imaginary();};

        // Multiplies the Complex with a Complex
        void _multiply(Complex const& obj);
        inline Complex _multiply_without_modification(Complex const& obj) const {return Complex(real() * obj.real() - (imaginary() * obj.imaginary()), real() * obj.imaginary() + imaginary() * obj.real());};

        // Multiplies the Complex with a double
        Complex _multiply_without_modification(Fraction const& obj) const { return Complex(real() * obj, imaginary() * obj); };
        Complex _multiply_without_modification(int const& obj) const { return Complex(real() * obj, imaginary() * obj); };
        Complex _multiply_without_modification(unsigned int const& obj) const { return Complex(real() * obj, imaginary() * obj); };

        // Substracts an another Complex to this Complex
        void _substract(Complex const& obj) {a_imaginary = imaginary() - obj.imaginary();a_real = real() - obj.real();};
        Complex _substract_without_modification(Complex const& obj) const {Complex new_complex = Complex(real() - obj.real(), imaginary() - obj.imaginary());return new_complex;};

        // Operator overloading with int
        // Equality operator
        bool operator==(int obj) { return _equal(obj); }
        bool operator!=(int obj) { return !_equal(obj); }
        // Multiplication operator
        Complex operator*(int obj) const { return _multiply_without_modification(obj); }
        // Multiplication operator
        Complex operator*(unsigned int obj) const { return _multiply_without_modification(obj); }

        // Operator overloading with Fraction
        // Minus operator
        Complex operator-(Fraction const& obj) { return _substract_without_modification(Complex(obj)); }
        // Minus operator assignment
        Complex& operator-=(Fraction const& obj) { _substract(Complex(obj)); return *this; }
        // Multiplication operator
        Complex operator*(Fraction const& obj) const { return _multiply_without_modification(obj); }
        // Plus operator
        Complex operator+(Fraction const& obj) { return _add_without_modification(Complex(obj)); };
        // Plus operator assignment
        Complex& operator+=(Fraction const& obj) { _add(Complex(obj)); return *this; }

        // Operator overloading with fractions
        // Decrement operator
        Complex& operator--(int) { _substract(Fraction(1)); return *this; }
        // Divide operator
        Complex operator/(Complex const& obj) const { return _divide_without_modification(obj); };
        // Divide operator assignment
        Complex& operator/=(Complex const& obj) { _divide(obj);return *this; };
        // Equality operator
        bool operator==(const Complex& obj) const { return _equal(obj); }
        // Increment operator
        Complex& operator++(int) { _add(Fraction(1)); return *this; }
        // Minus operator
        Complex operator-(Complex const& obj) const { return _substract_without_modification(obj); };
        // Minus operator assignment
        Complex& operator-=(const Complex& obj) { _substract(obj); return *this; }
        // Multiply operator assignment
        Complex operator*(Complex const& obj) const { return _multiply_without_modification(obj); };
        // Multiply operator
        Complex& operator*=(Complex const& obj) { _multiply(obj);return *this; };
        // Plus operator
        Complex operator+(Complex const& obj) const { return _add_without_modification(obj); };
        // Plus operator assignment
        Complex& operator+=(const Complex& obj) { _add(obj); return *this; }
    private:
        //*********
        //
        // Complex simple attributes
        //
        //*********

        // Imaginary part of the complex
        Fraction a_imaginary = Fraction(0);
        // Real part of the complex
        Fraction a_real = Fraction(0);
	};

	// Returns a complex from a std::string (indev)
    Complex string_to_complex(std::string source);

	// Multiplication operator
    Complex operator*(int obj_1, Complex obj);
	// Stream operator overloading (indev)
    std::ostream& operator<<(std::ostream& os, const Complex& obj);

    // Complex polynomial
    typedef __Monomonial_Template<Complex> __Monomonial;
    typedef Polynomial_Template<Complex> Polynomial;
    class Polynomial_Complex {
        public:
            // Polynomial_Complex constructor
            Polynomial_Complex(){a_polynomial.push_back(__Polynomial_Complex_Member());a_polynomial.push_back(__Polynomial_Complex_Member());a_polynomial[0].unknown = std::string("");a_polynomial[1].unknown = std::string("i");};

            // Conjugates the polynomial
            inline std::shared_ptr<Polynomial_Complex> conjugate(){std::shared_ptr<Polynomial_Complex> to_return = std::make_shared<Polynomial_Complex>(*this);to_return.get()->imaginary_polynomial()->multiply(scls::Complex(-1));return to_return;};
            // Converts a polynomial to a complex
            static std::shared_ptr<Polynomial_Complex> from_polynomial(Polynomial_Template<Complex>* polynomial);
            // Returns the imaginary polynomial
            inline Polynomial_Template<Complex>* imaginary_polynomial() const {for(int i = 0;i<static_cast<int>(a_polynomial.size());i++){if(a_polynomial[i].unknown == std::string("i")){return a_polynomial[i].polynomial.get();}}return 0;};
            // Returns the real polynomial
            inline Polynomial_Template<Complex>* real_polynomial() const {for(int i = 0;i<static_cast<int>(a_polynomial.size());i++){if(a_polynomial[i].unknown == std::string("")){return a_polynomial[i].polynomial.get();}}return 0;};
            // Returns the polynomial to an entire polynomial
            inline std::shared_ptr<Polynomial_Template<Complex>> to_polynomial() const {std::shared_ptr<Polynomial_Template<Complex>> to_return = std::make_shared<Polynomial_Template<Complex>>(*real_polynomial());to_return.get()->__add(imaginary_polynomial());to_return.get()->multiply(scls::Complex(0, 1));return to_return;};

        private:
            // Struct representating each members of the number
            struct __Polynomial_Complex_Member{std::shared_ptr<Polynomial_Template<Complex>>polynomial=std::make_shared<Polynomial_Template<Complex>>();std::string unknown = std::string();};

            // Each parts of the polynomial
            std::vector<__Polynomial_Complex_Member> a_polynomial;
        };

    // Complex formula
    typedef __Formula_Base_Template<Complex> __Formula;

    // Trigonometric complex
    struct Complex_Polar {
    	// Complex_Polar constructor
    	Complex_Polar(std::shared_ptr<__Formula_Base_Template<Complex>> needed_argument, std::shared_ptr<__Formula_Base_Template<Complex>> needed_modulus):a_argument(needed_argument),a_modulus(needed_modulus){};

    	// Converts the polar complex to an algebric complex
    	Complex to_complex() const {Complex c(scls::Fraction::from_double(std::cos(a_argument.get()->value_to_double())), scls::Fraction::from_double(std::sin(a_argument.get()->value_to_double())));return c * a_modulus.get()->value_to_fraction();};

    	// Getters and setters
    	__Formula_Base_Template<Complex>* argument(){return a_argument.get();};
    	__Formula_Base_Template<Complex>* modulus(){return a_modulus.get();};

    	// Module and argument
    	std::shared_ptr<__Formula_Base_Template<Complex>> a_argument;
    	std::shared_ptr<__Formula_Base_Template<Complex>> a_modulus;
    };

}


#endif /* SCLS_MATH_COMPLEX */
