#pragma once
#include <type_traits>
#include <algorithm>
#include <windows.h>

struct zero_division {
	const char* operator()() const noexcept {
		return "Division by zero in Color" ;
	}
} ;

template <typename T> struct is_defined_Vec2_variants {
	static constexpr bool value = false ;
} ;

template <> struct is_defined_Vec2_variants<int> {
	static constexpr bool value = true ;
} ;

template <> struct is_defined_Vec2_variants<float> {
	static constexpr bool value = true ;
} ;

template <typename T> constexpr bool is_defined_Vec2_variants_v = is_defined_Vec2_variants<T>::value ;

template <typename T> struct Vec2 {
	static_assert(is_defined_Vec2_variants_v<T>, "undefined Vec2 variants!") ;
} ;

template <> struct Vec2<int> {
	int x, y ;
	inline Vec2() noexcept ;
	template <typename T> Vec2(T n) ;
	template <typename T> Vec2(T x, T y) ;
	template <typename T> Vec2(const Vec2<T>& other) ;
	template <typename T> Vec2& operator=(const Vec2<T>& other) ;
	template <typename T> Vec2 operator+(const Vec2<T>& other) ;
	template <typename T> Vec2 operator-(const Vec2<T>& other) ;
	template <typename T> Vec2 operator*(const Vec2<T>& other) ;
	template <typename T> Vec2 operator/(const Vec2<T>& other) ;
	template <typename T> Vec2& operator+=(const Vec2<T>& other) ;
	template <typename T> Vec2& operator-=(const Vec2<T>& other) ;
	template <typename T> Vec2& operator*=(const Vec2<T>& other) ;
	template <typename T> Vec2& operator/=(const Vec2<T>& other) ;
	template <typename T> Vec2& operator=(const T& val) ;
	template <typename T> Vec2 operator+(const T& val) ;
	template <typename T> Vec2 operator-(const T& val) ;
	template <typename T> Vec2 operator*(const T& val) ;
	template <typename T> Vec2 operator/(const T& val) ;
	template <typename T> Vec2& operator+=(const T& val) ;
	template <typename T> Vec2& operator-=(const T& val) ;
	template <typename T> Vec2& operator*=(const T& val) ;
	template <typename T> Vec2& operator/=(const T& val) ;
	inline bool operator==(const Vec2& other) ;
	inline bool operator!=(const Vec2& other) ;
	inline operator Vec2<float>() const noexcept ;
} ;

template <> struct Vec2<float> {
	float x, y ;
	inline Vec2() noexcept ;
	template <typename T> Vec2(T n) ;
	template <typename T> Vec2(T x, T y) ;
	template <typename T> Vec2(const Vec2<T>& other) ;
	template <typename T> Vec2& operator=(const Vec2<T>& other) ;
	template <typename T> Vec2 operator+(const Vec2<T>& other) ;
	template <typename T> Vec2 operator-(const Vec2<T>& other) ;
	template <typename T> Vec2 operator*(const Vec2<T>& other) ;
	template <typename T> Vec2 operator/(const Vec2<T>& other) ;
	template <typename T> Vec2& operator+=(const Vec2<T>& other) ;
	template <typename T> Vec2& operator-=(const Vec2<T>& other) ;
	template <typename T> Vec2& operator*=(const Vec2<T>& other) ;
	template <typename T> Vec2& operator/=(const Vec2<T>& other) ;
	template <typename T> Vec2& operator=(const T& val) ;
	template <typename T> Vec2 operator+(const T& val) ;
	template <typename T> Vec2 operator-(const T& val) ;
	template <typename T> Vec2 operator*(const T& val) ;
	template <typename T> Vec2 operator/(const T& val) ;
	template <typename T> Vec2& operator+=(const T& val) ;
	template <typename T> Vec2& operator-=(const T& val) ;
	template <typename T> Vec2& operator*=(const T& val) ;
	template <typename T> Vec2& operator/=(const T& val) ;
	inline bool operator==(const Vec2& other) ;
	inline bool operator!=(const Vec2& other) ;
	inline operator Vec2<int>() const noexcept ;
} ;

// Vec2<int> implementation

Vec2<int>::Vec2() noexcept {
	x = y = 0 ;
}

template <typename T> Vec2<int>::Vec2(T n) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	x = y = static_cast<int>(n) ;
}

template <typename T> Vec2<int>::Vec2(T x, T y) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	this->x = static_cast<int>(x) ;
	this->y = static_cast<int>(y) ;
}

template <typename T> Vec2<int>::Vec2(const Vec2<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	x = static_cast<int>(other.x) ;
	y = static_cast<int>(other.y) ;
}

template <typename T> Vec2<int>& Vec2<int>::operator=(const Vec2<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	x = static_cast<int>(other.x) ;
	y = static_cast<int>(other.y) ;
	return *this ;
}

template <typename T> Vec2<int> Vec2<int>::operator+(const Vec2<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	return {x + static_cast<int>(other.x), y + static_cast<int>(other.y)} ;
}

template <typename T> Vec2<int> Vec2<int>::operator-(const Vec2<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	return {x - static_cast<int>(other.x), y - static_cast<int>(other.y)} ;
}

