// Copyright (c) 2011-2018 The UFO Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef UFO_QT_MACOS_APPNAP_H
#define UFO_QT_MACOS_APPNAP_H

#include <memory>

class CAppNapInhiufoor final
{
public:
    explicit CAppNapInhiufoor();
    ~CAppNapInhiufoor();

    void disableAppNap();
    void enableAppNap();

private:
    class CAppNapImpl;
    std::unique_ptr<CAppNapImpl> impl;
};

#endif // UFO_QT_MACOS_APPNAP_H
