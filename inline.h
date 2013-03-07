/*    inline.h
 *
 *    Copyright (C) 2012 by Larry Wall and others
 *
 *    You may distribute under the terms of either the GNU General Public
 *    License or the Artistic License, as specified in the README file.
 *
 * This file is a home for static inline functions that cannot go in other
 * headers files, because they depend on proto.h (included after most other
 * headers) or struct definitions.
 *
 * Each section names the header file that the functions "belong" to.
 */

/* ------------------------------- av.h ------------------------------- */

PERL_STATIC_INLINE I32
S_av_top_index(pTHX_ AV *av)
{
    PERL_ARGS_ASSERT_AV_TOP_INDEX;
    assert(SvTYPE(av) == SVt_PVAV);

    return AvFILL(av);
}

/* ------------------------------- cv.h ------------------------------- */

PERL_STATIC_INLINE I32 *
S_CvDEPTHp(const CV * const sv)
{
    assert(SvTYPE(sv) == SVt_PVCV || SvTYPE(sv) == SVt_PVFM);
    return &((XPVCV*)SvANY(sv))->xcv_depth;
}

/* ----------------------------- regexp.h ----------------------------- */

PERL_STATIC_INLINE struct regexp *
S_ReANY(const REGEXP * const re)
{
    assert(isREGEXP(re));
    return re->sv_u.svu_rx;
}

/* ------------------------------- sv.h ------------------------------- */

PERL_STATIC_INLINE SV *
S_SvREFCNT_inc(SV *sv)
{
    if (LIKELY(sv != NULL))
	SvREFCNT(sv)++;
    return sv;
}
PERL_STATIC_INLINE SV *
S_SvREFCNT_inc_NN(SV *sv)
{
    SvREFCNT(sv)++;
    return sv;
}
PERL_STATIC_INLINE void
S_SvREFCNT_inc_void(SV *sv)
{
    if (LIKELY(sv != NULL))
	SvREFCNT(sv)++;
}
PERL_STATIC_INLINE void
S_SvREFCNT_dec(pTHX_ SV *sv)
{
    if (LIKELY(sv != NULL)) {
	U32 rc = SvREFCNT(sv);
	if (rc > 1)
	    SvREFCNT(sv) = rc - 1;
	else
	    Perl_sv_free2(aTHX_ sv, rc);
    }
}

PERL_STATIC_INLINE void
S_SvREFCNT_dec_NN(pTHX_ SV *sv)
{
    U32 rc = SvREFCNT(sv);
    if (rc > 1)
	SvREFCNT(sv) = rc - 1;
    else
	Perl_sv_free2(aTHX_ sv, rc);
}

PERL_STATIC_INLINE void
SvAMAGIC_on(SV *sv)
{
    assert(SvROK(sv));
    if (SvOBJECT(SvRV(sv))) HvAMAGIC_on(SvSTASH(SvRV(sv)));
}
PERL_STATIC_INLINE void
SvAMAGIC_off(SV *sv)
{
    if (SvROK(sv) && SvOBJECT(SvRV(sv)))
	HvAMAGIC_off(SvSTASH(SvRV(sv)));
}

PERL_STATIC_INLINE U32
S_SvPADTMP_on(SV *sv)
{
    assert(!(SvFLAGS(sv) & SVs_PADMY));
    return SvFLAGS(sv) |= SVs_PADTMP;
}
PERL_STATIC_INLINE U32
S_SvPADTMP_off(SV *sv)
{
    assert(!(SvFLAGS(sv) & SVs_PADMY));
    return SvFLAGS(sv) &= ~SVs_PADTMP;
}
PERL_STATIC_INLINE U32
S_SvPADSTALE_on(SV *sv)
{
    assert(SvFLAGS(sv) & SVs_PADMY);
    return SvFLAGS(sv) |= SVs_PADSTALE;
}
PERL_STATIC_INLINE U32
S_SvPADSTALE_off(SV *sv)
{
    assert(SvFLAGS(sv) & SVs_PADMY);
    return SvFLAGS(sv) &= ~SVs_PADSTALE;
}
#ifdef PERL_CORE
PERL_STATIC_INLINE STRLEN
S_sv_or_pv_pos_u2b(pTHX_ SV *sv, const char *pv, STRLEN pos, STRLEN *lenp)
{
    if (SvGAMAGIC(sv)) {
	U8 *hopped = utf8_hop((U8 *)pv, pos);
	if (lenp) *lenp = (STRLEN)(utf8_hop(hopped, *lenp) - hopped);
	return (STRLEN)(hopped - (U8 *)pv);
    }
    return sv_pos_u2b_flags(sv,pos,lenp,SV_CONST_RETURN);
}
#endif