template <typename T> Vec2<int> Vec2<int>::operator*(const Vec2<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	return {x * static_cast<int>(other.x), y * static_cast<int>(other.y)} ;
}

template <typename T> Vec2<int> Vec2<int>::operator/(const Vec2<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	if(other.x == 0 || other.y == 0) throw zero_division() ;
	return {x / static_cast<int>(other.x), y / static_cast<int>(other.y)} ;
}

template <typename T> Vec2<int>& Vec2<int>::operator+=(const Vec2<T>& other) {
	*this = *this + other ;
	return *this ;
}

template <typename T> Vec2<int>& Vec2<int>::operator-=(const Vec2<T>& other) {
	*this = *this - other ;
	return *this ;
}

template <typename T> Vec2<int>& Vec2<int>::operator*=(const Vec2<T>& other) {
	*this = *this * other ;
	return *this ;
}

template <typename T> Vec2<int>& Vec2<int>::operator/=(const Vec2<T>& other) {
	*this = *this / other ;
	return *this ;
}

template <typename T> Vec2<int>& Vec2<int>::operator=(const T& val) {
	*this = Vec2<int>(static_cast<int>(val)) ;
	return *this ;
}

template <typename T> Vec2<int> Vec2<int>::operator+(const T& val) {
	return *this + Vec2<int>(static_cast<int>(val)) ;
}

template <typename T> Vec2<int> Vec2<int>::operator-(const T& val) {
	return *this - Vec2<int>(static_cast<int>(val)) ;
}

template <typename T> Vec2<int> Vec2<int>::operator*(const T& val) {
	return *this * Vec2<int>(static_cast<int>(val)) ;
}

template <typename T> Vec2<int> Vec2<int>::operator/(const T& val) {
	return *this / Vec2<int>(static_cast<int>(val)) ;
}

template <typename T> Vec2<int>& Vec2<int>::operator+=(const T& val) {
	*this += Vec2<int>(static_cast<int>(val)) ;
	return *this ;
}

template <typename T> Vec2<int>& Vec2<int>::operator-=(const T& val) {
	*this -= Vec2<int>(static_cast<int>(val)) ;
	return *this ;
}

template <typename T> Vec2<int>& Vec2<int>::operator*=(const T& val) {
	*this *= Vec2<int>(static_cast<int>(val)) ;
	return *this ;
}

template <typename T> Vec2<int>& Vec2<int>::operator/=(const T& val) {
	*this /= Vec2<int>(static_cast<int>(val)) ;
	return *this ;
}

bool Vec2<int>::operator==(const Vec2<int>& other) {
	return (x == other.x && y == other.y) ;
}

bool Vec2<int>::operator!=(const Vec2<int>& other) {
	return !(*this == other) ;
}

Vec2<int>::operator Vec2<float>() const noexcept {
	return {static_cast<float>(x), static_cast<float>(y)} ;
}

// Vec2<float> implementation

Vec2<float>::Vec2() noexcept {
	x = y = 0.0f ;
}

template <typename T> Vec2<float>::Vec2(T n) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	x = y = static_cast<float>(n) ;
}

template <typename T> Vec2<float>::Vec2(T x, T y) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	this->x = static_cast<float>(x) ;
	this->y = static_cast<float>(y) ;
}

template <typename T> Vec2<float>::Vec2(const Vec2<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	x = static_cast<float>(other.x) ;
	y = static_cast<float>(other.y) ;
}

template <typename T> Vec2<float>& Vec2<float>::operator=(const Vec2<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	x = static_cast<float>(other.x) ;
	y = static_cast<float>(other.y) ;
	return *this ;
}

template <typename T> Vec2<float> Vec2<float>::operator+(const Vec2<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	return {x + static_cast<float>(other.x), y + static_cast<float>(other.y)} ;
}

template <typename T> Vec2<float> Vec2<float>::operator-(const Vec2<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	return {x - static_cast<float>(other.x), y - static_cast<float>(other.y)} ;
}

template <typename T> Vec2<float> Vec2<float>::operator*(const Vec2<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	return {x * static_cast<float>(other.x), y * static_cast<float>(other.y)} ;
}

template <typename T> Vec2<float> Vec2<float>::operator/(const Vec2<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Vec2 variant!") ;
	if(other.x == 0 || other.y == 0) throw zero_division() ;
	return {x / static_cast<float>(other.x), y / static_cast<float>(other.y)} ;
}

template <typename T> Vec2<float>& Vec2<float>::operator+=(const Vec2<T>& other) {
	*this = *this + other ;
	return *this ;
}

template <typename T> Vec2<float>& Vec2<float>::operator-=(const Vec2<T>& other) {
	*this = *this - other ;
	return *this ;
}

template <typename T> Vec2<float>& Vec2<float>::operator*=(const Vec2<T>& other) {
	*this = *this * other ;
	return *this ;
}

template <typename T> Vec2<float>& Vec2<float>::operator/=(const Vec2<T>& other) {
	*this = *this / other ;
	return *this ;
}

template <typename T> Vec2<float>& Vec2<float>::operator=(const T& val) {
	*this = Vec2<float>(static_cast<float>(val)) ;
	return *this ;
}

template <typename T> Vec2<float> Vec2<float>::operator+(const T& val) {
	return *this + Vec2<float>(static_cast<float>(val)) ;
}

