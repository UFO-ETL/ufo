// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The UFO Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef UFO_SCRIPT_UFOCONSENSUS_H
#define UFO_SCRIPT_UFOCONSENSUS_H

#include <stdint.h>

#if defined(BUILD_UFO_INTERNAL) && defined(HAVE_CONFIG_H)
#include <config/UFO-config.h>
  #if defined(_WIN32)
    #if defined(HAVE_DLLEXPORT_ATTRIBUTE)
      #define EXPORT_SYMBOL __declspec(dllexport)
    #else
      #define EXPORT_SYMBOL
    #endif
  #elif defined(HAVE_DEFAULT_VISIBILITY_ATTRIBUTE)
    #define EXPORT_SYMBOL __attribute__ ((visibility ("default")))
  #endif
#elif defined(MSC_VER) && !defined(STATIC_LIBUFOCONSENSUS)
  #define EXPORT_SYMBOL __declspec(dllimport)
#endif

#ifndef EXPORT_SYMBOL
  #define EXPORT_SYMBOL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define UFOCONSENSUS_API_VER 1

typedef enum UFOconsensus_error_t
{
    UFOconsensus_ERR_OK = 0,
    UFOconsensus_ERR_TX_INDEX,
    UFOconsensus_ERR_TX_SIZE_MISMATCH,
    UFOconsensus_ERR_TX_DESERIALIZE,
    UFOconsensus_ERR_AMOUNT_REQUIRED,
    UFOconsensus_ERR_INVALID_FLAGS,
} UFOconsensus_error;

/** Script verification flags */
enum
{
    UFOconsensus_SCRIPT_FLAGS_VERIFY_NONE                = 0,
    UFOconsensus_SCRIPT_FLAGS_VERIFY_P2SH                = (1U << 0), // evaluate P2SH (BIP16) subscripts
    UFOconsensus_SCRIPT_FLAGS_VERIFY_DERSIG              = (1U << 2), // enforce strict DER (BIP66) compliance
    UFOconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY           = (1U << 4), // enforce NULLDUMMY (BIP147)
    UFOconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY = (1U << 9), // enable CHECKLOCKTIMEVERIFY (BIP65)
    UFOconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY = (1U << 10), // enable CHECKSEQUENCEVERIFY (BIP112)
    UFOconsensus_SCRIPT_FLAGS_VERIFY_WITNESS             = (1U << 11), // enable WITNESS (BIP141)
    UFOconsensus_SCRIPT_FLAGS_VERIFY_ALL                 = UFOconsensus_SCRIPT_FLAGS_VERIFY_P2SH | UFOconsensus_SCRIPT_FLAGS_VERIFY_DERSIG |
                                                               UFOconsensus_SCRIPT_FLAGS_VERIFY_NULLDUMMY | UFOconsensus_SCRIPT_FLAGS_VERIFY_CHECKLOCKTIMEVERIFY |
                                                               UFOconsensus_SCRIPT_FLAGS_VERIFY_CHECKSEQUENCEVERIFY | UFOconsensus_SCRIPT_FLAGS_VERIFY_WITNESS
};

/// Returns 1 if the input nIn of the serialized transaction pointed to by
/// txTo correctly spends the scriptPubKey pointed to by scriptPubKey under
/// the additional constraints specified by flags.
/// If not nullptr, err will contain an error/success code for the operation
EXPORT_SYMBOL int UFOconsensus_verify_script(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen,
                                                 const unsigned char *txTo        , unsigned int txToLen,
                                                 unsigned int nIn, unsigned int flags, UFOconsensus_error* err);

EXPORT_SYMBOL int UFOconsensus_verify_script_with_amount(const unsigned char *scriptPubKey, unsigned int scriptPubKeyLen, int64_t amount,
                                    const unsigned char *txTo        , unsigned int txToLen,
                                    unsigned int nIn, unsigned int flags, UFOconsensus_error* err);

EXPORT_SYMBOL unsigned int UFOconsensus_version();

#ifdef __cplusplus
} // extern "C"
#endif

#undef EXPORT_SYMBOL

#endif // UFO_SCRIPT_UFOCONSENSUS_H
