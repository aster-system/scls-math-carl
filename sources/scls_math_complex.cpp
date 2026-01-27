/*
 * scls_math_complex.cpp
 *
 *  Created on: 7 déc. 2025
 *      Author: mmatt
 */

// Include the good header file
#include "../scls_math.h"

//*********
//
// The Complex part
//
//*********

// The namespace "scls" is used to simplify the all.
namespace scls {

    // Returns the Complex to a simple std::string
    std::string Complex::to_mathml(Textual_Math_Settings* settings) const {
        if(imaginary() == 0){
            if(real().denominator() == 1){return to_std_string_simple(settings);}
            else if(real().numerator() == 1) {return std::string("/") + std::to_string(real().denominator());}
            else {return std::string("(") + std::to_string(real().numerator()) + std::string(")/") + std::to_string(real().denominator());}
        }
        else if(real() == 0){return to_std_string_simple(settings);}
        return std::string("(") + to_std_string_simple(settings) + std::string(")");
    }
    std::string Complex::to_std_string_simple(unsigned int max_number_size, Textual_Math_Settings* settings) const {
        // Asserts
        if(real() == 0 && imaginary() == 0 && settings != 0 && !settings->hide_if_0()){return std::string("0");}

        std::string to_return = "";
        if(real() != 0) {
            if(settings == 0 || settings->complex_double() == -1){to_return += real().to_std_string(max_number_size, settings) + " ";}
            else{to_return += format_number_to_text(real().to_double(), settings->complex_double()) + " ";}
        }
        if(imaginary() != 0) {
            if(imaginary() > 0) {to_return += "+ ";}
            else {to_return += "- ";}
            if(settings == 0 || settings->complex_double() == -1){to_return += imaginary().abs().to_std_string(max_number_size, settings) + "i";}
            else{to_return += format_number_to_text(std::abs(imaginary().to_double()), settings->complex_double()) + "i";}
        }
        while(to_return.size() > 0 && to_return[to_return.size() - 1] == ' ') to_return = to_return.substr(0, to_return.size() - 1);
        if(to_return == "") to_return = "0";
        return to_return;
    };

    //*********
    //
    // Operator methods
    //
    //*********

    // Divides the Complex with a Complex
    void Complex::_divide(Complex const& obj) {
        _multiply(obj.conjugate());
        Complex real_denominateur = obj * obj.conjugate();
        a_imaginary = a_imaginary / real_denominateur.imaginary();
        a_real = a_real / real_denominateur.real();
    };

    // Divides the Complex with a Complex
    Complex Complex::_divide_without_modification(Complex const& obj) const {
        // Simplifications
        if(a_imaginary == 0 && obj.a_imaginary == 0){return Complex(a_real / obj.a_real);}

        Complex new_complex = Complex(a_real, a_imaginary);
        new_complex._multiply(obj.conjugate());
        Complex real_denominateur = obj * obj.conjugate();
        new_complex.a_imaginary = new_complex.a_imaginary / real_denominateur.real();
        new_complex.a_real = new_complex.a_real / real_denominateur.real();
        return new_complex;
    };

    // Multiplies the Complex with a Complex
    void Complex::_multiply(Complex const& obj) {
        Fraction new_imaginary = real() * obj.imaginary() + imaginary() * obj.real();
        Fraction new_real = real() * obj.real() - (imaginary() * obj.imaginary());
        a_imaginary = new_imaginary;
        a_real = new_real;
    };

	// Returns a complex from a std::string (indev)
    Complex string_to_complex(std::string source) {
	    Complex to_return(Fraction(0));

	    // Cut the number by + operator
	    std::vector<std::string> cutted = cut_string(source, "+");
	    for(int i = 0;i<static_cast<int>(cutted.size());i++){if(cutted[i] == ""){cutted.erase(cutted.begin()+i);i--;}}
	    if(cutted.size() > 0) {
            // First part of a complex number
            bool is_imaginary = false;
            std::string& current_string = cutted[0];
            // Special cases
            if(current_string == "i") to_return.set_imaginary(Fraction(1));
            else {
                // Analyse the part
                if(current_string[0] == 'i') {is_imaginary = true;current_string=current_string.substr(1,current_string.size()-1);}
                else if(current_string[current_string.size()-1] == 'i') {is_imaginary = true;current_string=current_string.substr(0,current_string.size()-1);}
                // Get the number
                Fraction current_number = Fraction::from_std_string(current_string);
                if(is_imaginary) {
                    to_return.set_imaginary(current_number);
                } else {
                    to_return.set_real(current_number);
                }
            }
	    }

	    return to_return;
	};

	// Multiplciation operator
    Complex operator*(int obj_1, Complex obj){ return obj._multiply_without_modification(obj_1); };
	// Stream operator overloading (indev)
    std::ostream& operator<<(std::ostream& os, const Complex& obj){ os << "Complex : " << obj.real().to_double() << " + " << obj.imaginary().to_double() << "i" ; return os; };
}