template <typename T> Vec2<float> Vec2<float>::operator-(const T& val) {
	return *this - Vec2<float>(static_cast<float>(val)) ;
}

template <typename T> Vec2<float> Vec2<float>::operator*(const T& val) {
	return *this * Vec2<float>(static_cast<float>(val)) ;
}

template <typename T> Vec2<float> Vec2<float>::operator/(const T& val) {
	return *this / Vec2<float>(static_cast<float>(val)) ;
}

template <typename T> Vec2<float>& Vec2<float>::operator+=(const T& val) {
	*this += Vec2<float>(static_cast<float>(val)) ;
	return *this ;
}

template <typename T> Vec2<float>& Vec2<float>::operator-=(const T& val) {
	*this -= Vec2<float>(static_cast<float>(val)) ;
	return *this ;
}

template <typename T> Vec2<float>& Vec2<float>::operator*=(const T& val) {
	*this *= Vec2<float>(static_cast<float>(val)) ;
	return *this ;
}

template <typename T> Vec2<float>& Vec2<float>::operator/=(const T& val) {
	*this /= Vec2<float>(static_cast<float>(val)) ;
	return *this ;
}

bool Vec2<float>::operator==(const Vec2<float>& other) {
	return (x == other.x && y == other.y) ;
}

bool Vec2<float>::operator!=(const Vec2<float>& other) {
	return !(*this == other) ;
}

Vec2<float>::operator Vec2<int>() const noexcept {
	return {static_cast<int>(x), static_cast<int>(y)} ;
}

template <typename T> struct is_defined_Rect_variants {
	static constexpr bool value = false ;
} ;

template <> struct is_defined_Rect_variants<int> {
	static constexpr bool value = true ;
} ;

template <> struct is_defined_Rect_variants<float> {
	static constexpr bool value = true ;
} ;

template <typename T> constexpr bool is_defined_Rect_variants_v = is_defined_Rect_variants<T>::value ;

template <typename T> struct Rect {
	static_assert(is_defined_Rect_variants_v<T>, "undefined Rect variants!") ;
} ;

template <> struct Rect<int> {
	int x, y, w, h ;
	inline Rect() noexcept ;
	template <typename T> Rect(T n) ;
	template <typename T> Rect(T x, T y, T w, T h) ;
	template <typename T> Rect(const Rect<T>& other) ;
	template <typename T> Rect& operator=(const Rect<T>& other) ;
	template <typename T> Rect operator+(const Rect<T>& other) ;
	template <typename T> Rect operator-(const Rect<T>& other) ;
	template <typename T> Rect operator*(const Rect<T>& other) ;
	template <typename T> Rect operator/(const Rect<T>& other) ;
	template <typename T> Rect& operator+=(const Rect<T>& other) ;
	template <typename T> Rect& operator-=(const Rect<T>& other) ;
	template <typename T> Rect& operator*=(const Rect<T>& other) ;
	template <typename T> Rect& operator/=(const Rect<T>& other) ;
	template <typename T> Rect& operator=(const T& val) ;
	template <typename T> Rect operator+(const T& val) ;
	template <typename T> Rect operator-(const T& val) ;
	template <typename T> Rect operator*(const T& val) ;
	template <typename T> Rect operator/(const T& val) ;
	template <typename T> Rect& operator+=(const T& val) ;
	template <typename T> Rect& operator-=(const T& val) ;
	template <typename T> Rect& operator*=(const T& val) ;
	template <typename T> Rect& operator/=(const T& val) ;
	inline bool operator==(const Rect& other) ;
	inline bool operator!=(const Rect& other) ;
	inline operator Rect<float>() const noexcept ;
} ;

template <> struct Rect<float> {
	float x, y, w, h ;
	inline Rect() noexcept ;
	template <typename T> Rect(T n) ;
	template <typename T> Rect(T x, T y, T w, T h) ;
	template <typename T> Rect(const Rect<T>& other) ;
	template <typename T> Rect& operator=(const Rect<T>& other) ;
	template <typename T> Rect operator+(const Rect<T>& other) ;
	template <typename T> Rect operator-(const Rect<T>& other) ;
	template <typename T> Rect operator*(const Rect<T>& other) ;
	template <typename T> Rect operator/(const Rect<T>& other) ;
	template <typename T> Rect& operator+=(const Rect<T>& other) ;
	template <typename T> Rect& operator-=(const Rect<T>& other) ;
	template <typename T> Rect& operator*=(const Rect<T>& other) ;
	template <typename T> Rect& operator/=(const Rect<T>& other) ;
	template <typename T> Rect& operator=(const T& val) ;
	template <typename T> Rect operator+(const T& val) ;
	template <typename T> Rect operator-(const T& val) ;
	template <typename T> Rect operator*(const T& val) ;
	template <typename T> Rect operator/(const T& val) ;
	template <typename T> Rect& operator+=(const T& val) ;
	template <typename T> Rect& operator-=(const T& val) ;
	template <typename T> Rect& operator*=(const T& val) ;
	template <typename T> Rect& operator/=(const T& val) ;
	inline bool operator==(const Rect& other) ;
	inline bool operator!=(const Rect& other) ;
	inline operator Rect<int>() const noexcept ;
} ;

