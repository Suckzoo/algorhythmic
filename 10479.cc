#include <cstdio>
#include <cassert>
#include <iostream>
#include <cmath>
#include <string>
#include <algorithm>
#include <vector>
#include <utility>
using namespace std;

#ifdef DEBUG
#define debug(fmt, args...) fprintf(stderr, "[%s:%d:%s()]: " fmt "\n", __FILE__, __LINE__, __func__, ##args)
#else
#define debug(fmt, args...)
#endif

#define endl "\n"

typedef pair<int, int> pii;
typedef long long llint;

const double EPS = 1e-10;

inline const double min(const double &x, const double &y) {
    return x < y ? x : y;
}
inline const double fabs(const double &x, const double &y) {
    return x < 0 ? -x : x;
}

struct vec2 {
    double x, y;
    vec2(): x(0), y(0){}
    vec2(double x, double y): x(x), y(y){};
    bool operator == (const vec2 &v) const {
        return fabs(x - v.x) < EPS && fabs(y - v.y) < EPS;
    }
    vec2 operator + (const vec2 &v) const {
        return vec2(x+v.x, y+v.y);
    }
    vec2 operator - () const {
        return vec2(-x, -y);
    }
    vec2 operator - (const vec2 &v) const {
        return vec2(x-v.x, y-v.y);
    }
    vec2 operator * (const double d) const {
        return vec2(x*d, y*d);
    }
    vec2 operator / (const double d) const {
        assert(fabs(d) > EPS);
        return vec2(x/d, y/d);
    }
    double operator * (const vec2 &v) const {
        return x*v.x + y*v.y;
    }
    double normsq() const {
        return x*x + y*y;
    }
    double norm() const {
        return sqrt(x*x + y*y);
    }
    vec2 normalize() const {
        return vec2(x, y) / norm();
    }
    vec2 normal() const {
        return vec2(y, -x).normalize();
    }
    double dist_from(const vec2 &v) const {
        return (*this - v).norm();
    }
};

class Segment {
    vec2 s, e;
public:
    Segment(vec2 s, vec2 e): s(s), e(e) {}
    Segment operator + (const vec2 &v) const {
        return Segment(s+v, e+v);
    }
    Segment operator - (const vec2 &v) const {
        return Segment(s-v, e-v);
    }
    vec2 get_s() const {
        return s;
    }
    vec2 get_e() const {
        return e;
    }
    bool point_in_segment(const vec2 &pt) const {
        if (s == pt || e == pt) return true;
        vec2 s_to_pt = (s - pt).normalize();
        vec2 e_to_pt = (e - pt).normalize();
        return s_to_pt * e_to_pt <= -1 + EPS;
    }
    vec2 nearest_point_from_point(const vec2 center) const {
        vec2 normal = (e - s).normal();
        vec2 pt_to_seg = e - center;
        double dist = pt_to_seg * normal;
        vec2 foot = center + normal * dist;
        if (point_in_segment(foot)) return foot;
        if (s.dist_from(center) > e.dist_from(center)) return e;
        return s;
    }
};

void solve(double M, double B, double D, double T) {
    debug("solve: M=%.3lf B=%.3lf D=%.3lf T=%.3lf", M, B, D, T);
    // initially dog is at = (0, -5.5)
    // dog located at on time t = (0, -5.5) + t*(0,M)
    // nth(starts from 0) Bike located at on time t = (-D, 0) + n*(-4, 0) + t*(B, 0)
    auto dog_at_t = [&](double t) -> vec2 {
        vec2 dog;
        if (t < T) dog = vec2(0, -5.5);
        else dog = vec2(0, -5.5) + vec2(0, M) * (t - T);
        debug("dog at time %.2lf: (%.2lf, %.2lf)", t, dog.x, dog.y);
        return dog;
    };
    auto bike_at_t = [&](double t, int bike_nth) -> Segment {
        auto head = vec2(-D, 0) + vec2((double)(-4*bike_nth), 0) + vec2(B*t, 0);
        debug("head of bike %d at time %.2lf: (%.2lf, %.2lf)", bike_nth + 1, t, head.x, head.y);
        return Segment(head, head - vec2(2, 0));
    };
    auto doggo_distance_from_bike = [&](double t, int bike_nth) -> double {
        vec2 dog = dog_at_t(t);
        Segment bike = bike_at_t(t, bike_nth);
        vec2 nearest_point = bike.nearest_point_from_point(dog);
        double dist = dog.dist_from(nearest_point);
        debug("nearest point with bike %d at time %.2lf: (%.2lf, %.2lf) (dist = %.2lf)", bike_nth + 1, t, nearest_point.x, nearest_point.y, dist);
        return dist;
    };
    const int N = 10;
    const double r = 0.5;
    for (int i = 0; i < N; i++) {
        // if (i-1)th bike passed, the x of bike's head is at least 2.5, which means toured 2.5 - B_{i-1}(0).x meter
        // which means (2.5 - B_{i-1}(0).x)/B seconds elapsed 
        // B_{i-1}(0).x == -D-4*(i-1)
        // 2.5 - (-D-4*(i-1)) == D + 4*(i-1) + 2.5;
        double lower_t = 0, upper_t = T + 6.0 / M;
        if (i > 0) {
            lower_t = (D + 2.5 + (double)(4 * (i - 1))) / B;
        }
        if (lower_t < T) {
            lower_t = T;
        }
        debug("ternary search parameter for bike %d / lower_t: %.2lf, upper_t: %.2lf", i+1, lower_t, upper_t);
        
        while (upper_t - lower_t > EPS) {
            double t1, t2;
            t1 = lower_t + (upper_t - lower_t) / 3;
            t2 = lower_t + 2 * (upper_t - lower_t) / 3;
            double d1, d2;
            d1 = doggo_distance_from_bike(t1, i);
            d2 = doggo_distance_from_bike(t2, i);
            debug("upper_t: %.2lf, lower_t: %.2lf, t1: %.6lf, d1: %.2lf, t2: %.6lf, d2: %.2lf", upper_t, lower_t, t1, d1, t2, d2);
            if (min(d1, d2) < r) {
                cout << "Collision with bicycle " << i+1 << endl;
                return;
            }
            if (fabs(d1 - d2) < EPS) {
                lower_t = t1;
                upper_t = t2;
            } else if (d1 > d2) {
                lower_t = t1;
            } else if (d1 < d2) {
                upper_t = t2;
            }
        }
        // vec2 bike_tail = vec2(-D, 0) - vec2(2, 0) - vec2(4 * (i-1), 0);
        // -D-2-4(i-1) + Bt = 0
        double t_after_bike_pass_origin = (D + 2.0 + 4.0 * (double)(i)) / B;
        auto doggo_after_bike_pass = dog_at_t(t_after_bike_pass_origin);
        debug("after bike pass: t=%.6lf, doggo_at=(%.6lf, %.6lf)", t_after_bike_pass_origin, doggo_after_bike_pass.x, doggo_after_bike_pass.y);
        if (doggo_after_bike_pass.y > r) { 
            if (i == 0) {
                cout << "Max beats the first bicycle" << endl;
            } else {
                cout << "Max crosses safely after bicycle " << i << endl;
            }
            return;
        }
    }
    cout << "Max crosses safely after bicycle 10" << endl;
}
int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);
    int NT;
    cin >> NT;
    while (NT--) {
        double M, B, D, T;
        cin >> M >> B >> D >> T;
        debug();
        debug("REMAINING CASES: %d", NT);
        debug();
        solve(M, B, D, T);
    }
    return 0;
}
