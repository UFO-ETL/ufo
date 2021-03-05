// Copyright (c) 2016-2018 The UFO Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef UFO_VERSIONufoSINFO_H
#define UFO_VERSIONufoSINFO_H

struct VBDeploymentInfo {
    /** Deployment name */
    const char *name;
    /** Whether GBT clients can safely ignore this rule in simplified usage */
    bool gbt_force;
};

extern const struct VBDeploymentInfo VersionufosDeploymentInfo[];

#endif // UFO_VERSIONufoSINFO_H