// Rect<int> implementation

Rect<int>::Rect() noexcept {
	x = y = w = h = 0 ;
}

template <typename T> Rect<int>::Rect(T n) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	x = y = w = h = static_cast<int>(n) ;
}

template <typename T> Rect<int>::Rect(T x, T y, T w, T h) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	this->x = static_cast<int>(x) ;
	this->y = static_cast<int>(y) ;
	this->w = static_cast<int>(w) ;
	this->h = static_cast<int>(h) ;
}

template <typename T> Rect<int>::Rect(const Rect<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	x = static_cast<int>(other.x) ;
	y = static_cast<int>(other.y) ;
	w = static_cast<int>(other.w) ;
	h = static_cast<int>(other.h) ;
}

template <typename T> Rect<int>& Rect<int>::operator=(const Rect<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	x = static_cast<int>(other.x) ;
	y = static_cast<int>(other.y) ;
	w = static_cast<int>(other.w) ;
	h = static_cast<int>(other.h) ;
	return *this ;
}

template <typename T> Rect<int> Rect<int>::operator+(const Rect<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	return {x + static_cast<int>(other.x), y + static_cast<int>(other.y), w + static_cast<int>(other.w), h + static_cast<int>(other.h)} ;
}

template <typename T> Rect<int> Rect<int>::operator-(const Rect<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	return {x - static_cast<int>(other.x), y - static_cast<int>(other.y), w - static_cast<int>(other.w), h - static_cast<int>(other.h)} ;
}

template <typename T> Rect<int> Rect<int>::operator*(const Rect<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	return {x * static_cast<int>(other.x), y * static_cast<int>(other.y), w * static_cast<int>(other.w), h * static_cast<int>(other.h)} ;
}

template <typename T> Rect<int> Rect<int>::operator/(const Rect<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	if(other.x == 0 || other.y == 0) throw zero_division() ;
	return {x / static_cast<int>(other.x), y / static_cast<int>(other.y), w / static_cast<int>(other.w), h / static_cast<int>(other.h)} ;
}

template <typename T> Rect<int>& Rect<int>::operator+=(const Rect<T>& other) {
	*this = *this + other ;
	return *this ;
}

template <typename T> Rect<int>& Rect<int>::operator-=(const Rect<T>& other) {
	*this = *this - other ;
	return *this ;
}

template <typename T> Rect<int>& Rect<int>::operator*=(const Rect<T>& other) {
	*this = *this * other ;
	return *this ;
}

template <typename T> Rect<int>& Rect<int>::operator/=(const Rect<T>& other) {
	*this = *this / other ;
	return *this ;
}

template <typename T> Rect<int>& Rect<int>::operator=(const T& val) {
	*this = Rect<int>(static_cast<int>(val)) ;
	return *this ;
}

template <typename T> Rect<int> Rect<int>::operator+(const T& val) {
	return *this + Rect<int>(static_cast<int>(val)) ;
}

template <typename T> Rect<int> Rect<int>::operator-(const T& val) {
	return *this - Rect<int>(static_cast<int>(val)) ;
}

template <typename T> Rect<int> Rect<int>::operator*(const T& val) {
	return *this * Rect<int>(static_cast<int>(val)) ;
}

template <typename T> Rect<int> Rect<int>::operator/(const T& val) {
	return *this / Rect<int>(static_cast<int>(val)) ;
}

template <typename T> Rect<int>& Rect<int>::operator+=(const T& val) {
	*this += Rect<int>(static_cast<int>(val)) ;
	return *this ;
}

template <typename T> Rect<int>& Rect<int>::operator-=(const T& val) {
	*this -= Rect<int>(static_cast<int>(val)) ;
	return *this ;
}

template <typename T> Rect<int>& Rect<int>::operator*=(const T& val) {
	*this *= Rect<int>(static_cast<int>(val)) ;
	return *this ;
}

template <typename T> Rect<int>& Rect<int>::operator/=(const T& val) {
	*this /= Rect<int>(static_cast<int>(val)) ;
	return *this ;
}

bool Rect<int>::operator==(const Rect<int>& other) {
	return (x == other.x && y == other.y && w == other.w && h == other.h) ;
}

bool Rect<int>::operator!=(const Rect<int>& other) {
	return !(*this == other) ;
}

Rect<int>::operator Rect<float>() const noexcept {
	return {static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h)} ;
}

// Rect<float> implementation

Rect<float>::Rect() noexcept {
	x = y = w = h = 0 ;
}

template <typename T> Rect<float>::Rect(T n) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	x = y = w = h = static_cast<float>(n) ;
}

template <typename T> Rect<float>::Rect(T x, T y, T w, T h) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	this->x = static_cast<float>(x) ;
	this->y = static_cast<float>(y) ;
	this->w = static_cast<float>(w) ;
	this->h = static_cast<float>(h) ;
}

template <typename T> Rect<float>::Rect(const Rect<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	x = static_cast<float>(other.x) ;
	y = static_cast<float>(other.y) ;
	w = static_cast<float>(other.w) ;
	h = static_cast<float>(other.h) ;
}

