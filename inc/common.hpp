#pragma once

#include <cmath>
#include <limits>

template<typename T>
concept FloatingPoint = std::floating_point<T>;

// ------------------------------------ Floating point comparsion ------------------------------------

template<FloatingPoint T>
inline bool FloatingPointLE(T left, T right, T epsilon = std::numeric_limits<T>::epsilon())
{
    T max_val = std::max({std::fabs(left), std::fabs(right), T(1.0)});
    return left < right + epsilon * max_val;
}

template<FloatingPoint T>
inline bool FloatingPointL(T left, T right, T epsilon = std::numeric_limits<T>::epsilon())
{
    T max_val = std::max({std::fabs(left), std::fabs(right), T(1.0)});
    return left < right - epsilon * max_val;
}

template<FloatingPoint T>
inline bool FloatingPointGE(T left, T right, T epsilon = std::numeric_limits<T>::epsilon())
{
    T max_val = std::max({std::fabs(left), std::fabs(right), T(1.0)});
    return left > right - epsilon * max_val;
}

template<FloatingPoint T>
inline bool FloatingPointG(T left, T right, T epsilon = std::numeric_limits<T>::epsilon())
{
    T max_val = std::max({std::fabs(left), std::fabs(right), T(1.0)});
    return left > right + epsilon * max_val;
}

template<FloatingPoint T>
inline bool FloatingPointE(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
{
    T max_val = std::max({std::fabs(a), std::fabs(b), T(1.0)});
    return std::fabs(a - b) < epsilon * max_val;
}

// ------------------------------------ istream utilites ---------------------------------------------

template <typename T>
inline bool fully_scanned_val(const std::string& token, T& val) {
    std::istringstream iss(token);
    iss >> std::noskipws >> val; 
    return iss && iss.eof(); 
}

template <typename T>
inline bool scan_until_next_line(std::istream &stream, T &val) {
    while (true) {
        std::string token;
        stream >> token;
        if (stream.eof()) {
            stream.clear();
            return false;
        }
        
        if (fully_scanned_val(token, val)) {
            return true;
        } else {
            stream.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // skip untill next line or EOF
        }
    }
}

// ---------------------------------------------------------------------------------------------------
