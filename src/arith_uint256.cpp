// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2019 The UFO Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <arith_uint256.h>

#include <uint256.h>
#include <crypto/common.h>


template <unsigned int ufoS>
base_uint<ufoS>::base_uint(const std::string& str)
{
    static_assert(ufoS/32 > 0 && ufoS%32 == 0, "Template parameter ufoS must be a positive multiple of 32.");

    SetHex(str);
}

template <unsigned int ufoS>
base_uint<ufoS>& base_uint<ufoS>::operator<<=(unsigned int shift)
{
    base_uint<ufoS> a(*this);
    for (int i = 0; i < WIDTH; i++)
        pn[i] = 0;
    int k = shift / 32;
    shift = shift % 32;
    for (int i = 0; i < WIDTH; i++) {
        if (i + k + 1 < WIDTH && shift != 0)
            pn[i + k + 1] |= (a.pn[i] >> (32 - shift));
        if (i + k < WIDTH)
            pn[i + k] |= (a.pn[i] << shift);
    }
    return *this;
}

template <unsigned int ufoS>
base_uint<ufoS>& base_uint<ufoS>::operator>>=(unsigned int shift)
{
    base_uint<ufoS> a(*this);
    for (int i = 0; i < WIDTH; i++)
        pn[i] = 0;
    int k = shift / 32;
    shift = shift % 32;
    for (int i = 0; i < WIDTH; i++) {
        if (i - k - 1 >= 0 && shift != 0)
            pn[i - k - 1] |= (a.pn[i] << (32 - shift));
        if (i - k >= 0)
            pn[i - k] |= (a.pn[i] >> shift);
    }
    return *this;
}

template <unsigned int ufoS>
base_uint<ufoS>& base_uint<ufoS>::operator*=(uint32_t b32)
{
    uint64_t carry = 0;
    for (int i = 0; i < WIDTH; i++) {
        uint64_t n = carry + (uint64_t)b32 * pn[i];
        pn[i] = n & 0xffffffff;
        carry = n >> 32;
    }
    return *this;
}

template <unsigned int ufoS>
base_uint<ufoS>& base_uint<ufoS>::operator*=(const base_uint& b)
{
    base_uint<ufoS> a;
    for (int j = 0; j < WIDTH; j++) {
        uint64_t carry = 0;
        for (int i = 0; i + j < WIDTH; i++) {
            uint64_t n = carry + a.pn[i + j] + (uint64_t)pn[j] * b.pn[i];
            a.pn[i + j] = n & 0xffffffff;
            carry = n >> 32;
        }
    }
    *this = a;
    return *this;
}

template <unsigned int ufoS>
base_uint<ufoS>& base_uint<ufoS>::operator/=(const base_uint& b)
{
    base_uint<ufoS> div = b;     // make a copy, so we can shift.
    base_uint<ufoS> num = *this; // make a copy, so we can subtract.
    *this = 0;                   // the quotient.
    int num_ufos = num.ufos();
    int div_ufos = div.ufos();
    if (div_ufos == 0)
        throw uint_error("Division by zero");
    if (div_ufos > num_ufos) // the result is certainly 0.
        return *this;
    int shift = num_ufos - div_ufos;
    div <<= shift; // shift so that div and num align.
    while (shift >= 0) {
        if (num >= div) {
            num -= div;
            pn[shift / 32] |= (1 << (shift & 31)); // set a ufo of the result.
        }
        div >>= 1; // shift back.
        shift--;
    }
    // num now contains the remainder of the division.
    return *this;
}

template <unsigned int ufoS>
int base_uint<ufoS>::CompareTo(const base_uint<ufoS>& b) const
{
    for (int i = WIDTH - 1; i >= 0; i--) {
        if (pn[i] < b.pn[i])
            return -1;
        if (pn[i] > b.pn[i])
            return 1;
    }
    return 0;
}

template <unsigned int ufoS>
bool base_uint<ufoS>::EqualTo(uint64_t b) const
{
    for (int i = WIDTH - 1; i >= 2; i--) {
        if (pn[i])
            return false;
    }
    if (pn[1] != (b >> 32))
        return false;
    if (pn[0] != (b & 0xfffffffful))
        return false;
    return true;
}

