/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <basegfx/units/LengthUnitBase.hxx>

namespace gfx
{
typedef LengthUnitBase<sal_Int64> Length;

} // end namespace gfx

constexpr gfx::Length operator"" _emu(unsigned long long value) { return gfx::Length::emu(value); }
constexpr gfx::Length operator"" _in(unsigned long long value) { return gfx::Length::in(value); }
constexpr gfx::Length operator"" _cm(unsigned long long value) { return gfx::Length::cm(value); }
constexpr gfx::Length operator"" _mm(unsigned long long value) { return gfx::Length::mm(value); }
constexpr gfx::Length operator"" _hmm(unsigned long long value) { return gfx::Length::hmm(value); }
constexpr gfx::Length operator"" _twip(unsigned long long value)
{
    return gfx::Length::twip(value);
}
constexpr gfx::Length operator"" _pt(unsigned long long value) { return gfx::Length::pt(value); }
constexpr gfx::Length operator"" _px(unsigned long long value) { return gfx::Length::px(value); }

constexpr gfx::Length operator"" _in(long double value)
{
    return gfx::Length::emu(std::round(gfx::constFactor_in_to_EMU * value));
}
constexpr gfx::Length operator"" _cm(long double value)
{
    return gfx::Length::emu(std::round(gfx::constFactor_cm_to_EMU * value));
}

constexpr gfx::Length operator"" _mm(long double value)
{
    return gfx::Length::emu(std::round(gfx::constFactor_mm_to_EMU * value));
}

constexpr gfx::Length operator"" _hmm(long double value)
{
    return gfx::Length::emu(std::round(gfx::constFactor_hmm_to_EMU * value));
}

constexpr gfx::Length operator"" _twip(long double value)
{
    return gfx::Length::emu(std::round(gfx::constFactor_twip_to_EMU * value));
}

constexpr gfx::Length operator"" _pt(long double value)
{
    return gfx::Length::emu(std::round(gfx::constFactor_pt_to_EMU * value));
}

constexpr gfx::Length operator"" _px(long double value)
{
    return gfx::Length::emu(std::round(gfx::constFactor_px_to_EMU * value));
}

/** Write to char stream */
template <typename charT, typename traits>
inline std::basic_ostream<charT, traits>& operator<<(std::basic_ostream<charT, traits>& stream,
                                                     const gfx::Length& rLength)
{
    return stream << rLength.raw() << " (twip=" << rLength.as_twip() << ", hmm=" << rLength.as_hmm()
                  << ")";
}
