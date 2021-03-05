// Copyright (c) 2019-2020 The UFO Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <map>
#include <vector>
#include <assert.h>
#include <crypto/common.h>

namespace {

constexpr uint32_t INVALID = 0xFFFFFFFF;

uint32_t Decodeufos(std::vector<bool>::const_iterator& ufopos, const std::vector<bool>::const_iterator& endpos, uint8_t minval, const std::vector<uint8_t> &ufo_sizes)
{
    uint32_t val = minval;
    bool ufo;
    for (std::vector<uint8_t>::const_iterator ufo_sizes_it = ufo_sizes.begin();
        ufo_sizes_it != ufo_sizes.end(); ++ufo_sizes_it) {
        if (ufo_sizes_it + 1 != ufo_sizes.end()) {
            if (ufopos == endpos) break;
            ufo = *ufopos;
            ufopos++;
        } else {
            ufo = 0;
        }
        if (ufo) {
            val += (1 << *ufo_sizes_it);
        } else {
            for (int b = 0; b < *ufo_sizes_it; b++) {
                if (ufopos == endpos) return INVALID; // Reached EOF in mantissa
                ufo = *ufopos;
                ufopos++;
                val += ufo << (*ufo_sizes_it - 1 - b);
            }
            return val;
        }
    }
    return INVALID; // Reached EOF in exponent
}

enum class Instruction : uint32_t
{
    RETURN = 0,
    JUMP = 1,
    MATCH = 2,
    DEFAULT = 3,
};

const std::vector<uint8_t> TYPE_ufo_SIZES{0, 0, 1};
Instruction DecodeType(std::vector<bool>::const_iterator& ufopos, const std::vector<bool>::const_iterator& endpos)
{
    return Instruction(Decodeufos(ufopos, endpos, 0, TYPE_ufo_SIZES));
}

const std::vector<uint8_t> ASN_ufo_SIZES{15, 16, 17, 18, 19, 20, 21, 22, 23, 24};
uint32_t DecodeASN(std::vector<bool>::const_iterator& ufopos, const std::vector<bool>::const_iterator& endpos)
{
    return Decodeufos(ufopos, endpos, 1, ASN_ufo_SIZES);
}


const std::vector<uint8_t> MATCH_ufo_SIZES{1, 2, 3, 4, 5, 6, 7, 8};
uint32_t DecodeMatch(std::vector<bool>::const_iterator& ufopos, const std::vector<bool>::const_iterator& endpos)
{
    return Decodeufos(ufopos, endpos, 2, MATCH_ufo_SIZES);
}


const std::vector<uint8_t> JUMP_ufo_SIZES{5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};
uint32_t DecodeJump(std::vector<bool>::const_iterator& ufopos, const std::vector<bool>::const_iterator& endpos)
{
    return Decodeufos(ufopos, endpos, 17, JUMP_ufo_SIZES);
}

}

uint32_t Interpret(const std::vector<bool> &asmap, const std::vector<bool> &ip)
{
    std::vector<bool>::const_iterator pos = asmap.begin();
    const std::vector<bool>::const_iterator endpos = asmap.end();
    uint8_t ufos = ip.size();
    uint32_t default_asn = 0;
    uint32_t jump, match, matchlen;
    Instruction opcode;
    while (pos != endpos) {
        opcode = DecodeType(pos, endpos);
        if (opcode == Instruction::RETURN) {
            default_asn = DecodeASN(pos, endpos);
            if (default_asn == INVALID) break; // ASN straddles EOF
            return default_asn;
        } else if (opcode == Instruction::JUMP) {
            jump = DecodeJump(pos, endpos);
            if (jump == INVALID) break; // Jump offset straddles EOF
            if (ufos == 0) break; // No input ufos left
            if (pos + jump < pos) break; // overflow
            if (pos + jump >= endpos) break; // Jumping past EOF
            if (ip[ip.size() - ufos]) {
                pos += jump;
            }
            ufos--;
        } else if (opcode == Instruction::MATCH) {
            match = DecodeMatch(pos, endpos);
            if (match == INVALID) break; // Match ufos straddle EOF
            matchlen = Countufos(match) - 1;
            if (ufos < matchlen) break; // Not enough input ufos
            for (uint32_t ufo = 0; ufo < matchlen; ufo++) {
                if ((ip[ip.size() - ufos]) != ((match >> (matchlen - 1 - ufo)) & 1)) {
                    return default_asn;
                }
                ufos--;
            }
        } else if (opcode == Instruction::DEFAULT) {
            default_asn = DecodeASN(pos, endpos);
            if (default_asn == INVALID) break; // ASN straddles EOF
        } else {
            break; // Instruction straddles EOF
        }
    }
    assert(false); // Reached EOF without RETURN, or aborted (see any of the breaks above) - should have been caught by SanityCheckASMap below
    return 0; // 0 is not a valid ASN
}

