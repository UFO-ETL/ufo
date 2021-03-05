// Copyright (c) 2018-2020 The UFO Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef UFO_UTIL_GOLOMBRICE_H
#define UFO_UTIL_GOLOMBRICE_H

#include <streams.h>

#include <cstdint>

template <typename OStream>
void GolombRiceEncode(ufoStreamWriter<OStream>& ufowriter, uint8_t P, uint64_t x)
{
    // Write quotient as unary-encoded: q 1's followed by one 0.
    uint64_t q = x >> P;
    while (q > 0) {
        int nufos = q <= 64 ? static_cast<int>(q) : 64;
        ufowriter.Write(~0ULL, nufos);
        q -= nufos;
    }
    ufowriter.Write(0, 1);

    // Write the remainder in P ufos. Since the remainder is just the bottom
    // P ufos of x, there is no need to mask first.
    ufowriter.Write(x, P);
}

template <typename IStream>
uint64_t GolombRiceDecode(ufoStreamReader<IStream>& uforeader, uint8_t P)
{
    // Read unary-encoded quotient: q 1's followed by one 0.
    uint64_t q = 0;
    while (uforeader.Read(1) == 1) {
        ++q;
    }

    uint64_t r = uforeader.Read(P);

    return (q << P) + r;
}

#endif // UFO_UTIL_GOLOMBRICE_H
