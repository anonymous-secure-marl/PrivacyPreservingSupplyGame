#include <iostream>
#include <fstream>
#include <mathimf.h>
#include <math.h>

using namespace std;

#define RADIUS 6371000//6378137
#define NUM_ROWS 100000
// #define NUM_ROWS 100

double average_ulp_err_a = 0;
double max_ulp_err_a = 0;
double min_ulp_err_a = 10000000000;

double average_deviation = 0;
double max_deviation = 0;
double min_deviation = 10000000000;

float int_to_flt(uint32_t x) {
    float y = *((float*) &x);
    return y;
}

int64_t get_exponent(double x) {
    int64_t x_int = *((int64_t*) &x);
    return ((x_int >> 52) & 2047) - 1023;
}

double ULP_error (float actual, double expected) {
    double abs_err = abs(double(actual) - double(expected));
    int32_t expected_exp = get_exponent(expected);
    double ulp;
    if (expected_exp < -126) {
        ulp = exp2(-126.0 - 23.0);
    } else {
        ulp = exp2(expected_exp - 23.0);
    }
    return abs_err/ulp;
}

double geodesic_ideal(float latA, float lonA, float latB, float lonB) {
    float lat_sub = (latA - latB)/2.0;
    float lon_sub = (lonA - lonB)/2.0;
    float sin_lat_sub = sinpi(lat_sub);
    float hav_lat = sin_lat_sub * sin_lat_sub;
    float sin_lon_sub = sinpi(lon_sub);
    float hav_lon = sin_lon_sub * sin_lon_sub;
    float cos_latA = cospi(latA);
    float cos_latB = cospi(latB);
    float cos_latA_cos_latB = cos_latA * cos_latB;
    float hav_lon_cos_latA_cos_latB = cos_latA_cos_latB * hav_lon;
    float a = hav_lat + hav_lon_cos_latA_cos_latB;
    return a;
}

double geodesic_ideal_dbl(float latA, float lonA, float latB, float lonB) {
    double lat_sub = (double(latA) - double(latB))/2.0;
    double lon_sub = (double(lonA) - double(lonB))/2.0;
    double sin_lat_sub = sinpi(lat_sub);
    double hav_lat = sin_lat_sub * sin_lat_sub;
    double sin_lon_sub = sinpi(lon_sub);
    double hav_lon = sin_lon_sub * sin_lon_sub;
    double cos_latA = cospi(latA);
    double cos_latB = cospi(latB);
    double cos_latA_cos_latB = cos_latA * cos_latB;
    double hav_lon_cos_latA_cos_latB = cos_latA_cos_latB * hav_lon;
    double a = hav_lat + hav_lon_cos_latA_cos_latB;
    return a;
}

float compute_distance (float a) {
    return 2*RADIUS*atan(sqrt(a/(1.0-a)));
}

double compute_distance_dbl (double a) {
    return 2*RADIUS*atan(sqrt(a/(1.0-a)));
}

