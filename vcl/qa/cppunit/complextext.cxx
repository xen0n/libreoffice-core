/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <config_features.h>
#include <config_fonts.h>

#include <ostream>
#include <vector>
#include <tools/long.hxx>
#include <vcl/glyphitemcache.hxx>

#if HAVE_MORE_FONTS
// must be declared before inclusion of test/bootstrapfixture.hxx
static std::ostream& operator<<(std::ostream& rStream, const std::vector<sal_Int32>& rVec);
#endif
#include <test/bootstrapfixture.hxx>

#include <vcl/wrkwin.hxx>
#include <vcl/virdev.hxx>
// workaround MSVC2015 issue with std::unique_ptr
#include <sallayout.hxx>
#include <salgdi.hxx>


#include <ImplLayoutArgs.hxx>

#if HAVE_MORE_FONTS
static std::ostream& operator<<(std::ostream& rStream, const std::vector<sal_Int32>& rVec)
{
    rStream << "{ ";
    for (size_t i = 0; i < rVec.size() - 1; i++)
        rStream << rVec[i] << ", ";
    rStream << rVec.back();
    rStream << " }";
    return rStream;
}
#endif

class VclComplexTextTest : public test::BootstrapFixture
{
public:
    VclComplexTextTest() : BootstrapFixture(true, false) {}

    /// Play with font measuring etc.
    void testArabic();
    void testKashida();
    void testTdf95650(); // Windows-only issue
    void testCaching();
    void testCachingSubstring();

    CPPUNIT_TEST_SUITE(VclComplexTextTest);
    CPPUNIT_TEST(testArabic);
    CPPUNIT_TEST(testKashida);
    CPPUNIT_TEST(testTdf95650);
    CPPUNIT_TEST(testCaching);
    CPPUNIT_TEST(testCachingSubstring);
    CPPUNIT_TEST_SUITE_END();
};

void VclComplexTextTest::testArabic()
{
#if HAVE_MORE_FONTS
    OUString aOneTwoThree(u"واحِدْ إثٍنين ثلاثةٌ");
    ScopedVclPtrInstance<WorkWindow> pWin(static_cast<vcl::Window *>(nullptr));
    CPPUNIT_ASSERT( pWin );

    vcl::Font aFont("DejaVu Sans", "Book", Size(0, 12));

    OutputDevice *pOutDev = pWin->GetOutDev();
    pOutDev->SetFont( aFont );

    // absolute character widths AKA text array.
    std::vector<sal_Int32> aRefCharWidths {6,  9,  16, 16, 22, 22, 26, 29, 32, 32,
                                      36, 40, 49, 53, 56, 63, 63, 66, 72, 72};
    std::vector<sal_Int32> aCharWidths(aOneTwoThree.getLength(), 0);
    tools::Long nTextWidth = pOutDev->GetTextArray(aOneTwoThree, &aCharWidths);

    CPPUNIT_ASSERT_EQUAL(aRefCharWidths, aCharWidths);
    // this sporadically returns 75 or 74 on some of the windows tinderboxes eg. tb73
    CPPUNIT_ASSERT_EQUAL(tools::Long(72), nTextWidth);
    CPPUNIT_ASSERT_EQUAL(sal_Int32(nTextWidth), aCharWidths.back());

    // text advance width and line height
    CPPUNIT_ASSERT_EQUAL(tools::Long(72), pOutDev->GetTextWidth(aOneTwoThree));
    CPPUNIT_ASSERT_EQUAL(tools::Long(14), pOutDev->GetTextHeight());

    // exact bounding rectangle, not essentially the same as text width/height
    tools::Rectangle aBoundRect;
    pOutDev->GetTextBoundRect(aBoundRect, aOneTwoThree);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(0, aBoundRect.Left(), 1); // This sometimes equals to 1
    CPPUNIT_ASSERT_DOUBLES_EQUAL(1, aBoundRect.Top(), 1);
    CPPUNIT_ASSERT_DOUBLES_EQUAL(71, aBoundRect.getOpenWidth(), 1); // This sometimes equals to 70
    CPPUNIT_ASSERT_DOUBLES_EQUAL(15, aBoundRect.getOpenHeight(), 1);

#if 0
    // FIXME: This seems to be wishful thinking, GetTextRect() does not take
    // rotation into account.

    // normal orientation
    tools::Rectangle aInput;
    tools::Rectangle aRect = pOutDev->GetTextRect( aInput, aOneTwoThree );

    // now rotate 270 degrees
    vcl::Font aRotated( aFont );
    aRotated.SetOrientation( 2700 );
    pOutDev->SetFont( aRotated );
    tools::Rectangle aRectRot = pOutDev->GetTextRect( aInput, aOneTwoThree );

    // Check that we did do the rotation...
    fprintf( stderr, "%" SAL_PRIdINT64 " %" SAL_PRIdINT64 " %" SAL_PRIdINT64 " %" SAL_PRIdINT64 "\n",
             sal_Int64(aRect.GetWidth()), sal_Int64(aRect.GetHeight()),
             sal-Int64(aRectRot.GetWidth()), sal_Int64(aRectRot.GetHeight()) );
    CPPUNIT_ASSERT( aRectRot.GetWidth() == aRect.GetHeight() );
    CPPUNIT_ASSERT( aRectRot.GetHeight() == aRect.GetWidth() );
#endif
#endif
}