template <typename T> Rect<float>& Rect<float>::operator=(const Rect<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	x = static_cast<float>(other.x) ;
	y = static_cast<float>(other.y) ;
	w = static_cast<float>(other.w) ;
	h = static_cast<float>(other.h) ;
	return *this ;
}

template <typename T> Rect<float> Rect<float>::operator+(const Rect<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	return {x + static_cast<float>(other.x), y + static_cast<float>(other.y), w + static_cast<float>(other.w), h + static_cast<float>(other.h)} ;
}

template <typename T> Rect<float> Rect<float>::operator-(const Rect<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	return {x - static_cast<float>(other.x), y - static_cast<float>(other.y), w - static_cast<float>(other.w), h - static_cast<float>(other.h)} ;
}

template <typename T> Rect<float> Rect<float>::operator*(const Rect<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	return {x * static_cast<float>(other.x), y * static_cast<float>(other.y), w * static_cast<float>(other.w), h * static_cast<float>(other.h)} ;
}

template <typename T> Rect<float> Rect<float>::operator/(const Rect<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Rect variant!") ;
	if(other.x == 0 || other.y == 0) throw zero_division() ;
	return {x / static_cast<float>(other.x), y / static_cast<float>(other.y), w / static_cast<float>(other.w), h / static_cast<float>(other.h)} ;
}

template <typename T> Rect<float>& Rect<float>::operator+=(const Rect<T>& other) {
	*this = *this + other ;
	return *this ;
}

template <typename T> Rect<float>& Rect<float>::operator-=(const Rect<T>& other) {
	*this = *this - other ;
	return *this ;
}

template <typename T> Rect<float>& Rect<float>::operator*=(const Rect<T>& other) {
	*this = *this * other ;
	return *this ;
}

template <typename T> Rect<float>& Rect<float>::operator/=(const Rect<T>& other) {
	*this = *this / other ;
	return *this ;
}

template <typename T> Rect<float>& Rect<float>::operator=(const T& val) {
	*this = Rect<float>(static_cast<float>(val)) ;
	return *this ;
}

template <typename T> Rect<float> Rect<float>::operator+(const T& val) {
	return *this + Rect<float>(static_cast<float>(val)) ;
}

template <typename T> Rect<float> Rect<float>::operator-(const T& val) {
	return *this - Rect<float>(static_cast<float>(val)) ;
}

template <typename T> Rect<float> Rect<float>::operator*(const T& val) {
	return *this * Rect<float>(static_cast<float>(val)) ;
}

template <typename T> Rect<float> Rect<float>::operator/(const T& val) {
	return *this / Rect<float>(static_cast<float>(val)) ;
}

template <typename T> Rect<float>& Rect<float>::operator+=(const T& val) {
	*this += Rect<float>(static_cast<float>(val)) ;
	return *this ;
}

template <typename T> Rect<float>& Rect<float>::operator-=(const T& val) {
	*this -= Rect<float>(static_cast<float>(val)) ;
	return *this ;
}

template <typename T> Rect<float>& Rect<float>::operator*=(const T& val) {
	*this *= Rect<float>(static_cast<float>(val)) ;
	return *this ;
}

template <typename T> Rect<float>& Rect<float>::operator/=(const T& val) {
	*this /= Rect<float>(static_cast<float>(val)) ;
	return *this ;
}

bool Rect<float>::operator==(const Rect<float>& other) {
	return (x == other.x && y == other.y && w == other.w && h == other.h) ;
}

bool Rect<float>::operator!=(const Rect<float>& other) {
	return !(*this == other) ;
}

Rect<float>::operator Rect<int>() const noexcept {
	return {static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h)} ;
}

template <typename T> struct is_defined_Color_variants {
	static constexpr bool value = false ;
} ;

template <> struct is_defined_Color_variants<unsigned char> {
	static constexpr bool value = true ;
} ;

template <> struct is_defined_Color_variants<float> {
	static constexpr bool value = true ;
} ;

template <typename T> constexpr bool is_defined_Color_variants_v = is_defined_Color_variants<T>::value ;

template <typename T> struct Color {
	static_assert(is_defined_Color_variants_v<T>, "undefined Color variants!") ;
} ;

template <> struct Color<unsigned char> {
	unsigned char r, g, b, a ;
	inline Color() noexcept ;
	template <typename T> Color(T n) ;
	template <typename T> Color(T r, T g, T b, T a) ;
	template <typename T> Color(const Color<T>& other) ;
	template <typename T> Color& operator=(const Color<T>& other) ;
	template <typename T> Color operator+(const Color<T>& other) ;
	template <typename T> Color operator-(const Color<T>& other) ;
	template <typename T> Color operator*(const Color<T>& other) ;
	template <typename T> Color operator/(const Color<T>& other) ;
	template <typename T> Color& operator+=(const Color<T>& other) ;
	template <typename T> Color& operator-=(const Color<T>& other) ;
	template <typename T> Color& operator*=(const Color<T>& other) ;
	template <typename T> Color& operator/=(const Color<T>& other) ;
	template <typename T> Color& operator=(const T& val) ;
	template <typename T> Color operator+(const T& val) ;
	template <typename T> Color operator-(const T& val) ;
	template <typename T> Color operator*(const T& val) ;
	template <typename T> Color operator/(const T& val) ;
	template <typename T> Color& operator+=(const T& val) ;
	template <typename T> Color& operator-=(const T& val) ;
	template <typename T> Color& operator*=(const T& val) ;
	template <typename T> Color& operator/=(const T& val) ;
	inline bool operator==(const Color& other) ;
	inline bool operator!=(const Color& other) ;
	inline operator Color<float>() const noexcept ;
} ;

