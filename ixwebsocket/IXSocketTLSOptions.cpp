/*
 *  IXSocketTLSOptions.h
 *  Author: Matt DeBoer
 *  Copyright (c) 2019 Machine Zone, Inc. All rights reserved.
 */

#include "IXSocketTLSOptions.h"

#include <assert.h>
#include <fstream>

namespace ix
{
    const char* kTLSCAFileUseSystemDefaults = "SYSTEM";
    const char* kTLSCAFileDisableVerify = "NONE";
    const char* kTLSCiphersUseDefault = "DEFAULT";

    bool SocketTLSOptions::isValid() const
    {
        if (!_validated)
        {
            if (!certFile.empty() && !std::ifstream(certFile))
            {
                _errMsg = "certFile not found: " + certFile;
                return false;
            }
            if (!keyFile.empty() && !std::ifstream(keyFile))
            {
                _errMsg = "keyFile not found: " + keyFile;
                return false;
            }
            if (!caFile.empty() && caFile != kTLSCAFileDisableVerify &&
                caFile != kTLSCAFileUseSystemDefaults && !std::ifstream(caFile))
            {
                _errMsg = "caFile not found: " + caFile;
                return false;
            }

            if (certFile.empty() != keyFile.empty())
            {
                _errMsg = "certFile and keyFile must be both present, or both absent";
                return false;
            }

            _validated = true;
        }
        return true;
    }

    bool SocketTLSOptions::hasCertAndKey() const
    {
        return !certFile.empty() && !keyFile.empty();
    }

    bool SocketTLSOptions::isUsingSystemDefaults() const
    {
        return caFile == kTLSCAFileUseSystemDefaults;
    }

    bool SocketTLSOptions::isPeerVerifyDisabled() const
    {
        return caFile == kTLSCAFileDisableVerify;
    }

    bool SocketTLSOptions::isUsingDefaultCiphers() const
    {
        return ciphers.empty() || ciphers == kTLSCiphersUseDefault;
    }

    const std::string& SocketTLSOptions::getErrorMsg() const
    {
        return _errMsg;
    }
} // namespace ix
