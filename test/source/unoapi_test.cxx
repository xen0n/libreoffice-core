/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <test/unoapi_test.hxx>

#include <com/sun/star/util/XCloseable.hpp>
#include <com/sun/star/frame/Desktop.hpp>
#include <comphelper/processfactory.hxx>
#include <utility>

using namespace css;
using namespace css::uno;

UnoApiTest::UnoApiTest(OUString path)
    : m_aBaseString(std::move(path))
{
}

void UnoApiTest::setUp()
{
    test::BootstrapFixture::setUp();

    mxDesktop
        = css::frame::Desktop::create(comphelper::getComponentContext(getMultiServiceFactory()));
    CPPUNIT_ASSERT_MESSAGE("no desktop!", mxDesktop.is());
}

void UnoApiTest::createFileURL(std::u16string_view aFileBase, OUString& rFilePath)
{
    rFilePath = m_directories.getSrcRootURL() + m_aBaseString + "/" + aFileBase;
}

void UnoApiTest::closeDocument(uno::Reference<lang::XComponent> const& xDocument)
{
    uno::Reference<util::XCloseable> xCloseable(xDocument, UNO_QUERY_THROW);
    xCloseable->close(false);
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