void test_accuracy(string file_name) {
    fstream fin(file_name.c_str(), fstream::in);
    for (int i = 0; i < NUM_ROWS; i++) {
    // for (int i = 0; i < 100; i++) {
        int seed, idx;
        uint32_t latA_int, latB_int, lonA_int, lonB_int, a_int;
        fin >> seed;
        fin >> idx;
        fin >> latA_int;
        fin >> latB_int;
        fin >> lonA_int;
        fin >> lonB_int;
        fin >> a_int;
        float latA = int_to_flt(latA_int), latB = int_to_flt(latB_int), lonA = int_to_flt(lonA_int), lonB = int_to_flt(lonB_int);
        // float a_expected = geodesic_ideal(latA, lonA, latB, lonB);
        float a_actual = int_to_flt(a_int);
        double a_expected = geodesic_ideal_dbl(latA, lonA, latB, lonB);
        // float a_actual = geodesic_ideal(latA, lonA, latB, lonB);
        // float expected_distance = compute_distance(a_expected);
        // float actual_distance = compute_distance(a_actual);
        double expected_distance = compute_distance_dbl(a_expected);
        double actual_distance = compute_distance_dbl(a_actual);
        double ulp_err_a = ULP_error(a_actual, a_expected);
        double deviation = abs(actual_distance - expected_distance);
        average_ulp_err_a += ulp_err_a;
        if (ulp_err_a > max_ulp_err_a) {
            cout.precision(20);
            // cout << i << "\t" << latA << "\t" << latB << "\t" << lonA << "\t" << lonB << "\t" << a_actual << "\t" << a_expected << "\t" << ulp_err_a << "\t" << expected_distance << "\t" << actual_distance << endl;
            cout << i << "\t" << latA << "\t" << latB << "\t" << lonA << "\t" << lonB << "\t" << a_actual << "\t" << a_expected << "\t" << ulp_err_a << endl;
        }
        max_ulp_err_a = max(ulp_err_a, max_ulp_err_a);
        min_ulp_err_a = min(ulp_err_a, min_ulp_err_a);
        average_deviation += deviation;
        max_deviation = max(deviation, max_deviation);
        min_deviation = min(deviation, min_deviation);
        /*
        // if (ulp_err_a > 4) {
        if (deviation > 1000) {
            cout.precision(15);
            cout << i << "\t" << latA << "\t" << latB << "\t" << lonA << "\t" << lonB << "\t" << a_actual << "\t" << a_expected << "\t" << ulp_err_a << "\t" << expected_distance << "\t" << actual_distance << endl;
        }
        // */
    }
    cout << "Average ULP Error: " << average_ulp_err_a/NUM_ROWS << endl;
    cout << "Max ULP Error: " << max_ulp_err_a << endl;
    cout << "Min ULP Error: " << min_ulp_err_a << endl;
    cout << "Average Deviation: " << average_deviation/NUM_ROWS << endl;
    cout << "Max Deviation: " << max_deviation << endl;
    cout << "Min Deviation: " << min_deviation << endl;

    fin.close();
}

int main () {
    cout << "========= Ours ===========" << endl;
    test_accuracy("geodesic-out.csv");

    average_ulp_err_a = 0;
    max_ulp_err_a = 0;
    min_ulp_err_a = 10000000000;
    average_deviation = 0;
    max_deviation = 0;
    min_deviation = 10000000000;
    cout << "========= ABY ===========" << endl;
    test_accuracy("geodesic-ABY.csv");

    average_ulp_err_a = 0;
    max_ulp_err_a = 0;
    min_ulp_err_a = 10000000000;
    average_deviation = 0;
    max_deviation = 0;
    min_deviation = 10000000000;
    cout << "========= Ours-new ===========" << endl;
    test_accuracy("geodesic-out-new.csv");

    average_ulp_err_a = 0;
    max_ulp_err_a = 0;
    min_ulp_err_a = 10000000000;
    average_deviation = 0;
    max_deviation = 0;
    min_deviation = 10000000000;
    cout << "========= ABY-new ===========" << endl;
    test_accuracy("geodesic-ABY-new.csv");

    average_ulp_err_a = 0;
    max_ulp_err_a = 0;
    min_ulp_err_a = 10000000000;
    average_deviation = 0;
    max_deviation = 0;
    min_deviation = 10000000000;
    cout << "========= Ours-new-new ===========" << endl;
    test_accuracy("geodesic-out-new-new.csv");

    average_ulp_err_a = 0;
    max_ulp_err_a = 0;
    min_ulp_err_a = 10000000000;
    average_deviation = 0;
    max_deviation = 0;
    min_deviation = 10000000000;
    cout << "========= ABY-new-new ===========" << endl;
    test_accuracy("geodesic-ABY-new-new.csv");

    average_ulp_err_a = 0;
    max_ulp_err_a = 0;
    min_ulp_err_a = 10000000000;
    average_deviation = 0;
    max_deviation = 0;
    min_deviation = 10000000000;
    cout << "========= Ours and ABY ===========" << endl;
    test_accuracy("geodesic-out-no-trig.csv");
}
