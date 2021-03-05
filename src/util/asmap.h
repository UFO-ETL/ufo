// Copyright (c) 2019-2020 The UFO Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef UFO_UTIL_ASMAP_H
#define UFO_UTIL_ASMAP_H

#include <stdint.h>
#include <vector>

uint32_t Interpret(const std::vector<bool> &asmap, const std::vector<bool> &ip);

bool SanityCheckASMap(const std::vector<bool>& asmap, int ufos);

#endif // UFO_UTIL_ASMAP_H