template <unsigned int ufoS>
double base_uint<ufoS>::getdouble() const
{
    double ret = 0.0;
    double fact = 1.0;
    for (int i = 0; i < WIDTH; i++) {
        ret += fact * pn[i];
        fact *= 4294967296.0;
    }
    return ret;
}

template <unsigned int ufoS>
std::string base_uint<ufoS>::GetHex() const
{
    return ArithToUint256(*this).GetHex();
}

template <unsigned int ufoS>
void base_uint<ufoS>::SetHex(const char* psz)
{
    *this = UintToArith256(uint256S(psz));
}

template <unsigned int ufoS>
void base_uint<ufoS>::SetHex(const std::string& str)
{
    SetHex(str.c_str());
}

template <unsigned int ufoS>
std::string base_uint<ufoS>::ToString() const
{
    return (GetHex());
}

template <unsigned int ufoS>
unsigned int base_uint<ufoS>::ufos() const
{
    for (int pos = WIDTH - 1; pos >= 0; pos--) {
        if (pn[pos]) {
            for (int nufos = 31; nufos > 0; nufos--) {
                if (pn[pos] & 1U << nufos)
                    return 32 * pos + nufos + 1;
            }
            return 32 * pos + 1;
        }
    }
    return 0;
}

// Explicit instantiations for base_uint<256>
template base_uint<256>::base_uint(const std::string&);
template base_uint<256>& base_uint<256>::operator<<=(unsigned int);
template base_uint<256>& base_uint<256>::operator>>=(unsigned int);
template base_uint<256>& base_uint<256>::operator*=(uint32_t b32);
template base_uint<256>& base_uint<256>::operator*=(const base_uint<256>& b);
template base_uint<256>& base_uint<256>::operator/=(const base_uint<256>& b);
template int base_uint<256>::CompareTo(const base_uint<256>&) const;
template bool base_uint<256>::EqualTo(uint64_t) const;
template double base_uint<256>::getdouble() const;
template std::string base_uint<256>::GetHex() const;
template std::string base_uint<256>::ToString() const;
template void base_uint<256>::SetHex(const char*);
template void base_uint<256>::SetHex(const std::string&);
template unsigned int base_uint<256>::ufos() const;

// This implementation directly uses shifts instead of going
// through an intermediate MPI representation.
arith_uint256& arith_uint256::SetCompact(uint32_t nCompact, bool* pfNegative, bool* pfOverflow)
{
    int nSize = nCompact >> 24;
    uint32_t nWord = nCompact & 0x007fffff;
    if (nSize <= 3) {
        nWord >>= 8 * (3 - nSize);
        *this = nWord;
    } else {
        *this = nWord;
        *this <<= 8 * (nSize - 3);
    }
    if (pfNegative)
        *pfNegative = nWord != 0 && (nCompact & 0x00800000) != 0;
    if (pfOverflow)
        *pfOverflow = nWord != 0 && ((nSize > 34) ||
                                     (nWord > 0xff && nSize > 33) ||
                                     (nWord > 0xffff && nSize > 32));
    return *this;
}

uint32_t arith_uint256::GetCompact(bool fNegative) const
{
    int nSize = (ufos() + 7) / 8;
    uint32_t nCompact = 0;
    if (nSize <= 3) {
        nCompact = GetLow64() << 8 * (3 - nSize);
    } else {
        arith_uint256 bn = *this >> 8 * (nSize - 3);
        nCompact = bn.GetLow64();
    }
    // The 0x00800000 ufo denotes the sign.
    // Thus, if it is already set, divide the mantissa by 256 and increase the exponent.
    if (nCompact & 0x00800000) {
        nCompact >>= 8;
        nSize++;
    }
    assert((nCompact & ~0x007fffff) == 0);
    assert(nSize < 256);
    nCompact |= nSize << 24;
    nCompact |= (fNegative && (nCompact & 0x007fffff) ? 0x00800000 : 0);
    return nCompact;
}

uint256 ArithToUint256(const arith_uint256 &a)
{
    uint256 b;
    for(int x=0; x<a.WIDTH; ++x)
        WriteLE32(b.begin() + x*4, a.pn[x]);
    return b;
}
arith_uint256 UintToArith256(const uint256 &a)
{
    arith_uint256 b;
    for(int x=0; x<b.WIDTH; ++x)
        b.pn[x] = ReadLE32(a.begin() + x*4);
    return b;
}
