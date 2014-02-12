#ifndef FIXED_PT_H
#define FIXED_PT_H

typedef    int32_t    fixedpt;
typedef    int64_t    fixedptd;
typedef    uint32_t   fixedptu;
typedef    uint64_t   fixedptud;

template<int FRAC_BITS>
inline fixedpt int2fp(fixedpt I) {
    return (fixedptd)(I) << FRAC_BITS;
}

template<int FRAC_BITS>
inline fixedpt fp2int(fixedpt F) {
    return (F) >> FRAC_BITS;
}

/* Multiplies two fixedpt numbers, returns the result. */
template<int FRAC_BITS>
inline fixedpt fp_mul(fixedpt A, fixedpt B) {
    return (((fixedptd)A * (fixedptd)B) >> FRAC_BITS);
}

/* Divides two fixedpt numbers, returns the result. */
template<int FRAC_BITS>
inline fixedpt fp_div(fixedpt A, fixedpt B) {
    return (((fixedptd)A << FRAC_BITS) / (fixedptd)B);
}

template<int FRAC_BITS>
inline fixedpt float2fp(float flt) {
    return rint((1<<FRAC_BITS) * flt);
}

#endif