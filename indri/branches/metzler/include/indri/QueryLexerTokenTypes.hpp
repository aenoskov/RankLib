#ifndef INC_QueryLexerTokenTypes_hpp_
#define INC_QueryLexerTokenTypes_hpp_

ANTLR_BEGIN_NAMESPACE(indri)
ANTLR_BEGIN_NAMESPACE(lang)
/* $ANTLR 2.7.5 (20050128): "indrilang.g" -> "QueryLexerTokenTypes.hpp"$ */

#ifndef CUSTOM_API
# define CUSTOM_API
#endif

#ifdef __cplusplus
struct CUSTOM_API QueryLexerTokenTypes {
#endif
	enum {
		EOF_ = 1,
		SUM = 4,
		WSUM = 5,
		WAND = 6,
		OD = 7,
		OR = 8,
		IDENTSIM = 9,
		NOT = 10,
		UW = 11,
		COMBINE = 12,
		WEIGHT = 13,
		MAX = 14,
		FILREQ = 15,
		FILREJ = 16,
		ANY = 17,
		BAND = 18,
		WSYN = 19,
		SYN = 20,
		PRIOR = 21,
		DATEAFTER = 22,
		DATEBEFORE = 23,
		DATEBETWEEN = 24,
		LESS = 25,
		GREATER = 26,
		BETWEEN = 27,
		EQUALS = 28,
		NUMBER = 29,
		NEGATIVE_NUMBER = 30,
		FLOAT = 31,
		STAR = 32,
		O_PAREN = 33,
		C_PAREN = 34,
		O_ANGLE = 35,
		C_ANGLE = 36,
		O_SQUARE = 37,
		C_SQUARE = 38,
		O_BRACE = 39,
		C_BRACE = 40,
		DBL_QUOTE = 41,
		QUOTE = 42,
		DOT = 43,
		COMMA = 44,
		DASH = 45,
		COLON = 46,
		TAB = 47,
		CR = 48,
		LF = 49,
		SPACE = 50,
		HIGH_CHAR = 51,
		DIGIT = 52,
		ASCII_LETTER = 53,
		SAFE_LETTER = 54,
		SAFE_CHAR = 55,
		BASESIXFOUR_CHAR = 56,
		TEXT_TERM = 57,
		TERM = 58,
		ENCODED_QUOTED_TERM = 59,
		ENCODED_TERM = 60,
		OPERATOR = 61,
		JUNK = 62,
		SLASH = 63,
		NULL_TREE_LOOKAHEAD = 3
	};
#ifdef __cplusplus
};
#endif
ANTLR_END_NAMESPACE
ANTLR_END_NAMESPACE
#endif /*INC_QueryLexerTokenTypes_hpp_*/
