// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2020 The UFO Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef UFO_OUTPUTTYPE_H
#define UFO_OUTPUTTYPE_H

#include <attributes.h>
#include <script/signingprovider.h>
#include <script/standard.h>

#include <array>
#include <string>
#include <vector>

enum class OutputType {
    LEGACY,
    P2SH_SEGWIT,
    BECH32,
};

static constexpr auto OUTPUT_TYPES = std::array{
    OutputType::LEGACY,
    OutputType::P2SH_SEGWIT,
    OutputType::BECH32,
};

[[nodiscard]] bool ParseOutputType(const std::string& str, OutputType& output_type);
const std::string& FormatOutputType(OutputType type);

/**
 * Get a destination of the requested type (if possible) to the specified key.
 * The caller must make sure LearnRelatedScripts has been called beforehand.
 */
CTxDestination GetDestinationForKey(const CPubKey& key, OutputType);

/** Get all destinations (potentially) supported by the wallet for the given key. */
std::vector<CTxDestination> GetAllDestinationsForKey(const CPubKey& key);

/**
 * Get a destination of the requested type (if possible) to the specified script.
 * This function will automatically add the script (and any other
 * necessary scripts) to the keystore.
 */
CTxDestination AddAndGetDestinationForScript(FillableSigningProvider& keystore, const CScript& script, OutputType);

#endif // UFO_OUTPUTTYPE_H
