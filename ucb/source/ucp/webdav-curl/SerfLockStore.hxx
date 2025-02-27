/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */


#pragma once

#include <map>
#include <mutex>
#include <rtl/ref.hxx>
#include <rtl/ustring.hxx>
#include <com/sun/star/ucb/Lock.hpp>

#include "CurlSession.hxx"

namespace http_dav_ucp
{

class TickerThread;

struct LockInfo
{
    OUString m_sToken;
    css::ucb::Lock m_Lock;
    rtl::Reference<CurlSession> m_xSession;
    sal_Int32 m_nLastChanceToSendRefreshRequest;

    LockInfo()
        : m_nLastChanceToSendRefreshRequest( -1 ) {}

    LockInfo( const OUString& sToken,
              css::ucb::Lock const& rLock,
              rtl::Reference<CurlSession> const & xSession,
              sal_Int32 nLastChanceToSendRefreshRequest )
        : m_sToken(sToken)
        , m_Lock(rLock)
        , m_xSession(xSession)
        , m_nLastChanceToSendRefreshRequest(nLastChanceToSendRefreshRequest)
    {}
};

typedef std::map< OUString, LockInfo > LockInfoMap;

class SerfLockStore
{
    std::mutex         m_aMutex;
    rtl::Reference< TickerThread > m_pTickerThread;
    LockInfoMap        m_aLockInfoMap;

public:
    SerfLockStore();
    ~SerfLockStore();

    OUString const* getLockTokenForURI(OUString const& rURI, css::ucb::Lock const* pLock);

    void addLock( const OUString& rURI,
                  css::ucb::Lock const& rLock,
                  const OUString& sToken,
                  rtl::Reference<CurlSession> const & xSession,
                  // time in seconds since Jan 1 1970
                  // -1: infinite lock, no refresh
                  sal_Int32 nLastChanceToSendRefreshRequest );

    void removeLock(const OUString& rURI);

    void refreshLocks();

private:
    void removeLockImpl(std::unique_lock<std::mutex> & rGuard, const OUString& rURI);
    void startTicker();
    void stopTicker(std::unique_lock<std::mutex> & rGuard);
};

} // namespace http_dav_ucp

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