void VclComplexTextTest::testKashida()
{
#if HAVE_MORE_FONTS
    // Cache the glyph list of some Arabic text.
    ScopedVclPtrInstance<VirtualDevice> pOutputDevice;
    auto aText
        = OUString(u"عنصر الفوسفور عنصر فلزي صلب. تتكون الدورة الرابعة من 15 عنصرا.");
    std::unique_ptr<SalLayout> pLayout = pOutputDevice->ImplLayout(
        aText, 0, aText.getLength(), Point(0, 0), 0, {}, SalLayoutFlags::GlyphItemsOnly);
    SalLayoutGlyphs aGlyphs = pLayout->GetGlyphs();
    CPPUNIT_ASSERT(aGlyphs.IsValid());
    CPPUNIT_ASSERT(aGlyphs.Impl(0) != nullptr);

    // Now lay it out using the cached glyph list.
    vcl::text::ImplLayoutArgs aLayoutArgs(aText, 0, aText.getLength(), SalLayoutFlags::NONE,
                               pOutputDevice->GetFont().GetLanguageTag(), nullptr);
    pLayout = pOutputDevice->GetGraphics()->GetTextLayout(0);
    CPPUNIT_ASSERT(pLayout->LayoutText(aLayoutArgs, aGlyphs.Impl(0)));

    // Without the accompanying fix in place, this test would have failed with 'assertion failed'.
    // The kashida justification flag was lost when going via the glyph cache.
    CPPUNIT_ASSERT(aLayoutArgs.mnFlags & SalLayoutFlags::KashidaJustification);
#endif
}

void VclComplexTextTest::testTdf95650()
{
    static constexpr OUStringLiteral aTxt =
        u"\u0131\u0302\u0504\u4E44\u3031\u3030\u3531\u2D30"
        "\u3037\u0706\u0908\u0B0A\u0D0C\u0F0E\u072E\u100A"
        "\u0D11\u1312\u0105\u020A\u0512\u1403\u030C\u1528"
        "\u2931\u632E\u7074\u0D20\u0E0A\u100A\uF00D\u0D20"
        "\u030A\u0C0B\u20E0\u0A0D";
    ScopedVclPtrInstance<WorkWindow> pWin(static_cast<vcl::Window *>(nullptr));
    CPPUNIT_ASSERT(pWin);

    OutputDevice *pOutDev = pWin->GetOutDev();
    // Check that the following executes without failing assertion
    pOutDev->ImplLayout(aTxt, 9, 1, Point(), 0, {}, SalLayoutFlags::BiDiRtl);
}

static void checkCompareGlyphs( const SalLayoutGlyphs& aGlyphs1, const SalLayoutGlyphs& aGlyphs2,
    const std::string& message )
{
    CPPUNIT_ASSERT_EQUAL_MESSAGE(message, aGlyphs1.IsValid(), aGlyphs2.IsValid());
    // And check it's the same.
    for( int level = 0; level < MAX_FALLBACK; ++level )
    {
        const std::string messageLevel = OString(message.c_str()
            + OString::Concat(", level: ") + OString::number(level)).getStr();
        if( aGlyphs1.Impl(level) == nullptr)
        {
            CPPUNIT_ASSERT_MESSAGE(messageLevel, aGlyphs2.Impl(level) == nullptr);
            continue;
        }
        const SalLayoutGlyphsImpl* g1 = aGlyphs1.Impl(level);
        const SalLayoutGlyphsImpl* g2 = aGlyphs2.Impl(level);
        CPPUNIT_ASSERT_EQUAL_MESSAGE(messageLevel, g1->GetFont().get(), g2->GetFont().get());
        CPPUNIT_ASSERT_EQUAL_MESSAGE(messageLevel, g1->size(), g2->size());
        for( size_t i = 0; i < g1->size(); ++i )
        {
            const bool equal = (*g1)[i] == (*g2)[i];
            CPPUNIT_ASSERT_MESSAGE(messageLevel, equal);
        }
    }
}

