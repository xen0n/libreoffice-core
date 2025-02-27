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

#include <dsmeta.hxx>
#include <connectivity/DriversConfig.hxx>
#include <dsntypes.hxx>
#include <comphelper/processfactory.hxx>

#include <map>
#include <utility>

namespace dbaui
{

    using namespace dbaccess;
    using namespace ::com::sun::star;

    namespace {

    struct FeatureSupport
    {
        // authentication mode of the data source
        AuthenticationMode      eAuthentication;

        FeatureSupport()
            :eAuthentication( AuthUserPwd )
        {
        }

        explicit FeatureSupport(AuthenticationMode Auth)
            :eAuthentication( Auth )
        {
        }
    };

    struct FeatureMapping
    {
        /// one of the items from dsitems.hxx
        ItemID      nItemID;
        OUString pAsciiFeatureName;
    };

    // global tables
    const FeatureMapping s_aMappings[] = {
        { DSID_AUTORETRIEVEENABLED,     "GeneratedValues" },
        { DSID_AUTOINCREMENTVALUE,      "GeneratedValues" },
        { DSID_AUTORETRIEVEVALUE,       "GeneratedValues" },
        { DSID_SQL92CHECK,              "UseSQL92NamingConstraints" },
        { DSID_APPEND_TABLE_ALIAS,      "AppendTableAliasInSelect" },
        { DSID_AS_BEFORE_CORRNAME,      "UseKeywordAsBeforeAlias" },
        { DSID_ENABLEOUTERJOIN,         "UseBracketedOuterJoinSyntax" },
        { DSID_IGNOREDRIVER_PRIV,       "IgnoreDriverPrivileges" },
        { DSID_PARAMETERNAMESUBST,      "ParameterNameSubstitution" },
        { DSID_SUPPRESSVERSIONCL,       "DisplayVersionColumns" },
        { DSID_CATALOG,                 "UseCatalogInSelect" },
        { DSID_SCHEMA,                  "UseSchemaInSelect" },
        { DSID_INDEXAPPENDIX,           "UseIndexDirectionKeyword" },
        { DSID_DOSLINEENDS,             "UseDOSLineEnds" },
        { DSID_BOOLEANCOMPARISON,       "BooleanComparisonMode" },
        { DSID_CHECK_REQUIRED_FIELDS,   "FormsCheckRequiredFields" },
        { DSID_IGNORECURRENCY,          "IgnoreCurrency" },
        { DSID_ESCAPE_DATETIME,         "EscapeDateTime" },
        { DSID_PRIMARY_KEY_SUPPORT,     "PrimaryKeySupport" },
        { DSID_RESPECTRESULTSETTYPE,    "RespectDriverResultSetType" },
        { DSID_MAX_ROW_SCAN,            "MaxRowScan" },
    };
    }

    static const FeatureSet& lcl_getFeatureSet( const OUString& _rURL )
    {
        typedef std::map< OUString, FeatureSet > FeatureSets;
        static FeatureSets s_aFeatureSets = []()
        {
            FeatureSets tmp;
            ::connectivity::DriversConfig aDriverConfig( ::comphelper::getProcessComponentContext() );
            const uno::Sequence< OUString > aPatterns = aDriverConfig.getURLs();
            for ( auto const & pattern : aPatterns )
            {
                FeatureSet aCurrentSet;
                const ::comphelper::NamedValueCollection aCurrentFeatures( aDriverConfig.getFeatures( pattern ).getNamedValues() );

                for ( const FeatureMapping& rFeatureMapping : s_aMappings )
                {
                    if ( aCurrentFeatures.has( rFeatureMapping.pAsciiFeatureName ) )
                        aCurrentSet.put( rFeatureMapping.nItemID );
                }

                tmp[ pattern ] = aCurrentSet;
            }
            return tmp;
        }();

        OSL_ENSURE( s_aFeatureSets.find( _rURL ) != s_aFeatureSets.end(), "invalid URL/pattern!" );
        return s_aFeatureSets[ _rURL ];
    }

    static AuthenticationMode getAuthenticationMode( const OUString& _sURL )
    {
        static std::map< OUString, FeatureSupport > s_aSupport = []()
        {
            std::map< OUString, FeatureSupport > tmp;
            ::connectivity::DriversConfig aDriverConfig(::comphelper::getProcessComponentContext());
            const uno::Sequence< OUString > aURLs = aDriverConfig.getURLs();
            const OUString* pIter = aURLs.getConstArray();
            const OUString* pEnd = pIter + aURLs.getLength();
            for(;pIter != pEnd;++pIter)
            {
                FeatureSupport aInit( AuthNone );
                const ::comphelper::NamedValueCollection& aMetaData = aDriverConfig.getMetaData(*pIter);
                if ( aMetaData.has("Authentication") )
                {
                    OUString sAuth;
                    aMetaData.get("Authentication") >>= sAuth;
                    if ( sAuth == "UserPassword" )
                        aInit = FeatureSupport(AuthUserPwd);
                    else if ( sAuth == "Password" )
                        aInit = FeatureSupport(AuthPwd);
                }
                tmp.insert(std::make_pair(*pIter,aInit));
            }
            return tmp;
        }();
        OSL_ENSURE(s_aSupport.find(_sURL) != s_aSupport.end(),"Illegal URL!");
        return s_aSupport[ _sURL ].eAuthentication;
    }

    // DataSourceMetaData_Impl
    class DataSourceMetaData_Impl
    {
    public:
        explicit DataSourceMetaData_Impl(OUString sURL);

        const OUString& getType() const { return m_sURL; }

    private:
        const OUString m_sURL;
    };

    DataSourceMetaData_Impl::DataSourceMetaData_Impl( OUString _sURL )
        :m_sURL(std::move( _sURL ))
    {
    }

    // DataSourceMetaData
    DataSourceMetaData::DataSourceMetaData( const OUString& _sURL )
        :m_pImpl( std::make_shared<DataSourceMetaData_Impl>( _sURL ) )
    {
    }

    DataSourceMetaData::~DataSourceMetaData()
    {
    }

    const FeatureSet& DataSourceMetaData::getFeatureSet() const
    {
        return lcl_getFeatureSet( m_pImpl->getType() );
    }

    AuthenticationMode  DataSourceMetaData::getAuthentication( const OUString& _sURL )
    {
        return getAuthenticationMode( _sURL );
    }

} // namespace dbaui

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