template <> struct Color<float> {
	float r, g, b, a ;
	inline Color() noexcept ;
	template <typename T> Color(T n) ;
	template <typename T> Color(T r, T g, T b, T a) ;
	template <typename T> Color(const Color<T>& other) ;
	template <typename T> Color& operator=(const Color<T>& other) ;
	template <typename T> Color operator+(const Color<T>& other) ;
	template <typename T> Color operator-(const Color<T>& other) ;
	template <typename T> Color operator*(const Color<T>& other) ;
	template <typename T> Color operator/(const Color<T>& other) ;
	template <typename T> Color& operator+=(const Color<T>& other) ;
	template <typename T> Color& operator-=(const Color<T>& other) ;
	template <typename T> Color& operator*=(const Color<T>& other) ;
	template <typename T> Color& operator/=(const Color<T>& other) ;
	template <typename T> Color& operator=(const T& val) ;
	template <typename T> Color operator+(const T& val) ;
	template <typename T> Color operator-(const T& val) ;
	template <typename T> Color operator*(const T& val) ;
	template <typename T> Color operator/(const T& val) ;
	template <typename T> Color& operator+=(const T& val) ;
	template <typename T> Color& operator-=(const T& val) ;
	template <typename T> Color& operator*=(const T& val) ;
	template <typename T> Color& operator/=(const T& val) ;
	inline bool operator==(const Color& other) ;
	inline bool operator!=(const Color& other) ;
	inline operator Color<unsigned char>() const noexcept ;
} ;

// Color<unsigned char> implementation

Color<unsigned char>::Color() noexcept {
	r = g = b = a = 0 ;
}

template <typename T> Color<unsigned char>::Color(T n) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	r = g = b = a = static_cast<unsigned char>(std::clamp(n, 0, 255)) ;
}

template <typename T> Color<unsigned char>::Color(T r, T g, T b, T a) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	this->r = static_cast<unsigned char>(std::clamp<T>(r, 0, 255)) ;
	this->g = static_cast<unsigned char>(std::clamp<T>(g, 0, 255)) ;
	this->b = static_cast<unsigned char>(std::clamp<T>(b, 0, 255)) ;
	this->a = static_cast<unsigned char>(std::clamp<T>(a, 0, 255)) ;
}

template <typename T> Color<unsigned char>::Color(const Color<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	r = static_cast<unsigned char>(std::clamp(other.r, 0, 255)) ;
	g = static_cast<unsigned char>(std::clamp(other.g, 0, 255)) ;
	b = static_cast<unsigned char>(std::clamp(other.b, 0, 255)) ;
	a = static_cast<unsigned char>(std::clamp(other.a, 0, 255)) ;
}

template <typename T> Color<unsigned char>& Color<unsigned char>::operator=(const Color<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	r = static_cast<unsigned char>(std::clamp(other.r, 0, 255)) ;
	g = static_cast<unsigned char>(std::clamp(other.g, 0, 255)) ;
	b = static_cast<unsigned char>(std::clamp(other.b, 0, 255)) ;
	a = static_cast<unsigned char>(std::clamp(other.a, 0, 255)) ;
	return *this ;
}

template <typename T> Color<unsigned char> Color<unsigned char>::operator+(const Color<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	return {
		std::clamp(r + static_cast<unsigned char>(other.r), 0, 255), 
		std::clamp(g + static_cast<unsigned char>(other.g), 0, 255), 
		std::clamp(b + static_cast<unsigned char>(other.b), 0, 255), 
		std::clamp(a + static_cast<unsigned char>(other.a), 0, 255)
	} ;
}

template <typename T> Color<unsigned char> Color<unsigned char>::operator-(const Color<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	return {
		std::clamp(r - static_cast<unsigned char>(other.r), 0, 255), 
		std::clamp(g - static_cast<unsigned char>(other.g), 0, 255), 
		std::clamp(b - static_cast<unsigned char>(other.b), 0, 255), 
		std::clamp(a - static_cast<unsigned char>(other.a), 0, 255)
	} ;
}

template <typename T> Color<unsigned char> Color<unsigned char>::operator*(const Color<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	return {
		std::clamp(r * static_cast<unsigned char>(other.r), 0, 255), 
		std::clamp(g * static_cast<unsigned char>(other.g), 0, 255), 
		std::clamp(b * static_cast<unsigned char>(other.b), 0, 255), 
		std::clamp(a * static_cast<unsigned char>(other.a), 0, 255)
	} ;
}

template <typename T> Color<unsigned char> Color<unsigned char>::operator/(const Color<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	if(other.r == 0 || other.g == 0 || other.b == 0 || other.a == 0) throw zero_division() ;
		return {
			std::clamp(r / static_cast<unsigned char>(other.r), 0, 255), 
			std::clamp(g / static_cast<unsigned char>(other.g), 0, 255), 
			std::clamp(b / static_cast<unsigned char>(other.b), 0, 255), 
			std::clamp(a / static_cast<unsigned char>(other.a), 0, 255)
		} ;
}