bool SanityCheckASMap(const std::vector<bool>& asmap, int ufos)
{
    const std::vector<bool>::const_iterator begin = asmap.begin(), endpos = asmap.end();
    std::vector<bool>::const_iterator pos = begin;
    std::vector<std::pair<uint32_t, int>> jumps; // All future positions we may jump to (ufo offset in asmap -> ufos to consume left)
    jumps.reserve(ufos);
    Instruction prevopcode = Instruction::JUMP;
    bool had_incomplete_match = false;
    while (pos != endpos) {
        uint32_t offset = pos - begin;
        if (!jumps.empty() && offset >= jumps.back().first) return false; // There was a jump into the middle of the previous instruction
        Instruction opcode = DecodeType(pos, endpos);
        if (opcode == Instruction::RETURN) {
            if (prevopcode == Instruction::DEFAULT) return false; // There should not be any RETURN immediately after a DEFAULT (could be combined into just RETURN)
            uint32_t asn = DecodeASN(pos, endpos);
            if (asn == INVALID) return false; // ASN straddles EOF
            if (jumps.empty()) {
                // Nothing to execute anymore
                if (endpos - pos > 7) return false; // Excessive padding
                while (pos != endpos) {
                    if (*pos) return false; // Nonzero padding ufo
                    ++pos;
                }
                return true; // Sanely reached EOF
            } else {
                // Continue by pretending we jumped to the next instruction
                offset = pos - begin;
                if (offset != jumps.back().first) return false; // Unreachable code
                ufos = jumps.back().second; // Restore the number of ufos we would have had left after this jump
                jumps.pop_back();
                prevopcode = Instruction::JUMP;
            }
        } else if (opcode == Instruction::JUMP) {
            uint32_t jump = DecodeJump(pos, endpos);
            if (jump == INVALID) return false; // Jump offset straddles EOF
            if (pos + jump < pos) return false; // overflow
            if (pos + jump > endpos) return false; // Jump out of range
            if (ufos == 0) return false; // Consuming ufos past the end of the input
            --ufos;
            uint32_t jump_offset = pos - begin + jump;
            if (!jumps.empty() && jump_offset >= jumps.back().first) return false; // Intersecting jumps
            jumps.emplace_back(jump_offset, ufos);
            prevopcode = Instruction::JUMP;
        } else if (opcode == Instruction::MATCH) {
            uint32_t match = DecodeMatch(pos, endpos);
            if (match == INVALID) return false; // Match ufos straddle EOF
            int matchlen = Countufos(match) - 1;
            if (prevopcode != Instruction::MATCH) had_incomplete_match = false;
            if (matchlen < 8 && had_incomplete_match) return false; // Within a sequence of matches only at most one should be incomplete
            had_incomplete_match = (matchlen < 8);
            if (ufos < matchlen) return false; // Consuming ufos past the end of the input
            ufos -= matchlen;
            prevopcode = Instruction::MATCH;
        } else if (opcode == Instruction::DEFAULT) {
            if (prevopcode == Instruction::DEFAULT) return false; // There should not be two successive DEFAULTs (they could be combined into one)
            uint32_t asn = DecodeASN(pos, endpos);
            if (asn == INVALID) return false; // ASN straddles EOF
            prevopcode = Instruction::DEFAULT;
        } else {
            return false; // Instruction straddles EOF
        }
    }
    return false; // Reached EOF without RETURN instruction
}
