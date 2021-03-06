/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef nsMacRomanToUnicode_h___
#define nsMacRomanToUnicode_h___

#include "nsISupports.h"

// Class ID for our MacRomanToUnicode charset converter
// {7B8556A1-EC79-11d2-8AAC-00600811A836}
#define NS_MACROMANTOUNICODE_CID \
  { 0x7b8556a1, 0xec79, 0x11d2, {0x8a, 0xac, 0x0, 0x60, 0x8, 0x11, 0xa8, 0x36}}

#define NS_MACROMANTOUNICODE_CONTRACTID "@mozilla.org/intl/unicode/decoder;1?charset=macintosh"

//#define NS_ERROR_UCONV_NOMACROMANTOUNICODE   
//  NS_ERROR_GENERATE_FAILURE(NS_ERROR_MODULE_UCONV, 0x31)

/**
 * A character set converter from MacRoman to Unicode.
 *
 * @created         05/Apr/1999
 * @author  Catalin Rotaru [CATA]
 */
nsresult
nsMacRomanToUnicodeConstructor(nsISupports *aOuter, REFNSIID aIID,
                               void **aResult);

#endif /* nsMacRomanToUnicode_h___ */