/* ------------------------------- handy.h ------------------------------- */

/* saves machine code for a common noreturn idiom typically used in Newx*() */
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-function"
#endif
static void
S_croak_memory_wrap(void)
{
    Perl_croak_nocontext("%s",PL_memory_wrap);
}
#ifdef __clang__
#pragma clang diagnostic pop
#endif

#ifdef BOOTSTRAP_CHARSET
static bool
S_bootstrap_ctype(U8 character, UV classnum, bool full_Latin1)
{
    /* See comments in handy.h.  This is placed in this file primarily to avoid
     * having to have an entry for it in embed.fnc */

    dTHX;

    if (! full_Latin1 && ! isASCII(character)) {
        return FALSE;
    }

    switch (classnum) {
        case _CC_ALPHANUMERIC: return isALPHANUMERIC_L1(character);
        case _CC_ALPHA:     return isALPHA_L1(character);
        case _CC_ASCII:     return isASCII_L1(character);
        case _CC_BLANK:     return isBLANK_L1(character);
        case _CC_CASED:     return isLOWER_L1(character)
                                   || isUPPER_L1(character);
        case _CC_CNTRL:     return isCNTRL_L1(character);
        case _CC_DIGIT:     return isDIGIT_L1(character);
        case _CC_GRAPH:     return isGRAPH_L1(character);
        case _CC_LOWER:     return isLOWER_L1(character);
        case _CC_PRINT:     return isPRINT_L1(character);
        case _CC_PSXSPC:    return isPSXSPC_L1(character);
        case _CC_PUNCT:     return isPUNCT_L1(character);
        case _CC_SPACE:     return isSPACE_L1(character);
        case _CC_UPPER:     return isUPPER_L1(character);
        case _CC_WORDCHAR:  return isWORDCHAR_L1(character);
        case _CC_XDIGIT:    return isXDIGIT_L1(character);
        case _CC_VERTSPACE: return isSPACE_L1(character) && ! isBLANK_L1(character);
        case _CC_IDFIRST:   return isIDFIRST_L1(character);
        case _CC_QUOTEMETA: return _isQUOTEMETA(character);
        case _CC_CHARNAME_CONT: return isCHARNAME_CONT(character);
        case _CC_NONLATIN1_FOLD: return _HAS_NONLATIN1_FOLD_CLOSURE_ONLY_FOR_USE_BY_REGCOMP_DOT_C_AND_REGEXEC_DOT_C(character);
        case _CC_NON_FINAL_FOLD: return _IS_NON_FINAL_FOLD_ONLY_FOR_USE_BY_REGCOMP_DOT_C(character);
        case _CC_IS_IN_SOME_FOLD: return _IS_IN_SOME_FOLD_ONLY_FOR_USE_BY_REGCOMP_DOT_C(character);
        case _CC_BACKSLASH_FOO_LBRACE_IS_META: return 0;


        default: break;
    }
    Perl_croak(aTHX_ "panic: bootstrap_ctype() has an unexpected character class '%"UVxf"'", classnum);
}
#endif

/* ------------------------------- utf8.h ------------------------------- */

PERL_STATIC_INLINE void
S_append_utf8_from_native_byte(const U8 byte, U8** dest)
{
    /* Takes an input 'byte' (Latin1 or EBCDIC) and appends it to the UTF-8
     * encoded string at '*dest', updating '*dest' to include it */

    PERL_ARGS_ASSERT_APPEND_UTF8_FROM_NATIVE_BYTE;

    if (NATIVE_IS_INVARIANT(byte))
        *(*dest)++ = byte;
    else {
        *(*dest)++ = UTF8_EIGHT_BIT_HI(byte);
        *(*dest)++ = UTF8_EIGHT_BIT_LO(byte);
    }
}

/* These two exist only to replace the macros they formerly were so that their
 * use can be deprecated */

PERL_STATIC_INLINE bool
S_isIDFIRST_lazy(pTHX_ const char* p)
{
    PERL_ARGS_ASSERT_ISIDFIRST_LAZY;

    return isIDFIRST_lazy_if(p,1);
}

PERL_STATIC_INLINE bool
S_isALNUM_lazy(pTHX_ const char* p)
{
    PERL_ARGS_ASSERT_ISALNUM_LAZY;

    return isALNUM_lazy_if(p,1);
}