static void testCachedGlyphs( const OUString& aText, const OUString& aFontName )
{
    const std::string message = OUString("Font: " + aFontName + ", text: '" + aText + "'").toUtf8().getStr();
    ScopedVclPtrInstance<VirtualDevice> pOutputDevice;
    vcl::Font aFont( aFontName, Size(0, 12));
    pOutputDevice->SetFont( aFont );
    SalLayoutGlyphsCache::self()->clear();
    // Get the glyphs for the text.
    std::unique_ptr<SalLayout> pLayout1 = pOutputDevice->ImplLayout(
        aText, 0, aText.getLength(), Point(0, 0), 0, {}, SalLayoutFlags::GlyphItemsOnly);
    SalLayoutGlyphs aGlyphs1 = pLayout1->GetGlyphs();
    // Reuse the cached glyphs to get glyphs again.
    std::unique_ptr<SalLayout> pLayout2 = pOutputDevice->ImplLayout(
        aText, 0, aText.getLength(), Point(0, 0), 0, {}, SalLayoutFlags::GlyphItemsOnly, nullptr, &aGlyphs1);
    SalLayoutGlyphs aGlyphs2 = pLayout2->GetGlyphs();
    checkCompareGlyphs(aGlyphs1, aGlyphs2, message + " (reuse)");
    // Get cached glyphs from SalLayoutGlyphsCache.
    const SalLayoutGlyphs* aGlyphs3 = SalLayoutGlyphsCache::self()->GetLayoutGlyphs(
        pOutputDevice, aText, 0, aText.getLength(), 0);
    CPPUNIT_ASSERT_MESSAGE(message, aGlyphs3 != nullptr);
    checkCompareGlyphs(aGlyphs1, *aGlyphs3, message + " (cache)");
}

// Check that caching using SalLayoutGlyphs gives same results as without caching.
// This should preferably use fonts that come with LO.
void VclComplexTextTest::testCaching()
{
    // Just something basic, no font fallback.
    testCachedGlyphs( "test", "Dejavu Sans" );
    // This font does not have latin characters, will need fallback.
    testCachedGlyphs( "test", "KacstBook" );
}

static void testCachedGlyphsSubstring( const OUString& aText, const OUString& aFontName, bool rtl )
{
    const std::string prefix = OUString("Font: " + aFontName + ", text: '" + aText + "'").toUtf8().getStr();
    ScopedVclPtrInstance<VirtualDevice> pOutputDevice;
    // BiDiStrong is needed, otherwise SalLayoutGlyphsImpl::cloneCharRange() will not do anything.
    vcl::text::ComplexTextLayoutFlags layoutFlags = vcl::text::ComplexTextLayoutFlags::BiDiStrong;
    if(rtl)
        layoutFlags |= vcl::text::ComplexTextLayoutFlags::BiDiRtl;
    pOutputDevice->SetLayoutMode( layoutFlags );
    vcl::Font aFont( aFontName, Size(0, 12));
    pOutputDevice->SetFont( aFont );
    SalLayoutGlyphsCache::self()->clear();
    // Get the glyphs for the entire text once, to ensure the cache can built subsets from it.
    pOutputDevice->ImplLayout( aText, 0, aText.getLength(), Point(0, 0), 0, {}, SalLayoutFlags::GlyphItemsOnly);
    // Now check for all subsets. Some of them possibly do not make sense in practice, but the code
    // should cope with them.
    for( sal_Int32 len = 1; len <= aText.getLength(); ++len )
        for( sal_Int32 pos = 0; pos < aText.getLength() - len; ++pos )
        {
            std::string message = prefix + " (" + std::to_string(pos) + "/" + std::to_string(len) + ")";
            std::unique_ptr<SalLayout> pLayout1 = pOutputDevice->ImplLayout(
                aText, pos, len, Point(0, 0), 0, {}, SalLayoutFlags::GlyphItemsOnly);
            SalLayoutGlyphs aGlyphs1 = pLayout1->GetGlyphs();
            const SalLayoutGlyphs* aGlyphs2 = SalLayoutGlyphsCache::self()->GetLayoutGlyphs(
                pOutputDevice, aText, pos, len, 0);
            CPPUNIT_ASSERT_MESSAGE(message, aGlyphs2 != nullptr);
            checkCompareGlyphs(aGlyphs1, *aGlyphs2, message);
        }

}

// Check that SalLayoutGlyphsCache works properly when it builds a subset
// of glyphs using SalLayoutGlyphsImpl::cloneCharRange().
// This should preferably use fonts that come with LO.
void VclComplexTextTest::testCachingSubstring()
{
    // Just something basic.
    testCachedGlyphsSubstring( "test", "Dejavu Sans", false );
    // And complex arabic text, taken from tdf104649.docx .
    OUString text(u"فصل (پاره 2): درخواست حاجت از ديگران و برآوردن حاجت ديگران 90");
    testCachedGlyphsSubstring( text, "Dejavu Sans", true );
    // The text is RTL, but Writer will sometimes try to lay it out as LTR, for whatever reason
    // (tdf#149264)./ So make sure that gets handled properly too (SalLayoutGlyphsCache should
    // not use glyph subsets in that case).
    testCachedGlyphsSubstring( text, "Dejavu Sans", false );
}

CPPUNIT_TEST_SUITE_REGISTRATION(VclComplexTextTest);

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
