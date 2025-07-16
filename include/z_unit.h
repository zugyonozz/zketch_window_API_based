#pragma once
#include <type_traits>
#include <windows.h>

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
	Vec2() : x(0), y(0) {}
	Vec2(int n) : x(n), y(n) {}
	Vec2(int x, int y) : x(x), y(y) {}
} ;

template <> struct Vec2<float> {
	float x, y ;
	Vec2() : x(0.0f), y(0.0f) {}
	Vec2(float n) : x(n), y(n) {}
	Vec2(float x, float y) : x(x), y(y) {}
} ;

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
	Rect() : x(0), y(0), w(0), h(0) {}
	Rect(int n) : x(n), y(n), w(n), h(n) {}
	Rect(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {}
} ;

template <> struct Rect<float> {
	float x, y, w, h ;
	Rect() : x(0.0f), y(0.0f), w(0.0f), h(0.0f) {}
	Rect(float n) : x(n), y(n), w(n), h(n) {}
	Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}
} ;

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
	static_assert(is_defined_Color_variants_v<T>, "undefined Color variants!");
} ;

template <> struct Color<unsigned char> {
	unsigned char r, g, b, a ;
	Color() : r(0), g(0), b(0), a(0) {}
	Color(unsigned char n) : r(n), g(n), b(n), a(n) {}
	Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a) : r(r), g(g), b(b), a(a) {}
} ;

template <> struct Color<float> {
	float r, g, b, a ;
	Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
	Color(float n) : r(n < 0.0f ? 0.0f : (n > 1.0f ? 1.0f : n)), g(n < 0.0f ? 0.0f : (n > 1.0f ? 1.0f : n)), b(n < 0.0f ? 0.0f : (n > 1.0f ? 1.0f : n)), a(n < 0.0f ? 0.0f : (n > 1.0f ? 1.0f : n)) {}
	Color(float r, float g, float b, float a) : r(r < 0.0f ? 0.0f : (r > 1.0f ? 1.0f : r)), g(g < 0.0f ? 0.0f : (g > 1.0f ? 1.0f : g)), b(b < 0.0f ? 0.0f : (b > 1.0f ? 1.0f : b)), a(a < 0.0f ? 0.0f : (a > 1.0f ? 1.0f : a)) {}
} ;

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
	static constexpr bool value = is_defined_Vec2_variants_v<T> || is_defined_Rect_variants_v<T> || is_defined_Color_variants_v<T> || is_defined_Vert_variants_v<T> ;
} ;

template <typename T> constexpr bool is_defined_unit_v = is_defined_unit<T>::value ;

template <typename T1, typename T2 = T1> struct unit_op {
	static_assert(is_defined_unit_v<T1> && is_defined_unit_v<T2>, "undefined unit to operate!") ;
} ;

template <typename T1, typename T2> struct unit_op<Vec2<T1>, Vec2<T2>> {
	template <typename Op> auto operator()(Vec2<T1> a, Vec2<T2> b, Op op) {
		if constexpr(std::is_same_v<decltype(op(a.x, b.x)), bool>)
			return op(a.x, b.x) && op(a.y, b.y) ;
		if constexpr(std::is_floating_point_v<decltype(op(a.x, b.x))>)
			return Vec2<float>{static_cast<float>(op(a.x, b.x)), static_cast<float>(op(a.y, b.y))} ;
		return Vec2<int>{static_cast<int>(op(a.x, b.x)), static_cast<int>(op(a.y, b.y))} ;
	}
} ;

template <typename T1, typename T2> struct unit_op<Rect<T1>, Rect<T2>> {
	template <typename Op> auto operator()(Rect<T1> a, Rect<T2> b, Op op) {
		if constexpr(std::is_same_v<decltype(op(a.x, b.x)), bool>)
			return op(a.x, b.x) && op(a.y, b.y) && op(a.w, b.w) && op(a.h, b.h) ;
		if constexpr(std::is_floating_point_v<decltype(op(a.x, b.x))>)
			return Rect<float>{static_cast<float>(op(a.x, b.x)), static_cast<float>(op(a.y, b.y)), static_cast<float>(op(a.w, b.w)), static_cast<float>(op(a.h, b.h))} ;
		return Rect<int>{static_cast<int>(op(a.x, b.x)), static_cast<int>(op(a.y, b.y)), static_cast<int>(op(a.w, b.w)), static_cast<int>(op(a.h, b.h))} ;
	}
} ;

template <typename T1, typename T2> struct unit_op<Color<T1>, Color<T2>> {
	template <typename Op> auto operator()(Color<T1> a, Color<T2> b, Op op) {
		if constexpr(std::is_same_v<decltype(op(a.r, b.r)), bool>)
			return op(a.r, b.r) && op(a.g, b.g) && op(a.b, b.b) && op(a.a, b.a) ;
		if constexpr(std::is_floating_point_v<decltype(op(a.r, b.r))>)
			return Color<float>{static_cast<float>(op(a.r, b.r)), static_cast<float>(op(a.g, b.g)), static_cast<float>(op(a.b, b.b)), static_cast<float>(op(a.a, b.a))} ;
		return Color<unsigned char>{static_cast<unsigned char>(op(a.r, b.r)), static_cast<unsigned char>(op(a.g, b.g)), static_cast<unsigned char>(op(a.b, b.b)), static_cast<unsigned char>(op(a.a, b.a))} ;
	}
} ;

struct zero_division {
	const char* what() const noexcept { return "Division by zero in unit_op" ; }
} ;

template <typename T1, typename T2> auto operator+(const T1& a, const T2& b) noexcept {
	return unit_op<T1, T2>{}(a, b, [](auto a, auto b){ return a + b ; }) ;
}

template <typename T1, typename T2> auto operator-(const T1& a, const T2& b) noexcept {
	return unit_op<T1, T2>{}(a, b, [](auto a, auto b){ return a - b ; }) ;
}

template <typename T1, typename T2> auto operator*(const T1& a, const T2& b) noexcept {
	return unit_op<T1, T2>{}(a, b, [](auto a, auto b){ return a * b ; }) ;
}

template <typename T1, typename T2> auto operator/(const T1& a, const T2& b) {
	return unit_op<T1, T2>{}( a, b, [] (auto a, auto b) { 
		if(b == 0) throw zero_division() ;
		return a / b ; 
	}) ;
}

template <typename T1, typename T2> bool operator==(const T1& a, const T2& b) noexcept {
	return unit_op<T1, T2>{}(a, b, [](auto a, auto b){ return a == b ; }) ;
}

template <typename T1, typename T2> bool operator!=(const T1& a, const T2& b) noexcept {
	return !(a == b) ;
}