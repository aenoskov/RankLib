/* $ANTLR 2.7.4: "indrilang.g" -> "QueryParser.cpp"$ */
#include "indri/QueryParser.hpp"
#include <antlr/NoViableAltException.hpp>
#include <antlr/SemanticException.hpp>
#include <antlr/ASTFactory.hpp>
ANTLR_BEGIN_NAMESPACE(indri)
ANTLR_BEGIN_NAMESPACE(lang)
#line 1 "indrilang.g"
#line 8 "QueryParser.cpp"
QueryParser::QueryParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,k)
{
}

QueryParser::QueryParser(ANTLR_USE_NAMESPACE(antlr)TokenBuffer& tokenBuf)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(tokenBuf,1)
{
}

QueryParser::QueryParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer, int k)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,k)
{
}

QueryParser::QueryParser(ANTLR_USE_NAMESPACE(antlr)TokenStream& lexer)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(lexer,1)
{
}

QueryParser::QueryParser(const ANTLR_USE_NAMESPACE(antlr)ParserSharedInputState& state)
: ANTLR_USE_NAMESPACE(antlr)LLkParser(state,1)
{
}

 indri::lang::ScoredExtentNode*  QueryParser::query() {
#line 164 "indrilang.g"
	 indri::lang::ScoredExtentNode* q ;
#line 37 "QueryParser.cpp"
#line 164 "indrilang.g"
	
	indri::lang::CombineNode* c = 0;
	indri::lang::ScoredExtentNode* s = 0;
	q = 0;
	
#line 44 "QueryParser.cpp"
	
	try {      // for error handling
		q=scoredExtentNode();
		{
		switch ( LA(1)) {
		case WSUM:
		case WAND:
		case OD:
		case OR:
		case IDENTSIM:
		case NOT:
		case UW:
		case COMBINE:
		case WEIGHT:
		case MAX:
		case FILREQ:
		case FILREJ:
		case ANY:
		case BAND:
		case SYN:
		case PRIOR:
		case DATEAFTER:
		case DATEBEFORE:
		case DATEBETWEEN:
		case LESS:
		case GREATER:
		case BETWEEN:
		case EQUALS:
		case NUMBER:
		case FLOAT:
		case O_ANGLE:
		case O_BRACE:
		case DBL_QUOTE:
		case TERM:
		case ENCODED_QUOTED_TERM:
		case ENCODED_TERM:
		case OPERATOR:
		{
			s=scoredExtentNode();
			if ( inputState->guessing==0 ) {
#line 171 "indrilang.g"
				
				c = new CombineNode;
				c->addChild(q);
				c->addChild(s);
				_nodes.push_back(c);
				q = c;
				
#line 93 "QueryParser.cpp"
			}
			{ // ( ... )*
			for (;;) {
				if ((_tokenSet_0.member(LA(1)))) {
					s=scoredExtentNode();
					if ( inputState->guessing==0 ) {
#line 179 "indrilang.g"
						
						c->addChild(s);
						
#line 104 "QueryParser.cpp"
					}
				}
				else {
					goto _loop82;
				}
				
			}
			_loop82:;
			} // ( ... )*
			break;
		}
		case ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(ANTLR_USE_NAMESPACE(antlr)Token::EOF_TYPE);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_1);
		} else {
			throw;
		}
	}
	return q ;
}

 indri::lang::ScoredExtentNode*  QueryParser::scoredExtentNode() {
#line 185 "indrilang.g"
	 indri::lang::ScoredExtentNode* s ;
#line 143 "QueryParser.cpp"
	
	try {      // for error handling
		switch ( LA(1)) {
		case WEIGHT:
		{
			s=weightNode();
			break;
		}
		case COMBINE:
		{
			s=combineNode();
			break;
		}
		case OR:
		{
			s=orNode();
			break;
		}
		case IDENTSIM:
		{
			s=identSimNode();
			break;
		}
		case NOT:
		{
			s=notNode();
			break;
		}
		case WAND:
		{
			s=wandNode();
			break;
		}
		case WSUM:
		{
			s=wsumNode();
			break;
		}
		case MAX:
		{
			s=maxNode();
			break;
		}
		case PRIOR:
		{
			s=priorNode();
			break;
		}
		case FILREJ:
		{
			s=filrejNode();
			break;
		}
		case FILREQ:
		{
			s=filreqNode();
			break;
		}
		case OD:
		case UW:
		case ANY:
		case BAND:
		case SYN:
		case DATEAFTER:
		case DATEBEFORE:
		case DATEBETWEEN:
		case LESS:
		case GREATER:
		case BETWEEN:
		case EQUALS:
		case NUMBER:
		case FLOAT:
		case O_ANGLE:
		case O_BRACE:
		case DBL_QUOTE:
		case TERM:
		case ENCODED_QUOTED_TERM:
		case ENCODED_TERM:
		case OPERATOR:
		{
			s=scoredRaw();
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return s ;
}

 indri::lang::ScoredExtentNode*  QueryParser::weightNode() {
#line 282 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 248 "QueryParser.cpp"
#line 282 "indrilang.g"
	
	indri::lang::WeightNode* wn = new indri::lang::WeightNode;
	_nodes.push_back(wn);
	
#line 254 "QueryParser.cpp"
	
	try {      // for error handling
		match(WEIGHT);
		r=weightedList(wn);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::combineNode() {
#line 289 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 275 "QueryParser.cpp"
#line 289 "indrilang.g"
	
	indri::lang::CombineNode* cn = new indri::lang::CombineNode;
	_nodes.push_back(cn);
	
#line 281 "QueryParser.cpp"
	
	try {      // for error handling
		match(COMBINE);
		r=unweightedList(cn);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::orNode() {
#line 317 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 302 "QueryParser.cpp"
#line 317 "indrilang.g"
	
	indri::lang::OrNode* on = new indri::lang::OrNode;
	_nodes.push_back(on);
	
#line 308 "QueryParser.cpp"
	
	try {      // for error handling
		match(OR);
		r=unweightedList(on);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::identSimNode() {
#line 324 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 329 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  variation = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 324 "indrilang.g"
	
	indri::lang::IdentSimNode* isn = new indri::lang::IdentSimNode;
	_nodes.push_back(isn);
	
#line 336 "QueryParser.cpp"
	
	try {      // for error handling
		match(IDENTSIM);
		variation = LT(1);
		match(NUMBER);
		r=unweightedList(isn);
		if ( inputState->guessing==0 ) {
#line 329 "indrilang.g"
			
			isn->setVariation( variation->getText() );
			
#line 348 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::notNode() {
#line 340 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 366 "QueryParser.cpp"
#line 340 "indrilang.g"
	
	indri::lang::NotNode* n = new indri::lang::NotNode;
	indri::lang::ScoredExtentNode* c = 0;
	_nodes.push_back(n);
	r = n;
	
#line 374 "QueryParser.cpp"
	
	try {      // for error handling
		match(NOT);
		{
		switch ( LA(1)) {
		case O_SQUARE:
		{
			r=extentRestriction(r);
			break;
		}
		case O_PAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(O_PAREN);
		c=scoredExtentNode();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 348 "indrilang.g"
			
			n->setChild(c);
			
#line 403 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::wandNode() {
#line 310 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 421 "QueryParser.cpp"
#line 310 "indrilang.g"
	
	indri::lang::WAndNode* wn = new indri::lang::WAndNode;
	_nodes.push_back(wn);
	
#line 427 "QueryParser.cpp"
	
	try {      // for error handling
		match(WAND);
		r=weightedList(wn);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::wsumNode() {
#line 303 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 448 "QueryParser.cpp"
#line 303 "indrilang.g"
	
	indri::lang::WSumNode* wn = new indri::lang::WSumNode;
	_nodes.push_back(wn);
	
#line 454 "QueryParser.cpp"
	
	try {      // for error handling
		match(WSUM);
		r=weightedList(wn);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::maxNode() {
#line 333 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 475 "QueryParser.cpp"
#line 333 "indrilang.g"
	
	indri::lang::MaxNode* mn = new indri::lang::MaxNode;
	_nodes.push_back(mn);
	
#line 481 "QueryParser.cpp"
	
	try {      // for error handling
		match(MAX);
		r=unweightedList(mn);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return r ;
}

 indri::lang::PriorNode*  QueryParser::priorNode() {
#line 352 "indrilang.g"
	 indri::lang::PriorNode* p ;
#line 502 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  name = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 352 "indrilang.g"
	
	indri::lang::Field* field = 0;
	p = 0;
	
#line 509 "QueryParser.cpp"
	
	try {      // for error handling
		match(PRIOR);
		match(O_PAREN);
		name = LT(1);
		match(TERM);
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 357 "indrilang.g"
			
			p = _priorFactory->create( name->getText() );
			
			if( p->getFieldName().length() ) {
			field = new Field( p->getFieldName() );
			p->setField( field );
			_nodes.push_back(field);
			}
			
			_nodes.push_back(p);
			
#line 530 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return p ;
}

 indri::lang::FilRejNode*  QueryParser::filrejNode() {
#line 428 "indrilang.g"
	 indri::lang::FilRejNode* fj ;
#line 548 "QueryParser.cpp"
#line 428 "indrilang.g"
	
	RawExtentNode* filter = 0;
	ScoredExtentNode* disallowed = 0;
	
#line 554 "QueryParser.cpp"
	
	try {      // for error handling
		match(FILREJ);
		match(O_PAREN);
		filter=unscoredTerm();
		disallowed=scoredExtentNode();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 434 "indrilang.g"
			
			fj = new FilRejNode( filter, disallowed );
			_nodes.push_back(fj);
			
#line 568 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return fj ;
}

 indri::lang::FilReqNode*  QueryParser::filreqNode() {
#line 439 "indrilang.g"
	 indri::lang::FilReqNode* fq ;
#line 586 "QueryParser.cpp"
#line 439 "indrilang.g"
	
	RawExtentNode* filter = 0;
	ScoredExtentNode* required = 0;
	
#line 592 "QueryParser.cpp"
	
	try {      // for error handling
		match(FILREQ);
		match(O_PAREN);
		filter=unscoredTerm();
		required=scoredExtentNode();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 445 "indrilang.g"
			
			fq = new FilReqNode( filter, required );
			_nodes.push_back(fq);
			
#line 606 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return fq ;
}

 ScoredExtentNode*  QueryParser::scoredRaw() {
#line 205 "indrilang.g"
	 ScoredExtentNode* sn ;
#line 624 "QueryParser.cpp"
#line 205 "indrilang.g"
	
	RawExtentNode* raw = 0;
	RawExtentNode* contexts = 0;
	sn = 0;
	
#line 631 "QueryParser.cpp"
	
	try {      // for error handling
		bool synPredMatched108 = false;
		if (((_tokenSet_3.member(LA(1))))) {
			int _m108 = mark();
			synPredMatched108 = true;
			inputState->guessing++;
			try {
				{
				qualifiedTerm();
				context_list();
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched108 = false;
			}
			rewind(_m108);
			inputState->guessing--;
		}
		if ( synPredMatched108 ) {
			raw=qualifiedTerm();
			contexts=context_list();
			if ( inputState->guessing==0 ) {
#line 212 "indrilang.g"
				
				sn = new indri::lang::RawScorerNode( raw, contexts );
				_nodes.push_back(sn);
				
#line 660 "QueryParser.cpp"
			}
		}
		else {
			bool synPredMatched110 = false;
			if (((_tokenSet_3.member(LA(1))))) {
				int _m110 = mark();
				synPredMatched110 = true;
				inputState->guessing++;
				try {
					{
					qualifiedTerm();
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched110 = false;
				}
				rewind(_m110);
				inputState->guessing--;
			}
			if ( synPredMatched110 ) {
				raw=qualifiedTerm();
				if ( inputState->guessing==0 ) {
#line 217 "indrilang.g"
					
					sn = new indri::lang::RawScorerNode( raw, contexts );
					_nodes.push_back(sn);
					
#line 688 "QueryParser.cpp"
				}
			}
			else {
				bool synPredMatched112 = false;
				if (((_tokenSet_3.member(LA(1))))) {
					int _m112 = mark();
					synPredMatched112 = true;
					inputState->guessing++;
					try {
						{
						unqualifiedTerm();
						match(DOT);
						}
					}
					catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
						synPredMatched112 = false;
					}
					rewind(_m112);
					inputState->guessing--;
				}
				if ( synPredMatched112 ) {
					raw=unqualifiedTerm();
					match(DOT);
					contexts=context_list();
					if ( inputState->guessing==0 ) {
#line 222 "indrilang.g"
						
						sn = new indri::lang::RawScorerNode( raw, contexts );
						_nodes.push_back(sn);
						
#line 719 "QueryParser.cpp"
					}
				}
				else if ((_tokenSet_3.member(LA(1)))) {
					raw=unqualifiedTerm();
					if ( inputState->guessing==0 ) {
#line 227 "indrilang.g"
						
						sn = new indri::lang::RawScorerNode( raw, contexts );
						_nodes.push_back(sn);
						
#line 730 "QueryParser.cpp"
					}
				}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return sn ;
}

 RawExtentNode*  QueryParser::qualifiedTerm() {
#line 466 "indrilang.g"
	 RawExtentNode* t ;
#line 753 "QueryParser.cpp"
#line 466 "indrilang.g"
	
	RawExtentNode* synonyms = 0;
	RawExtentNode* fields = 0;
	t = 0;
	
#line 760 "QueryParser.cpp"
	
	try {      // for error handling
		synonyms=unqualifiedTerm();
		match(DOT);
		fields=field_list();
		if ( inputState->guessing==0 ) {
#line 473 "indrilang.g"
			
			if( fields ) {
			t = new indri::lang::ExtentInside( synonyms, fields );
			_nodes.push_back(t);
			} else {
			t = synonyms;
			}
			
#line 776 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_4);
		} else {
			throw;
		}
	}
	return t ;
}

 ExtentOr*  QueryParser::context_list() {
#line 560 "indrilang.g"
	 ExtentOr* contexts ;
#line 794 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  first = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  additional = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 560 "indrilang.g"
	
	contexts = new ExtentOr;
	_nodes.push_back( contexts );
	
#line 802 "QueryParser.cpp"
	
	try {      // for error handling
		match(O_PAREN);
		first = LT(1);
		match(TERM);
		if ( inputState->guessing==0 ) {
#line 566 "indrilang.g"
			
			Field* firstField = new indri::lang::Field( first->getText() );
			_nodes.push_back( firstField );
			contexts->addChild( firstField );
			
#line 815 "QueryParser.cpp"
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				additional = LT(1);
				match(TERM);
				if ( inputState->guessing==0 ) {
#line 573 "indrilang.g"
					
					Field* additionalField = new Field(additional->getText());
					_nodes.push_back( additionalField );
					contexts->addChild( additionalField );
					
#line 830 "QueryParser.cpp"
				}
			}
			else {
				goto _loop209;
			}
			
		}
		_loop209:;
		} // ( ... )*
		match(C_PAREN);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return contexts ;
}

 RawExtentNode*  QueryParser::unqualifiedTerm() {
#line 482 "indrilang.g"
	 RawExtentNode* re ;
#line 857 "QueryParser.cpp"
	
	try {      // for error handling
		switch ( LA(1)) {
		case OD:
		case OPERATOR:
		{
			re=odNode();
			break;
		}
		case UW:
		{
			re=uwNode();
			break;
		}
		case BAND:
		{
			re=bandNode();
			break;
		}
		case DATEBEFORE:
		{
			re=dateBefore();
			break;
		}
		case DATEAFTER:
		{
			re=dateAfter();
			break;
		}
		case DATEBETWEEN:
		{
			re=dateBetween();
			break;
		}
		case O_ANGLE:
		{
			re=synonym_list();
			break;
		}
		case O_BRACE:
		{
			re=synonym_list_brace();
			break;
		}
		case SYN:
		{
			re=synonym_list_alt();
			break;
		}
		case ANY:
		{
			re=anyField();
			break;
		}
		case LESS:
		{
			re=lessNode();
			break;
		}
		case GREATER:
		{
			re=greaterNode();
			break;
		}
		case BETWEEN:
		{
			re=betweenNode();
			break;
		}
		case EQUALS:
		{
			re=equalsNode();
			break;
		}
		case NUMBER:
		case FLOAT:
		case DBL_QUOTE:
		case TERM:
		case ENCODED_QUOTED_TERM:
		case ENCODED_TERM:
		{
			re=rawText();
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return re ;
}

 indri::lang::ScoredExtentNode*  QueryParser::weightedList(
	 indri::lang::WeightedCombinationNode* wn 
) {
#line 243 "indrilang.g"
	 indri::lang::ScoredExtentNode* sr ;
#line 965 "QueryParser.cpp"
#line 243 "indrilang.g"
	
	double w = 0;
	ScoredExtentNode* n = 0;
	sr = wn;
	
#line 972 "QueryParser.cpp"
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case O_SQUARE:
		{
			sr=extentRestriction(wn);
			break;
		}
		case O_PAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(O_PAREN);
		{ // ( ... )+
		int _cnt116=0;
		for (;;) {
			if ((LA(1) == NUMBER || LA(1) == FLOAT)) {
				w=floating();
				n=scoredExtentNode();
				if ( inputState->guessing==0 ) {
#line 254 "indrilang.g"
					wn->addChild( w, n );
#line 1002 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt116>=1 ) { goto _loop116; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt116++;
		}
		_loop116:;
		}  // ( ... )+
		match(C_PAREN);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return sr ;
}

 indri::lang::ExtentRestriction*  QueryParser::extentRestriction(
	 indri::lang::ScoredExtentNode* sn 
) {
#line 499 "indrilang.g"
	 indri::lang::ExtentRestriction* er ;
#line 1032 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 499 "indrilang.g"
	
	indri::lang::Field* f = 0;
	er = 0;
	
#line 1039 "QueryParser.cpp"
	
	try {      // for error handling
		match(O_SQUARE);
		field = LT(1);
		match(TERM);
		match(C_SQUARE);
		if ( inputState->guessing==0 ) {
#line 504 "indrilang.g"
			
			f = new indri::lang::Field(field->getText());
			_nodes.push_back(f);
			er = new indri::lang::ExtentRestriction(sn, f);
			_nodes.push_back(er);
			
#line 1054 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_6);
		} else {
			throw;
		}
	}
	return er ;
}

 double  QueryParser::floating() {
#line 710 "indrilang.g"
	 double d ;
#line 1072 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  f = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 710 "indrilang.g"
	
	d = 0;
	
#line 1079 "QueryParser.cpp"
	
	try {      // for error handling
		switch ( LA(1)) {
		case FLOAT:
		{
			f = LT(1);
			match(FLOAT);
			if ( inputState->guessing==0 ) {
#line 713 "indrilang.g"
				
				d = atof(f->getText().c_str());
				
#line 1092 "QueryParser.cpp"
			}
			break;
		}
		case NUMBER:
		{
			n = LT(1);
			match(NUMBER);
			if ( inputState->guessing==0 ) {
#line 716 "indrilang.g"
				
				d = atof(n->getText().c_str());
				
#line 1105 "QueryParser.cpp"
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_0);
		} else {
			throw;
		}
	}
	return d ;
}

 indri::lang::ScoredExtentNode*  QueryParser::sumList(
	 indri::lang::WSumNode* wn 
) {
#line 259 "indrilang.g"
	 indri::lang::ScoredExtentNode* sr ;
#line 1132 "QueryParser.cpp"
#line 259 "indrilang.g"
	
	double w = 0;
	ScoredExtentNode* n = 0;
	sr = wn;
	
#line 1139 "QueryParser.cpp"
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case O_SQUARE:
		{
			sr=extentRestriction(wn);
			break;
		}
		case O_PAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(O_PAREN);
		{ // ( ... )+
		int _cnt120=0;
		for (;;) {
			if ((_tokenSet_0.member(LA(1)))) {
				n=scoredExtentNode();
				if ( inputState->guessing==0 ) {
#line 267 "indrilang.g"
					wn->addChild( 1.0, n );
#line 1168 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt120>=1 ) { goto _loop120; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt120++;
		}
		_loop120:;
		}  // ( ... )+
		match(C_PAREN);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_1);
		} else {
			throw;
		}
	}
	return sr ;
}

 indri::lang::ScoredExtentNode*  QueryParser::unweightedList(
	 indri::lang::UnweightedCombinationNode* cn 
) {
#line 271 "indrilang.g"
	 indri::lang::ScoredExtentNode* sr ;
#line 1198 "QueryParser.cpp"
#line 271 "indrilang.g"
	
	ScoredExtentNode* n = 0;
	sr = cn;
	
#line 1204 "QueryParser.cpp"
	
	try {      // for error handling
		{
		switch ( LA(1)) {
		case O_SQUARE:
		{
			sr=extentRestriction(cn);
			break;
		}
		case O_PAREN:
		{
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(O_PAREN);
		{ // ( ... )+
		int _cnt124=0;
		for (;;) {
			if ((_tokenSet_0.member(LA(1)))) {
				n=scoredExtentNode();
				if ( inputState->guessing==0 ) {
#line 278 "indrilang.g"
					cn->addChild( n );
#line 1233 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt124>=1 ) { goto _loop124; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt124++;
		}
		_loop124:;
		}  // ( ... )+
		match(C_PAREN);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_2);
		} else {
			throw;
		}
	}
	return sr ;
}

 indri::lang::ScoredExtentNode*  QueryParser::sumNode() {
#line 296 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 1261 "QueryParser.cpp"
#line 296 "indrilang.g"
	
	indri::lang::WSumNode* wn = new indri::lang::WSumNode;
	_nodes.push_back(wn);
	
#line 1267 "QueryParser.cpp"
	
	try {      // for error handling
		match(SUM);
		r=sumList(wn);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_1);
		} else {
			throw;
		}
	}
	return r ;
}

 indri::lang::ODNode*  QueryParser::odNode() {
#line 379 "indrilang.g"
	 indri::lang::ODNode* od ;
#line 1288 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  n1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  n2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 379 "indrilang.g"
	
	RawExtentNode* rn = 0;
	od = new indri::lang::ODNode;
	_nodes.push_back(od);
	
#line 1297 "QueryParser.cpp"
	
	try {      // for error handling
		{
		bool synPredMatched139 = false;
		if (((LA(1) == OD))) {
			int _m139 = mark();
			synPredMatched139 = true;
			inputState->guessing++;
			try {
				{
				match(OD);
				match(NUMBER);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched139 = false;
			}
			rewind(_m139);
			inputState->guessing--;
		}
		if ( synPredMatched139 ) {
			{
			match(OD);
			n1 = LT(1);
			match(NUMBER);
			if ( inputState->guessing==0 ) {
#line 388 "indrilang.g"
				od->setWindowSize( n1->getText() );
#line 1326 "QueryParser.cpp"
			}
			}
		}
		else {
			bool synPredMatched142 = false;
			if (((LA(1) == OD))) {
				int _m142 = mark();
				synPredMatched142 = true;
				inputState->guessing++;
				try {
					{
					match(OD);
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched142 = false;
				}
				rewind(_m142);
				inputState->guessing--;
			}
			if ( synPredMatched142 ) {
				{
				match(OD);
				}
			}
			else if ((LA(1) == OPERATOR)) {
				{
				match(OPERATOR);
				n2 = LT(1);
				match(NUMBER);
				if ( inputState->guessing==0 ) {
#line 392 "indrilang.g"
					od->setWindowSize( n2->getText() );
#line 1360 "QueryParser.cpp"
				}
				}
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
		}
		match(O_PAREN);
		{ // ( ... )+
		int _cnt146=0;
		for (;;) {
			if ((_tokenSet_3.member(LA(1)))) {
				rn=unscoredTerm();
				if ( inputState->guessing==0 ) {
#line 396 "indrilang.g"
					od->addChild( rn );
#line 1378 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt146>=1 ) { goto _loop146; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt146++;
		}
		_loop146:;
		}  // ( ... )+
		match(C_PAREN);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return od ;
}

 RawExtentNode*  QueryParser::unscoredTerm() {
#line 459 "indrilang.g"
	 RawExtentNode* t ;
#line 1406 "QueryParser.cpp"
#line 459 "indrilang.g"
	
	t = 0;
	
#line 1411 "QueryParser.cpp"
	
	try {      // for error handling
		bool synPredMatched163 = false;
		if (((_tokenSet_3.member(LA(1))))) {
			int _m163 = mark();
			synPredMatched163 = true;
			inputState->guessing++;
			try {
				{
				qualifiedTerm();
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched163 = false;
			}
			rewind(_m163);
			inputState->guessing--;
		}
		if ( synPredMatched163 ) {
			t=qualifiedTerm();
		}
		else if ((_tokenSet_3.member(LA(1)))) {
			t=unqualifiedTerm();
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_7);
		} else {
			throw;
		}
	}
	return t ;
}

 indri::lang::UWNode*  QueryParser::uwNode() {
#line 400 "indrilang.g"
	 indri::lang::UWNode* uw ;
#line 1456 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 400 "indrilang.g"
	
	uw = new indri::lang::UWNode;
	RawExtentNode* rn = 0;
	_nodes.push_back(uw);
	
#line 1464 "QueryParser.cpp"
	
	try {      // for error handling
		{
		bool synPredMatched150 = false;
		if (((LA(1) == UW))) {
			int _m150 = mark();
			synPredMatched150 = true;
			inputState->guessing++;
			try {
				{
				match(UW);
				match(NUMBER);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched150 = false;
			}
			rewind(_m150);
			inputState->guessing--;
		}
		if ( synPredMatched150 ) {
			{
			match(UW);
			n = LT(1);
			match(NUMBER);
			if ( inputState->guessing==0 ) {
#line 408 "indrilang.g"
				uw->setWindowSize( n->getText() );
#line 1493 "QueryParser.cpp"
			}
			}
		}
		else if ((LA(1) == UW)) {
			{
			match(UW);
			}
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		
		}
		match(O_PAREN);
		{ // ( ... )+
		int _cnt154=0;
		for (;;) {
			if ((_tokenSet_3.member(LA(1)))) {
				rn=unscoredTerm();
				if ( inputState->guessing==0 ) {
#line 414 "indrilang.g"
					uw->addChild( rn );
#line 1516 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt154>=1 ) { goto _loop154; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt154++;
		}
		_loop154:;
		}  // ( ... )+
		match(C_PAREN);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return uw ;
}

 indri::lang::BAndNode*  QueryParser::bandNode() {
#line 417 "indrilang.g"
	 indri::lang::BAndNode* b ;
#line 1544 "QueryParser.cpp"
#line 417 "indrilang.g"
	
	b = new indri::lang::BAndNode;
	RawExtentNode* rn = 0;
	_nodes.push_back(b);
	
#line 1551 "QueryParser.cpp"
	
	try {      // for error handling
		match(BAND);
		match(O_PAREN);
		{ // ( ... )+
		int _cnt157=0;
		for (;;) {
			if ((_tokenSet_3.member(LA(1)))) {
				rn=unscoredTerm();
				if ( inputState->guessing==0 ) {
#line 425 "indrilang.g"
					b->addChild( rn );
#line 1564 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt157>=1 ) { goto _loop157; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt157++;
		}
		_loop157:;
		}  // ( ... )+
		match(C_PAREN);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return b ;
}

 indri::lang::Field*  QueryParser::anyField() {
#line 450 "indrilang.g"
	 indri::lang::Field* f ;
#line 1592 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 450 "indrilang.g"
	
	f = 0;
	
#line 1598 "QueryParser.cpp"
	
	try {      // for error handling
		match(ANY);
		match(COLON);
		t = LT(1);
		match(TERM);
		if ( inputState->guessing==0 ) {
#line 454 "indrilang.g"
			
			f = new Field(t->getText());
			_nodes.push_back(f);
			
#line 1611 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return f ;
}

 ExtentAnd*  QueryParser::field_list() {
#line 540 "indrilang.g"
	 ExtentAnd* fields ;
#line 1629 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  first = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  additional = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 540 "indrilang.g"
	
	fields = new ExtentAnd;
	_nodes.push_back( fields );
	
#line 1637 "QueryParser.cpp"
	
	try {      // for error handling
		first = LT(1);
		match(TERM);
		if ( inputState->guessing==0 ) {
#line 546 "indrilang.g"
			
			Field* firstField = new indri::lang::Field( first->getText() );
			_nodes.push_back( firstField );
			fields->addChild( firstField );
			
#line 1649 "QueryParser.cpp"
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				additional = LT(1);
				match(TERM);
				if ( inputState->guessing==0 ) {
#line 553 "indrilang.g"
					
					Field* additionalField = new Field(additional->getText());
					_nodes.push_back( additionalField );
					fields->addChild( additionalField );
					
#line 1664 "QueryParser.cpp"
				}
			}
			else {
				goto _loop206;
			}
			
		}
		_loop206:;
		} // ( ... )*
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_4);
		} else {
			throw;
		}
	}
	return fields ;
}

 FieldLessNode*  QueryParser::dateBefore() {
#line 589 "indrilang.g"
	 FieldLessNode* extent ;
#line 1690 "QueryParser.cpp"
#line 589 "indrilang.g"
	
	UINT64 d = 0;
	Field* dateField = 0;
	extent = 0;
	
#line 1697 "QueryParser.cpp"
	
	try {      // for error handling
		match(DATEBEFORE);
		match(O_PAREN);
		d=date();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 594 "indrilang.g"
			
			dateField = new Field("date");
			extent = new FieldLessNode( dateField, d );
			_nodes.push_back( dateField );
			_nodes.push_back( extent );
			
#line 1712 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return extent ;
}

 FieldGreaterNode*  QueryParser::dateAfter() {
#line 601 "indrilang.g"
	 FieldGreaterNode* extent ;
#line 1730 "QueryParser.cpp"
#line 601 "indrilang.g"
	
	UINT64 d = 0;
	Field* dateField = 0;
	extent = 0;
	
#line 1737 "QueryParser.cpp"
	
	try {      // for error handling
		match(DATEAFTER);
		match(O_PAREN);
		d=date();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 606 "indrilang.g"
			
			dateField = new Field("date");
			extent = new FieldGreaterNode( dateField, d );
			_nodes.push_back( dateField );
			_nodes.push_back( extent );
			
#line 1752 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return extent ;
}

 FieldBetweenNode*  QueryParser::dateBetween() {
#line 613 "indrilang.g"
	 FieldBetweenNode* extent ;
#line 1770 "QueryParser.cpp"
#line 613 "indrilang.g"
	
	UINT64 low = 0;
	UINT64 high = 0;
	Field* dateField = 0;
	extent = 0;
	
#line 1778 "QueryParser.cpp"
	
	try {      // for error handling
		match(DATEBETWEEN);
		match(O_PAREN);
		low=date();
		match(COMMA);
		high=date();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 619 "indrilang.g"
			
			dateField = new Field("date");
			extent = new FieldBetweenNode( dateField, low, high );
			_nodes.push_back( dateField );
			_nodes.push_back( extent );
			
#line 1795 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return extent ;
}

 indri::lang::ExtentOr*  QueryParser::synonym_list() {
#line 511 "indrilang.g"
	 indri::lang::ExtentOr* s ;
#line 1813 "QueryParser.cpp"
#line 511 "indrilang.g"
	
	indri::lang::RawExtentNode* term = 0;
	s = new indri::lang::ExtentOr;
	_nodes.push_back(s);
	
#line 1820 "QueryParser.cpp"
	
	try {      // for error handling
		match(O_ANGLE);
		{ // ( ... )+
		int _cnt197=0;
		for (;;) {
			if ((_tokenSet_3.member(LA(1)))) {
				term=unscoredTerm();
				if ( inputState->guessing==0 ) {
#line 517 "indrilang.g"
					s->addChild(term);
#line 1832 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt197>=1 ) { goto _loop197; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt197++;
		}
		_loop197:;
		}  // ( ... )+
		match(C_ANGLE);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return s ;
}

 indri::lang::ExtentOr*  QueryParser::synonym_list_brace() {
#line 520 "indrilang.g"
	 indri::lang::ExtentOr* s ;
#line 1860 "QueryParser.cpp"
#line 520 "indrilang.g"
	
	indri::lang::RawExtentNode* term = 0;
	s = new indri::lang::ExtentOr;
	_nodes.push_back(s);
	
#line 1867 "QueryParser.cpp"
	
	try {      // for error handling
		match(O_BRACE);
		{ // ( ... )+
		int _cnt200=0;
		for (;;) {
			if ((_tokenSet_3.member(LA(1)))) {
				term=unscoredTerm();
				if ( inputState->guessing==0 ) {
#line 526 "indrilang.g"
					s->addChild(term);
#line 1879 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt200>=1 ) { goto _loop200; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt200++;
		}
		_loop200:;
		}  // ( ... )+
		match(C_BRACE);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return s ;
}

 indri::lang::ExtentOr*  QueryParser::synonym_list_alt() {
#line 529 "indrilang.g"
	 indri::lang::ExtentOr* s ;
#line 1907 "QueryParser.cpp"
#line 529 "indrilang.g"
	
	indri::lang::RawExtentNode* term = 0;
	// semantics of this node will change
	s = new indri::lang::ExtentOr;
	_nodes.push_back(s);
	
#line 1915 "QueryParser.cpp"
	
	try {      // for error handling
		match(SYN);
		match(O_PAREN);
		{ // ( ... )+
		int _cnt203=0;
		for (;;) {
			if ((_tokenSet_3.member(LA(1)))) {
				term=unscoredTerm();
				if ( inputState->guessing==0 ) {
#line 537 "indrilang.g"
					s->addChild(term);
#line 1928 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt203>=1 ) { goto _loop203; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt203++;
		}
		_loop203:;
		}  // ( ... )+
		match(C_PAREN);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return s ;
}

 FieldLessNode*  QueryParser::lessNode() {
#line 742 "indrilang.g"
	 FieldLessNode* ln ;
#line 1956 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 742 "indrilang.g"
	
	ln = 0;
	Field* compareField = 0;
	INT64 high = 0;
	
#line 1964 "QueryParser.cpp"
	
	try {      // for error handling
		match(LESS);
		match(O_PAREN);
		field = LT(1);
		match(TERM);
		high=number();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 747 "indrilang.g"
			
			compareField = new Field(field->getText());
			ln = new FieldLessNode( compareField, high );
			_nodes.push_back( compareField );
			_nodes.push_back( ln );
			
#line 1981 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return ln ;
}

 FieldGreaterNode*  QueryParser::greaterNode() {
#line 730 "indrilang.g"
	 FieldGreaterNode* gn ;
#line 1999 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 730 "indrilang.g"
	
	gn = 0;
	Field* compareField = 0;
	INT64 low = 0;
	
#line 2007 "QueryParser.cpp"
	
	try {      // for error handling
		match(GREATER);
		match(O_PAREN);
		field = LT(1);
		match(TERM);
		low=number();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 735 "indrilang.g"
			
			compareField = new Field(field->getText());
			gn = new FieldGreaterNode( compareField, low );
			_nodes.push_back( compareField );
			_nodes.push_back( gn );
			
#line 2024 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return gn ;
}

 FieldBetweenNode*  QueryParser::betweenNode() {
#line 754 "indrilang.g"
	 FieldBetweenNode* bn ;
#line 2042 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 754 "indrilang.g"
	
	bn = 0;
	Field* compareField = 0;
	INT64 low = 0;
	INT64 high = 0;
	
#line 2051 "QueryParser.cpp"
	
	try {      // for error handling
		match(BETWEEN);
		match(O_PAREN);
		field = LT(1);
		match(TERM);
		low=number();
		high=number();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 760 "indrilang.g"
			
			compareField = new Field(field->getText());
			bn = new FieldBetweenNode( compareField, low, high );
			_nodes.push_back( compareField );
			_nodes.push_back( bn );
			
#line 2069 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return bn ;
}

 FieldEqualsNode*  QueryParser::equalsNode() {
#line 767 "indrilang.g"
	 FieldEqualsNode* en ;
#line 2087 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 767 "indrilang.g"
	
	en = 0;
	Field* compareField = 0;
	INT64 eq = 0;
	
#line 2095 "QueryParser.cpp"
	
	try {      // for error handling
		match(EQUALS);
		match(O_PAREN);
		field = LT(1);
		match(TERM);
		eq=number();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 772 "indrilang.g"
			
			compareField = new Field(field->getText());
			en = new FieldEqualsNode( compareField, eq );
			_nodes.push_back( compareField );
			_nodes.push_back( en );
			
#line 2112 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return en ;
}

 IndexTerm*  QueryParser::rawText() {
#line 675 "indrilang.g"
	 IndexTerm* t ;
#line 2130 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  f = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  et = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  qet = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 675 "indrilang.g"
	
	t = 0;
	
#line 2140 "QueryParser.cpp"
	
	try {      // for error handling
		switch ( LA(1)) {
		case TERM:
		{
			id = LT(1);
			match(TERM);
			if ( inputState->guessing==0 ) {
#line 678 "indrilang.g"
				
				t = new indri::lang::IndexTerm(id->getText());
				_nodes.push_back(t);
				
#line 2154 "QueryParser.cpp"
			}
			break;
		}
		case NUMBER:
		{
			n = LT(1);
			match(NUMBER);
			if ( inputState->guessing==0 ) {
#line 682 "indrilang.g"
				
				t = new indri::lang::IndexTerm(n->getText());
				_nodes.push_back(t);
				
#line 2168 "QueryParser.cpp"
			}
			break;
		}
		case FLOAT:
		{
			f = LT(1);
			match(FLOAT);
			if ( inputState->guessing==0 ) {
#line 686 "indrilang.g"
				
				t = new indri::lang::IndexTerm(f->getText());
				_nodes.push_back(t);
				
#line 2182 "QueryParser.cpp"
			}
			break;
		}
		case DBL_QUOTE:
		{
			match(DBL_QUOTE);
			t=rawText();
			match(DBL_QUOTE);
			if ( inputState->guessing==0 ) {
#line 690 "indrilang.g"
				
				// if a text term appears in quotes, consider it stemmed
				t->setStemmed(true);
				
#line 2197 "QueryParser.cpp"
			}
			break;
		}
		case ENCODED_TERM:
		{
			et = LT(1);
			match(ENCODED_TERM);
			if ( inputState->guessing==0 ) {
#line 694 "indrilang.g"
				
				std::string decodedString; 
				base64_decode_string(decodedString, et->getText());
				t = new indri::lang::IndexTerm( decodedString );
				_nodes.push_back(t);
				
#line 2213 "QueryParser.cpp"
			}
			break;
		}
		case ENCODED_QUOTED_TERM:
		{
			qet = LT(1);
			match(ENCODED_QUOTED_TERM);
			if ( inputState->guessing==0 ) {
#line 700 "indrilang.g"
				
				std::string decodedString; 
				base64_decode_string(decodedString, et->getText());
				t = new indri::lang::IndexTerm( decodedString );
				t->setStemmed(true);
				_nodes.push_back(t);
				
#line 2230 "QueryParser.cpp"
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_5);
		} else {
			throw;
		}
	}
	return t ;
}

 Field*  QueryParser::field_restriction() {
#line 581 "indrilang.g"
	 Field* extent ;
#line 2255 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  fieldName = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	try {      // for error handling
		match(O_SQUARE);
		fieldName = LT(1);
		match(TERM);
		if ( inputState->guessing==0 ) {
#line 583 "indrilang.g"
			
			extent = new Field( fieldName->getText() );
			_nodes.push_back( extent );
			
#line 2268 "QueryParser.cpp"
		}
		match(C_SQUARE);
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_1);
		} else {
			throw;
		}
	}
	return extent ;
}

 UINT64  QueryParser::date() {
#line 641 "indrilang.g"
	 UINT64 d ;
#line 2287 "QueryParser.cpp"
	
	try {      // for error handling
		bool synPredMatched216 = false;
		if (((LA(1) == NUMBER))) {
			int _m216 = mark();
			synPredMatched216 = true;
			inputState->guessing++;
			try {
				{
				match(NUMBER);
				match(SLASH);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched216 = false;
			}
			rewind(_m216);
			inputState->guessing--;
		}
		if ( synPredMatched216 ) {
			d=slashDate();
		}
		else {
			bool synPredMatched218 = false;
			if (((LA(1) == NUMBER))) {
				int _m218 = mark();
				synPredMatched218 = true;
				inputState->guessing++;
				try {
					{
					match(NUMBER);
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched218 = false;
				}
				rewind(_m218);
				inputState->guessing--;
			}
			if ( synPredMatched218 ) {
				d=spaceDate();
			}
			else if ((LA(1) == TERM)) {
				d=dashDate();
			}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_8);
		} else {
			throw;
		}
	}
	return d ;
}

 UINT64  QueryParser::slashDate() {
#line 660 "indrilang.g"
	 UINT64 d ;
#line 2353 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  month = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  day = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  year = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 660 "indrilang.g"
	
	d = 0;
	
#line 2361 "QueryParser.cpp"
	
	try {      // for error handling
		month = LT(1);
		match(NUMBER);
		match(SLASH);
		day = LT(1);
		match(TERM);
		match(SLASH);
		year = LT(1);
		match(NUMBER);
		if ( inputState->guessing==0 ) {
#line 663 "indrilang.g"
			
			d = DateParse::convertDate( year->getText(), month->getText(), day->getText() ); 
			
#line 2377 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_8);
		} else {
			throw;
		}
	}
	return d ;
}

 UINT64  QueryParser::spaceDate() {
#line 667 "indrilang.g"
	 UINT64 d ;
#line 2395 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  day = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  month = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  year = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 667 "indrilang.g"
	
	d = 0;
	
#line 2403 "QueryParser.cpp"
	
	try {      // for error handling
		day = LT(1);
		match(NUMBER);
		month = LT(1);
		match(TERM);
		year = LT(1);
		match(NUMBER);
		if ( inputState->guessing==0 ) {
#line 670 "indrilang.g"
			
			d = DateParse::convertDate( year->getText(), month->getText(), day->getText() );
			
#line 2417 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_8);
		} else {
			throw;
		}
	}
	return d ;
}

 UINT64  QueryParser::dashDate() {
#line 646 "indrilang.g"
	 UINT64 d ;
#line 2435 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  dmy = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 646 "indrilang.g"
	
	d = 0;
	
#line 2441 "QueryParser.cpp"
	
	try {      // for error handling
		dmy = LT(1);
		match(TERM);
		if ( inputState->guessing==0 ) {
#line 649 "indrilang.g"
			
			const std::string& text = dmy->getText();
			int firstDash = text.find('-');
			int secondDash = text.find('-', firstDash+1);
			std::string day = text.substr( 0, firstDash ); 
			std::string month = text.substr( firstDash+1, secondDash-firstDash-1 );
			std::string year = text.substr( secondDash );
			
			d = DateParse::convertDate( year, month, day ); 
			
#line 2458 "QueryParser.cpp"
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_8);
		} else {
			throw;
		}
	}
	return d ;
}

 INT64  QueryParser::number() {
#line 720 "indrilang.g"
	 INT64 v ;
#line 2476 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  nn = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 720 "indrilang.g"
	
	v = 0;
	
#line 2483 "QueryParser.cpp"
	
	try {      // for error handling
		switch ( LA(1)) {
		case NUMBER:
		{
			n = LT(1);
			match(NUMBER);
			if ( inputState->guessing==0 ) {
#line 723 "indrilang.g"
				
				v = string_to_i64(n->getText());
				
#line 2496 "QueryParser.cpp"
			}
			break;
		}
		case NEGATIVE_NUMBER:
		{
			nn = LT(1);
			match(NEGATIVE_NUMBER);
			if ( inputState->guessing==0 ) {
#line 726 "indrilang.g"
				
				v = string_to_i64(nn->getText());
				
#line 2509 "QueryParser.cpp"
			}
			break;
		}
		default:
		{
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
		}
		}
	}
	catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& ex) {
		if( inputState->guessing == 0 ) {
			reportError(ex);
			consume();
			consumeUntil(_tokenSet_9);
		} else {
			throw;
		}
	}
	return v ;
}

void QueryParser::initializeASTFactory( ANTLR_USE_NAMESPACE(antlr)ASTFactory& )
{
}
const char* QueryParser::tokenNames[] = {
	"<0>",
	"EOF",
	"<2>",
	"NULL_TREE_LOOKAHEAD",
	"\"#sum\"",
	"\"#wsum\"",
	"\"#wand\"",
	"\"#od\"",
	"\"#or\"",
	"\"#identsim\"",
	"\"#not\"",
	"\"#uw\"",
	"\"#combine\"",
	"\"#weight\"",
	"\"#max\"",
	"\"#filreq\"",
	"\"#filrej\"",
	"\"#any\"",
	"\"#band\"",
	"\"#syn\"",
	"\"#prior\"",
	"\"#date:after\"",
	"\"#date:before\"",
	"\"#date:between\"",
	"\"#less\"",
	"\"#greater\"",
	"\"#between\"",
	"\"#equals\"",
	"NUMBER",
	"NEGATIVE_NUMBER",
	"FLOAT",
	"STAR",
	"O_PAREN",
	"C_PAREN",
	"O_ANGLE",
	"C_ANGLE",
	"O_SQUARE",
	"C_SQUARE",
	"O_BRACE",
	"C_BRACE",
	"DBL_QUOTE",
	"QUOTE",
	"DOT",
	"COMMA",
	"DASH",
	"COLON",
	"TAB",
	"CR",
	"LF",
	"SPACE",
	"HIGH_CHAR",
	"DIGIT",
	"ASCII_LETTER",
	"SAFE_LETTER",
	"SAFE_CHAR",
	"BASESIXFOUR_CHAR",
	"TEXT_TERM",
	"TERM",
	"ENCODED_QUOTED_TERM",
	"ENCODED_TERM",
	"OPERATOR",
	"JUNK",
	"SLASH",
	0
};

const unsigned long QueryParser::_tokenSet_0_data_[] = { 1610612704UL, 503316804UL, 0UL, 0UL };
// "#wsum" "#wand" "#od" "#or" "#identsim" "#not" "#uw" "#combine" "#weight" 
// "#max" "#filreq" "#filrej" "#any" "#band" "#syn" "#prior" "#date:after" 
// "#date:before" "#date:between" "#less" "#greater" "#between" "#equals" 
// NUMBER FLOAT O_ANGLE O_BRACE DBL_QUOTE TERM ENCODED_QUOTED_TERM ENCODED_TERM 
// OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_0(_tokenSet_0_data_,4);
const unsigned long QueryParser::_tokenSet_1_data_[] = { 2UL, 0UL, 0UL, 0UL };
// EOF 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_1(_tokenSet_1_data_,4);
const unsigned long QueryParser::_tokenSet_2_data_[] = { 1610612706UL, 503316806UL, 0UL, 0UL };
// EOF "#wsum" "#wand" "#od" "#or" "#identsim" "#not" "#uw" "#combine" 
// "#weight" "#max" "#filreq" "#filrej" "#any" "#band" "#syn" "#prior" 
// "#date:after" "#date:before" "#date:between" "#less" "#greater" "#between" 
// "#equals" NUMBER FLOAT C_PAREN O_ANGLE O_BRACE DBL_QUOTE TERM ENCODED_QUOTED_TERM 
// ENCODED_TERM OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_2(_tokenSet_2_data_,4);
const unsigned long QueryParser::_tokenSet_3_data_[] = { 1609435264UL, 503316804UL, 0UL, 0UL };
// "#od" "#uw" "#any" "#band" "#syn" "#date:after" "#date:before" "#date:between" 
// "#less" "#greater" "#between" "#equals" NUMBER FLOAT O_ANGLE O_BRACE 
// DBL_QUOTE TERM ENCODED_QUOTED_TERM ENCODED_TERM OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_3(_tokenSet_3_data_,4);
const unsigned long QueryParser::_tokenSet_4_data_[] = { 1610612706UL, 503316943UL, 0UL, 0UL };
// EOF "#wsum" "#wand" "#od" "#or" "#identsim" "#not" "#uw" "#combine" 
// "#weight" "#max" "#filreq" "#filrej" "#any" "#band" "#syn" "#prior" 
// "#date:after" "#date:before" "#date:between" "#less" "#greater" "#between" 
// "#equals" NUMBER FLOAT O_PAREN C_PAREN O_ANGLE C_ANGLE O_BRACE C_BRACE 
// DBL_QUOTE TERM ENCODED_QUOTED_TERM ENCODED_TERM OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_4(_tokenSet_4_data_,4);
const unsigned long QueryParser::_tokenSet_5_data_[] = { 1610612706UL, 503317966UL, 0UL, 0UL };
// EOF "#wsum" "#wand" "#od" "#or" "#identsim" "#not" "#uw" "#combine" 
// "#weight" "#max" "#filreq" "#filrej" "#any" "#band" "#syn" "#prior" 
// "#date:after" "#date:before" "#date:between" "#less" "#greater" "#between" 
// "#equals" NUMBER FLOAT C_PAREN O_ANGLE C_ANGLE O_BRACE C_BRACE DBL_QUOTE 
// DOT TERM ENCODED_QUOTED_TERM ENCODED_TERM OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_5(_tokenSet_5_data_,4);
const unsigned long QueryParser::_tokenSet_6_data_[] = { 0UL, 1UL, 0UL, 0UL };
// O_PAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_6(_tokenSet_6_data_,4);
const unsigned long QueryParser::_tokenSet_7_data_[] = { 1610612704UL, 503316942UL, 0UL, 0UL };
// "#wsum" "#wand" "#od" "#or" "#identsim" "#not" "#uw" "#combine" "#weight" 
// "#max" "#filreq" "#filrej" "#any" "#band" "#syn" "#prior" "#date:after" 
// "#date:before" "#date:between" "#less" "#greater" "#between" "#equals" 
// NUMBER FLOAT C_PAREN O_ANGLE C_ANGLE O_BRACE C_BRACE DBL_QUOTE TERM 
// ENCODED_QUOTED_TERM ENCODED_TERM OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_7(_tokenSet_7_data_,4);
const unsigned long QueryParser::_tokenSet_8_data_[] = { 0UL, 2050UL, 0UL, 0UL };
// C_PAREN COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_8(_tokenSet_8_data_,4);
const unsigned long QueryParser::_tokenSet_9_data_[] = { 805306368UL, 2UL, 0UL, 0UL };
// NUMBER NEGATIVE_NUMBER C_PAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_9(_tokenSet_9_data_,4);


ANTLR_END_NAMESPACE
ANTLR_END_NAMESPACE