template <typename T> Color<unsigned char>& Color<unsigned char>::operator+=(const Color<T>& other) {
	*this = *this + other ;
	return *this ;
}

template <typename T> Color<unsigned char>& Color<unsigned char>::operator-=(const Color<T>& other) {
	*this = *this - other ;
	return *this ;
}

template <typename T> Color<unsigned char>& Color<unsigned char>::operator*=(const Color<T>& other) {
	*this = *this * other ;
	return *this ;
}

template <typename T> Color<unsigned char>& Color<unsigned char>::operator/=(const Color<T>& other) {
	*this = *this / other ;
	return *this ;
}

template <typename T> Color<unsigned char>& Color<unsigned char>::operator=(const T& val) {
	*this = Color<unsigned char>(static_cast<unsigned char>(val)) ;
	return *this ;
}

template <typename T> Color<unsigned char> Color<unsigned char>::operator+(const T& val) {
	return *this + Color<unsigned char>(static_cast<unsigned char>(val)) ;
}

template <typename T> Color<unsigned char> Color<unsigned char>::operator-(const T& val) {
	return *this - Color<unsigned char>(static_cast<unsigned char>(val)) ;
}

template <typename T> Color<unsigned char> Color<unsigned char>::operator*(const T& val) {
	return *this * Color<unsigned char>(static_cast<unsigned char>(val)) ;
}

template <typename T> Color<unsigned char> Color<unsigned char>::operator/(const T& val) {
	return *this / Color<unsigned char>(static_cast<unsigned char>(val)) ;
}

template <typename T> Color<unsigned char>& Color<unsigned char>::operator+=(const T& val) {
	*this += Color<unsigned char>(static_cast<unsigned char>(val)) ;
	return *this ;
}

template <typename T> Color<unsigned char>& Color<unsigned char>::operator-=(const T& val) {
	*this -= Color<unsigned char>(static_cast<unsigned char>(val)) ;
	return *this ;
}

template <typename T> Color<unsigned char>& Color<unsigned char>::operator*=(const T& val) {
	*this *= Color<unsigned char>(static_cast<unsigned char>(val)) ;
	return *this ;
}

template <typename T> Color<unsigned char>& Color<unsigned char>::operator/=(const T& val) {
	*this /= Color<unsigned char>(static_cast<unsigned char>(val)) ;
	return *this ;
}

bool Color<unsigned char>::operator==(const Color<unsigned char>& other) {
	return (
		r == other.r && 
		g == other.g && 
		b == other.b && 
		a == other.a
	) ;
}

bool Color<unsigned char>::operator!=(const Color<unsigned char>& other) {
	return !(*this == other) ;
}

Color<unsigned char>::operator Color<float>() const noexcept {
	return {
		std::clamp(static_cast<float>(r) / 255.0f, 0.0f, 1.0f), 
		std::clamp(static_cast<float>(g) / 255.0f, 0.0f, 1.0f), 
		std::clamp(static_cast<float>(b) / 255.0f, 0.0f, 1.0f), 
		std::clamp(static_cast<float>(a) / 255.0f, 0.0f, 1.0f)
	} ;
}

// Color<float> implementation

Color<float>::Color() noexcept {
	r = g = b = a = 0 ;
}

template <typename T> Color<float>::Color(T n) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	r = g = b = a = static_cast<float>(std::clamp(n, 0.0f, 1.0f)) ;
}

template <typename T> Color<float>::Color(T r, T g, T b, T a) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	this->r = static_cast<float>(std::clamp(r, 0.0f, 1.0f)) ;
	this->g = static_cast<float>(std::clamp(g, 0.0f, 1.0f)) ;
	this->b = static_cast<float>(std::clamp(b, 0.0f, 1.0f)) ;
	this->a = static_cast<float>(std::clamp(a, 0.0f, 1.0f)) ;
}

template <typename T> Color<float>::Color(const Color<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	r = static_cast<float>(std::clamp(other.r, 0.0f, 1.0f)) ;
	g = static_cast<float>(std::clamp(other.g, 0.0f, 1.0f)) ;
	b = static_cast<float>(std::clamp(other.b, 0.0f, 1.0f)) ;
	a = static_cast<float>(std::clamp(other.a, 0.0f, 1.0f)) ;
}

template <typename T> Color<float>& Color<float>::operator=(const Color<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	r = static_cast<float>(std::clamp(other.r, 0.0f, 1.0f)) ;
	g = static_cast<float>(std::clamp(other.g, 0.0f, 1.0f)) ;
	b = static_cast<float>(std::clamp(other.b, 0.0f, 1.0f)) ;
	a = static_cast<float>(std::clamp(other.a, 0.0f, 1.0f)) ;
	return *this ;
}

template <typename T> Color<float> Color<float>::operator+(const Color<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	return {
		std::clamp(r + static_cast<float>(other.r), 0.0f, 1.0f), 
		std::clamp(g + static_cast<float>(other.g), 0.0f, 1.0f), 
		std::clamp(b + static_cast<float>(other.b), 0.0f, 1.0f), 
		std::clamp(a + static_cast<float>(other.a), 0.0f, 1.0f)
	} ;
}

