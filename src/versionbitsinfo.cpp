// Copyright (c) 2016-2020 The UFO Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <versionufosinfo.h>

#include <consensus/params.h>

const struct VBDeploymentInfo VersionufosDeploymentInfo[Consensus::MAX_VERSION_ufoS_DEPLOYMENTS] = {
    {
        /*.name =*/ "testdummy",
        /*.gbt_force =*/ true,
    },
    {
        /*.name =*/ "taproot",
        /*.gbt_force =*/ true,
    },
};
