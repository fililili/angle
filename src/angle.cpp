#include <cstdint>
#include <type_traits>
#include <concepts>
#include <compare>
#include <cassert>
#include <numeric>
#include <cmath>
#include <numbers>
#include <iostream>

class angle {
public:
    constexpr angle() = default;

    friend constexpr angle operator+ (angle a1, angle a2) {
        return a1._value + a2._value;
    }

    friend constexpr angle operator- (angle a) {
        return -a._value;
    }

    friend constexpr angle operator- (angle a1, angle a2) {
        return a1 + (-a2);
    }

    friend constexpr angle operator* (angle a, std::integral auto val) {
        return a._value * val;
    }

    friend constexpr angle operator* (angle a, std::floating_point auto val) {
        long double max_value = std::numeric_limits<std::uint32_t>::max();
        max_value += 1;
        
        long double _val = a._value * val;
        _val = std::fmod(_val, max_value);
        _val = _val >= 0? _val : _val + max_value;
        return std::round(_val);
    }

    friend constexpr angle operator* (auto val, angle a) {
        return a * val;
    }

    friend constexpr auto operator<=> (angle a1, angle a2) {
        return a1._value <=> a2._value;
    }

    friend constexpr auto operator== (angle a1, angle a2) {
        return 0 == a1 <=> a2;
    }
    
    friend constexpr angle operator% (angle a1, angle a2) {
        // check
        auto times = std::numeric_limits<std::uint32_t>::max() / a2._value + 1;
        assert(a2 * times == 0);

        return a1._value % a2._value;
    }

    friend constexpr angle angle_by_degree(long double deg);
    friend constexpr angle angle_by_radian(long double rad);
    friend constexpr long double convert_to_degree(angle a);
    friend constexpr long double convert_to_radian(angle a);

private:
    constexpr angle(std::uint32_t value) : _value{value}{}
    std::uint32_t _value;
};

inline constexpr angle angle_by_degree(long double deg) {
    long double max_value = std::numeric_limits<std::uint32_t>::max();
    max_value += 1.0;

    deg = std::fmod(deg, 360.0);
    deg = deg >= 0? deg : deg + 360.0;
    return std::round((deg / 360.0) * max_value);
}

inline constexpr angle angle_by_radian(long double rad) {
    long double max_value = std::numeric_limits<std::uint32_t>::max();
    max_value += 1.0;

    rad = std::fmod(rad, 2 * std::numbers::pi);
    rad = rad >= 0? rad : rad + 2 * std::numbers::pi;
    return std::round((rad / (2 * std::numbers::pi)) * max_value);
}

constexpr angle operator"" _deg(long double deg) {
    return angle_by_degree(deg);
}

constexpr angle operator"" _rad(long double rad) {
    return angle_by_radian(rad);
}

constexpr long double convert_to_degree(angle a) {
    auto unit = 1.0_deg;
    long double unit_val = unit._value;
    return a._value / unit_val;
}

constexpr long double convert_to_radian(angle a) {
    auto unit = 1.0_rad;
    long double unit_val = unit._value;
    return a._value / unit_val;
}

constexpr bool is_near(angle a1, angle a2, angle tol) {
    return a1 - a2 < tol || a2 - a1 < tol;
}

static_assert(std::is_trivial<angle>::value);
static_assert(0.0_deg == 360.0_deg);

static_assert(45.0_deg * 2 == 90.0_deg);
static_assert(90.0_deg * 2 ==180.0_deg);
static_assert(180.0_deg * 2 == 0.0_deg);

static_assert(45.0_deg * 2.0 == 90.0_deg);
static_assert(90.0_deg * 2.0 == 180.0_deg);
static_assert(180.0_deg * 2.0 == 0.0_deg);

static_assert(45.0_deg * -6 == 90.0_deg);
static_assert(90.0_deg * -2 == 180.0_deg);
static_assert(180.0_deg * -2 == 0.0_deg);

static_assert(45.0_deg * -6.0 == 90.0_deg);
static_assert(90.0_deg * -2.0 == 180.0_deg);
static_assert(180.0_deg * -2.0 == 0.0_deg);

static_assert(is_near(1.0_deg * 360, 0.0_deg, 0.1_deg));
static_assert(is_near(1.0_deg * 360.0, 0.0_deg, 0.1_deg));

static_assert(0.0_deg == 0.0_rad);
static_assert(180.0_deg == 3.141592653_rad);

int main() {
    std::cout << "180.0_deg is: " <<  convert_to_degree(180.0_deg) << "_deg" << std::endl;
    std::cout << "180.0_deg is: " <<  convert_to_radian(180.0_deg) << "_rad" << std::endl;
}