template <typename T> Color<float> Color<float>::operator-(const Color<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	return {
		std::clamp(r - static_cast<float>(other.r), 0.0f, 1.0f), 
		std::clamp(g - static_cast<float>(other.g), 0.0f, 1.0f), 
		std::clamp(b - static_cast<float>(other.b), 0.0f, 1.0f), 
		std::clamp(a - static_cast<float>(other.a), 0.0f, 1.0f)
	} ;
}

template <typename T> Color<float> Color<float>::operator*(const Color<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	return {
		std::clamp(r * static_cast<float>(other.r), 0.0f, 1.0f), 
		std::clamp(g * static_cast<float>(other.g), 0.0f, 1.0f), 
		std::clamp(b * static_cast<float>(other.b), 0.0f, 1.0f), 
		std::clamp(a * static_cast<float>(other.a), 0.0f, 1.0f)
	} ;
}

template <typename T> Color<float> Color<float>::operator/(const Color<T>& other) {
	static_assert(std::is_arithmetic_v<T>, "undefined Color variant!") ;
	if(other.r == 0.0f || other.g == 0.0f || other.b == 0.0f || other.a == 0.0f) throw zero_division() ;
		return {
			std::clamp(r / static_cast<float>(other.r), 0.0f, 1.0f), 
			std::clamp(g / static_cast<float>(other.g), 0.0f, 1.0f), 
			std::clamp(b / static_cast<float>(other.b), 0.0f, 1.0f), 
			std::clamp(a / static_cast<float>(other.a), 0.0f, 1.0f)
		} ;
}

template <typename T> Color<float>& Color<float>::operator+=(const Color<T>& other) {
	*this = *this + other ;
	return *this ;
}

template <typename T> Color<float>& Color<float>::operator-=(const Color<T>& other) {
	*this = *this - other ;
	return *this ;
}

template <typename T> Color<float>& Color<float>::operator*=(const Color<T>& other) {
	*this = *this * other ;
	return *this ;
}

template <typename T> Color<float>& Color<float>::operator/=(const Color<T>& other) {
	*this = *this / other ;
	return *this ;
}

template <typename T> Color<float>& Color<float>::operator=(const T& val) {
	*this = Color<float>(static_cast<float>(val)) ;
	return *this ;
}

template <typename T> Color<float> Color<float>::operator+(const T& val) {
	return *this + Color<float>(static_cast<float>(val)) ;
}

template <typename T> Color<float> Color<float>::operator-(const T& val) {
	return *this - Color<float>(static_cast<float>(val)) ;
}

template <typename T> Color<float> Color<float>::operator*(const T& val) {
	return *this * Color<float>(static_cast<float>(val)) ;
}

template <typename T> Color<float> Color<float>::operator/(const T& val) {
	return *this / Color<float>(static_cast<float>(val)) ;
}

template <typename T> Color<float>& Color<float>::operator+=(const T& val) {
	*this += Color<float>(static_cast<float>(val)) ;
	return *this ;
}

template <typename T> Color<float>& Color<float>::operator-=(const T& val) {
	*this -= Color<float>(static_cast<float>(val)) ;
	return *this ;
}

template <typename T> Color<float>& Color<float>::operator*=(const T& val) {
	*this *= Color<float>(static_cast<float>(val)) ;
	return *this ;
}

template <typename T> Color<float>& Color<float>::operator/=(const T& val) {
	*this /= Color<float>(static_cast<float>(val)) ;
	return *this ;
}

bool Color<float>::operator==(const Color<float>& other) {
	return (
		r == other.r && 
		g == other.g && 
		b == other.b && 
		a == other.a
	) ;
}

bool Color<float>::operator!=(const Color<float>& other) {
	return !(*this == other) ;
}

Color<float>::operator Color<unsigned char>() const noexcept {
	return {
		static_cast<unsigned char>(std::clamp(r * 255.0f, 0.0f, 255.0f)), 
		static_cast<unsigned char>(std::clamp(g * 255.0f, 0.0f, 255.0f)), 
		static_cast<unsigned char>(std::clamp(b * 255.0f, 0.0f, 255.0f)), 
		static_cast<unsigned char>(std::clamp(a * 255.0f, 0.0f, 255.0f))
	} ;
}

template <typename T> struct is_defined_Vert_variants {
	static constexpr bool value = false ;
} ;

template <> struct is_defined_Vert_variants<Color<unsigned char>> {
	static constexpr bool value = true ;
} ;

template <> struct is_defined_Vert_variants<Color<float>> {
	static constexpr bool value = true ;
} ;

template <typename T> constexpr bool is_defined_Vert_variants_v = is_defined_Vert_variants<T>::value ;

template <typename T> struct Vert {
	static_assert(is_defined_Vert_variants_v<T>, "undefined Vert variants!");
} ;

template <typename T> struct is_defined_unit {
	static constexpr bool value = std::is_arithmetic_v<T> || std::is_arithmetic_v<T> || is_defined_Color_variants_v<T> || is_defined_Vert_variants_v<T> ;
} ;

template <typename T> constexpr bool is_defined_unit_v = is_defined_unit<T>::value ;