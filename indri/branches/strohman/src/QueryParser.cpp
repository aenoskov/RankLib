/* $ANTLR 2.7.4: "indrilang.g" -> "QueryParser.cpp"$ */
#include "QueryParser.hpp"
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
#line 198 "indrilang.g"
	 indri::lang::ScoredExtentNode* q ;
#line 37 "QueryParser.cpp"
#line 198 "indrilang.g"
	
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
		case NOT:
		case UW:
		case COMBINE:
		case WEIGHT:
		case MAX:
		case FILREQ:
		case FILREJ:
		case ANY:
		case BAND:
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
		case DBL_QUOTE:
		case TERM:
		case ENCODED_QUOTED_TERM:
		case ENCODED_TERM:
		case OPERATOR:
		{
			s=scoredExtentNode();
			if ( inputState->guessing==0 ) {
#line 205 "indrilang.g"
				
				c = new CombineNode;
				c->addChild(q);
				c->addChild(s);
				_nodes.push_back(c);
				q = c;
				
#line 90 "QueryParser.cpp"
			}
			{ // ( ... )*
			for (;;) {
				if ((_tokenSet_0.member(LA(1)))) {
					s=scoredExtentNode();
					if ( inputState->guessing==0 ) {
#line 213 "indrilang.g"
						
						c->addChild(s);
						
#line 101 "QueryParser.cpp"
					}
				}
				else {
					goto _loop80;
				}
				
			}
			_loop80:;
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
#line 219 "indrilang.g"
	 indri::lang::ScoredExtentNode* s ;
#line 140 "QueryParser.cpp"
	
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
		case OD:
		case UW:
		case FILREQ:
		case FILREJ:
		case ANY:
		case BAND:
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
#line 313 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 230 "QueryParser.cpp"
#line 313 "indrilang.g"
	
	indri::lang::WeightNode* wn = new indri::lang::WeightNode;
	_nodes.push_back(wn);
	
#line 236 "QueryParser.cpp"
	
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
#line 320 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 257 "QueryParser.cpp"
#line 320 "indrilang.g"
	
	indri::lang::CombineNode* cn = new indri::lang::CombineNode;
	_nodes.push_back(cn);
	
#line 263 "QueryParser.cpp"
	
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
#line 348 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 284 "QueryParser.cpp"
#line 348 "indrilang.g"
	
	indri::lang::OrNode* on = new indri::lang::OrNode;
	_nodes.push_back(on);
	
#line 290 "QueryParser.cpp"
	
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

 indri::lang::ScoredExtentNode*  QueryParser::notNode() {
#line 362 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 311 "QueryParser.cpp"
#line 362 "indrilang.g"
	
	indri::lang::NotNode* n = new indri::lang::NotNode;
	indri::lang::ScoredExtentNode* c = 0;
	_nodes.push_back(n);
	r = n;
	
#line 319 "QueryParser.cpp"
	
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
#line 370 "indrilang.g"
			
			n->setChild(c);
			
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

 indri::lang::ScoredExtentNode*  QueryParser::wandNode() {
#line 341 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 366 "QueryParser.cpp"
#line 341 "indrilang.g"
	
	indri::lang::WAndNode* wn = new indri::lang::WAndNode;
	_nodes.push_back(wn);
	
#line 372 "QueryParser.cpp"
	
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
#line 334 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 393 "QueryParser.cpp"
#line 334 "indrilang.g"
	
	indri::lang::WSumNode* wn = new indri::lang::WSumNode;
	_nodes.push_back(wn);
	
#line 399 "QueryParser.cpp"
	
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
#line 355 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 420 "QueryParser.cpp"
#line 355 "indrilang.g"
	
	indri::lang::MaxNode* mn = new indri::lang::MaxNode;
	_nodes.push_back(mn);
	
#line 426 "QueryParser.cpp"
	
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
#line 374 "indrilang.g"
	 indri::lang::PriorNode* p ;
#line 447 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  name = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 374 "indrilang.g"
	
	indri::lang::Field* field = 0;
	p = 0;
	
#line 454 "QueryParser.cpp"
	
	try {      // for error handling
		match(PRIOR);
		match(O_PAREN);
		name = LT(1);
		match(TERM);
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 379 "indrilang.g"
			
			p = _priorFactory->create( name->getText() );
			
			if( p->getFieldName().length() ) {
			field = new Field( p->getFieldName() );
			p->setField( field );
			_nodes.push_back(field);
			}
			
			_nodes.push_back(p);
			
#line 475 "QueryParser.cpp"
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

 ScoredExtentNode*  QueryParser::scoredRaw() {
#line 236 "indrilang.g"
	 ScoredExtentNode* sn ;
#line 493 "QueryParser.cpp"
#line 236 "indrilang.g"
	
	RawExtentNode* raw = 0;
	RawExtentNode* contexts = 0;
	sn = 0;
	
#line 500 "QueryParser.cpp"
	
	try {      // for error handling
		bool synPredMatched100 = false;
		if (((_tokenSet_3.member(LA(1))))) {
			int _m100 = mark();
			synPredMatched100 = true;
			inputState->guessing++;
			try {
				{
				qualifiedTerm();
				context_list();
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched100 = false;
			}
			rewind(_m100);
			inputState->guessing--;
		}
		if ( synPredMatched100 ) {
			raw=qualifiedTerm();
			contexts=context_list();
			if ( inputState->guessing==0 ) {
#line 243 "indrilang.g"
				
				sn = new indri::lang::RawScorerNode( raw, contexts );
				_nodes.push_back(sn);
				
#line 529 "QueryParser.cpp"
			}
		}
		else {
			bool synPredMatched102 = false;
			if (((_tokenSet_3.member(LA(1))))) {
				int _m102 = mark();
				synPredMatched102 = true;
				inputState->guessing++;
				try {
					{
					qualifiedTerm();
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched102 = false;
				}
				rewind(_m102);
				inputState->guessing--;
			}
			if ( synPredMatched102 ) {
				raw=qualifiedTerm();
				if ( inputState->guessing==0 ) {
#line 248 "indrilang.g"
					
					sn = new indri::lang::RawScorerNode( raw, contexts );
					_nodes.push_back(sn);
					
#line 557 "QueryParser.cpp"
				}
			}
			else {
				bool synPredMatched104 = false;
				if (((_tokenSet_3.member(LA(1))))) {
					int _m104 = mark();
					synPredMatched104 = true;
					inputState->guessing++;
					try {
						{
						unqualifiedTerm();
						match(DOT);
						}
					}
					catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
						synPredMatched104 = false;
					}
					rewind(_m104);
					inputState->guessing--;
				}
				if ( synPredMatched104 ) {
					raw=unqualifiedTerm();
					match(DOT);
					contexts=context_list();
					if ( inputState->guessing==0 ) {
#line 253 "indrilang.g"
						
						sn = new indri::lang::RawScorerNode( raw, contexts );
						_nodes.push_back(sn);
						
#line 588 "QueryParser.cpp"
					}
				}
				else if ((_tokenSet_3.member(LA(1)))) {
					raw=unqualifiedTerm();
					if ( inputState->guessing==0 ) {
#line 258 "indrilang.g"
						
						sn = new indri::lang::RawScorerNode( raw, contexts );
						_nodes.push_back(sn);
						
#line 599 "QueryParser.cpp"
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
#line 488 "indrilang.g"
	 RawExtentNode* t ;
#line 622 "QueryParser.cpp"
#line 488 "indrilang.g"
	
	RawExtentNode* synonyms = 0;
	RawExtentNode* fields = 0;
	t = 0;
	
#line 629 "QueryParser.cpp"
	
	try {      // for error handling
		synonyms=unqualifiedTerm();
		match(DOT);
		fields=field_list();
		if ( inputState->guessing==0 ) {
#line 495 "indrilang.g"
			
			if( fields ) {
			t = new indri::lang::ExtentInside( synonyms, fields );
			_nodes.push_back(t);
			} else {
			t = synonyms;
			}
			
#line 645 "QueryParser.cpp"
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
#line 574 "indrilang.g"
	 ExtentOr* contexts ;
#line 663 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  first = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  additional = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 574 "indrilang.g"
	
	contexts = new ExtentOr;
	_nodes.push_back( contexts );
	
#line 671 "QueryParser.cpp"
	
	try {      // for error handling
		match(O_PAREN);
		first = LT(1);
		match(TERM);
		if ( inputState->guessing==0 ) {
#line 580 "indrilang.g"
			
			Field* firstField = new indri::lang::Field( first->getText() );
			_nodes.push_back( firstField );
			contexts->addChild( firstField );
			
#line 684 "QueryParser.cpp"
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				additional = LT(1);
				match(TERM);
				if ( inputState->guessing==0 ) {
#line 587 "indrilang.g"
					
					Field* additionalField = new Field(additional->getText());
					_nodes.push_back( additionalField );
					contexts->addChild( additionalField );
					
#line 699 "QueryParser.cpp"
				}
			}
			else {
				goto _loop196;
			}
			
		}
		_loop196:;
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
#line 504 "indrilang.g"
	 RawExtentNode* re ;
#line 726 "QueryParser.cpp"
	
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
		case FILREJ:
		{
			re=filrejNode();
			break;
		}
		case FILREQ:
		{
			re=filreqNode();
			break;
		}
		case O_ANGLE:
		{
			re=synonym_list();
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
#line 274 "indrilang.g"
	 indri::lang::ScoredExtentNode* sr ;
#line 834 "QueryParser.cpp"
#line 274 "indrilang.g"
	
	double w = 0;
	ScoredExtentNode* n = 0;
	sr = wn;
	
#line 841 "QueryParser.cpp"
	
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
		int _cnt108=0;
		for (;;) {
			if ((LA(1) == NUMBER || LA(1) == FLOAT)) {
				w=floating();
				n=scoredExtentNode();
				if ( inputState->guessing==0 ) {
#line 285 "indrilang.g"
					wn->addChild( w, n );
#line 871 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt108>=1 ) { goto _loop108; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt108++;
		}
		_loop108:;
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

 indri::lang::ScoredExtentNode*  QueryParser::extentRestriction(
	 indri::lang::ScoredExtentNode* sn 
) {
#line 521 "indrilang.g"
	 indri::lang::ScoredExtentNode* er ;
#line 901 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  passageWindowSize = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  inc = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 521 "indrilang.g"
	
	indri::lang::Field* f = 0;
	er = 0;
	
#line 910 "QueryParser.cpp"
	
	try {      // for error handling
		bool synPredMatched187 = false;
		if (((LA(1) == O_SQUARE))) {
			int _m187 = mark();
			synPredMatched187 = true;
			inputState->guessing++;
			try {
				{
				match(O_SQUARE);
				match(TERM);
				match(COLON);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched187 = false;
			}
			rewind(_m187);
			inputState->guessing--;
		}
		if ( synPredMatched187 ) {
			match(O_SQUARE);
			passageWindowSize = LT(1);
			match(TERM);
			match(COLON);
			inc = LT(1);
			match(NUMBER);
			match(C_SQUARE);
			if ( inputState->guessing==0 ) {
#line 526 "indrilang.g"
				
				for( int startWindow = 0; startWindow < passageWindowSize->getText().size(); startWindow++ ) {
				if( isdigit( passageWindowSize->getText()[startWindow] ) )
				break;
				}
				
				int increment = atoi(inc->getText().c_str());
				int windowSize = atoi(passageWindowSize->getText().c_str() + startWindow );
				
				er = new indri::lang::FixedWindow(sn, windowSize, increment);
				
#line 952 "QueryParser.cpp"
			}
		}
		else if ((LA(1) == O_SQUARE)) {
			match(O_SQUARE);
			match(LITERAL_passage);
			field = LT(1);
			match(TERM);
			match(C_SQUARE);
			if ( inputState->guessing==0 ) {
#line 538 "indrilang.g"
				
				f = new indri::lang::Field(field->getText());
				_nodes.push_back(f);
				er = new indri::lang::ExtentRestriction(sn, f);
				_nodes.push_back(er);
				
#line 969 "QueryParser.cpp"
			}
		}
		else {
			throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
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
#line 724 "indrilang.g"
	 double d ;
#line 992 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  f = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 724 "indrilang.g"
	
	d = 0;
	
#line 999 "QueryParser.cpp"
	
	try {      // for error handling
		switch ( LA(1)) {
		case FLOAT:
		{
			f = LT(1);
			match(FLOAT);
			if ( inputState->guessing==0 ) {
#line 727 "indrilang.g"
				
				d = atof(f->getText().c_str());
				
#line 1012 "QueryParser.cpp"
			}
			break;
		}
		case NUMBER:
		{
			n = LT(1);
			match(NUMBER);
			if ( inputState->guessing==0 ) {
#line 730 "indrilang.g"
				
				d = atof(n->getText().c_str());
				
#line 1025 "QueryParser.cpp"
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
#line 290 "indrilang.g"
	 indri::lang::ScoredExtentNode* sr ;
#line 1052 "QueryParser.cpp"
#line 290 "indrilang.g"
	
	double w = 0;
	ScoredExtentNode* n = 0;
	sr = wn;
	
#line 1059 "QueryParser.cpp"
	
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
		int _cnt112=0;
		for (;;) {
			if ((_tokenSet_0.member(LA(1)))) {
				n=scoredExtentNode();
				if ( inputState->guessing==0 ) {
#line 298 "indrilang.g"
					wn->addChild( 1.0, n );
#line 1088 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt112>=1 ) { goto _loop112; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt112++;
		}
		_loop112:;
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
#line 302 "indrilang.g"
	 indri::lang::ScoredExtentNode* sr ;
#line 1118 "QueryParser.cpp"
#line 302 "indrilang.g"
	
	ScoredExtentNode* n = 0;
	sr = cn;
	
#line 1124 "QueryParser.cpp"
	
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
		int _cnt116=0;
		for (;;) {
			if ((_tokenSet_0.member(LA(1)))) {
				n=scoredExtentNode();
				if ( inputState->guessing==0 ) {
#line 309 "indrilang.g"
					cn->addChild( n );
#line 1153 "QueryParser.cpp"
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

 indri::lang::ScoredExtentNode*  QueryParser::sumNode() {
#line 327 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 1181 "QueryParser.cpp"
#line 327 "indrilang.g"
	
	indri::lang::WSumNode* wn = new indri::lang::WSumNode;
	_nodes.push_back(wn);
	
#line 1187 "QueryParser.cpp"
	
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
#line 401 "indrilang.g"
	 indri::lang::ODNode* od ;
#line 1208 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  n1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  n2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 401 "indrilang.g"
	
	RawExtentNode* rn = 0;
	od = new indri::lang::ODNode;
	_nodes.push_back(od);
	
#line 1217 "QueryParser.cpp"
	
	try {      // for error handling
		{
		bool synPredMatched130 = false;
		if (((LA(1) == OD))) {
			int _m130 = mark();
			synPredMatched130 = true;
			inputState->guessing++;
			try {
				{
				match(OD);
				match(NUMBER);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched130 = false;
			}
			rewind(_m130);
			inputState->guessing--;
		}
		if ( synPredMatched130 ) {
			{
			match(OD);
			n1 = LT(1);
			match(NUMBER);
			if ( inputState->guessing==0 ) {
#line 410 "indrilang.g"
				od->setWindowSize( n1->getText() );
#line 1246 "QueryParser.cpp"
			}
			}
		}
		else {
			bool synPredMatched133 = false;
			if (((LA(1) == OD))) {
				int _m133 = mark();
				synPredMatched133 = true;
				inputState->guessing++;
				try {
					{
					match(OD);
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched133 = false;
				}
				rewind(_m133);
				inputState->guessing--;
			}
			if ( synPredMatched133 ) {
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
#line 414 "indrilang.g"
					od->setWindowSize( n2->getText() );
#line 1280 "QueryParser.cpp"
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
		int _cnt137=0;
		for (;;) {
			if ((_tokenSet_3.member(LA(1)))) {
				rn=unscoredTerm();
				if ( inputState->guessing==0 ) {
#line 418 "indrilang.g"
					od->addChild( rn );
#line 1298 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt137>=1 ) { goto _loop137; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt137++;
		}
		_loop137:;
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
#line 481 "indrilang.g"
	 RawExtentNode* t ;
#line 1326 "QueryParser.cpp"
#line 481 "indrilang.g"
	
	t = 0;
	
#line 1331 "QueryParser.cpp"
	
	try {      // for error handling
		bool synPredMatched154 = false;
		if (((_tokenSet_3.member(LA(1))))) {
			int _m154 = mark();
			synPredMatched154 = true;
			inputState->guessing++;
			try {
				{
				qualifiedTerm();
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched154 = false;
			}
			rewind(_m154);
			inputState->guessing--;
		}
		if ( synPredMatched154 ) {
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
#line 422 "indrilang.g"
	 indri::lang::UWNode* uw ;
#line 1376 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 422 "indrilang.g"
	
	uw = new indri::lang::UWNode;
	RawExtentNode* rn = 0;
	_nodes.push_back(uw);
	
#line 1384 "QueryParser.cpp"
	
	try {      // for error handling
		{
		bool synPredMatched141 = false;
		if (((LA(1) == UW))) {
			int _m141 = mark();
			synPredMatched141 = true;
			inputState->guessing++;
			try {
				{
				match(UW);
				match(NUMBER);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched141 = false;
			}
			rewind(_m141);
			inputState->guessing--;
		}
		if ( synPredMatched141 ) {
			{
			match(UW);
			n = LT(1);
			match(NUMBER);
			if ( inputState->guessing==0 ) {
#line 430 "indrilang.g"
				uw->setWindowSize( n->getText() );
#line 1413 "QueryParser.cpp"
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
		int _cnt145=0;
		for (;;) {
			if ((_tokenSet_3.member(LA(1)))) {
				rn=unscoredTerm();
				if ( inputState->guessing==0 ) {
#line 436 "indrilang.g"
					uw->addChild( rn );
#line 1436 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt145>=1 ) { goto _loop145; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt145++;
		}
		_loop145:;
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
#line 439 "indrilang.g"
	 indri::lang::BAndNode* b ;
#line 1464 "QueryParser.cpp"
#line 439 "indrilang.g"
	
	b = new indri::lang::BAndNode;
	RawExtentNode* rn = 0;
	_nodes.push_back(b);
	
#line 1471 "QueryParser.cpp"
	
	try {      // for error handling
		match(BAND);
		match(O_PAREN);
		{ // ( ... )+
		int _cnt148=0;
		for (;;) {
			if ((_tokenSet_3.member(LA(1)))) {
				rn=unscoredTerm();
				if ( inputState->guessing==0 ) {
#line 447 "indrilang.g"
					b->addChild( rn );
#line 1484 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt148>=1 ) { goto _loop148; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt148++;
		}
		_loop148:;
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

 indri::lang::FilRejNode*  QueryParser::filrejNode() {
#line 450 "indrilang.g"
	 indri::lang::FilRejNode* fj ;
#line 1512 "QueryParser.cpp"
#line 450 "indrilang.g"
	
	RawExtentNode* filtered = 0;
	RawExtentNode* disallowed = 0;
	
#line 1518 "QueryParser.cpp"
	
	try {      // for error handling
		match(FILREJ);
		match(O_PAREN);
		filtered=unscoredTerm();
		disallowed=unscoredTerm();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 456 "indrilang.g"
			
			fj = new FilRejNode( filtered, disallowed );
			_nodes.push_back(fj);
			
#line 1532 "QueryParser.cpp"
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
	return fj ;
}

 indri::lang::FilReqNode*  QueryParser::filreqNode() {
#line 461 "indrilang.g"
	 indri::lang::FilReqNode* fq ;
#line 1550 "QueryParser.cpp"
#line 461 "indrilang.g"
	
	RawExtentNode* filtered = 0;
	RawExtentNode* required = 0;
	
#line 1556 "QueryParser.cpp"
	
	try {      // for error handling
		match(FILREQ);
		match(O_PAREN);
		filtered=unscoredTerm();
		required=unscoredTerm();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 467 "indrilang.g"
			
			fq = new FilReqNode( filtered, required );
			_nodes.push_back(fq);
			
#line 1570 "QueryParser.cpp"
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
	return fq ;
}

 indri::lang::Field*  QueryParser::anyField() {
#line 472 "indrilang.g"
	 indri::lang::Field* f ;
#line 1588 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 472 "indrilang.g"
	
	f = 0;
	
#line 1594 "QueryParser.cpp"
	
	try {      // for error handling
		match(ANY);
		match(COLON);
		t = LT(1);
		match(TERM);
		if ( inputState->guessing==0 ) {
#line 476 "indrilang.g"
			
			f = new Field(t->getText());
			_nodes.push_back(f);
			
#line 1607 "QueryParser.cpp"
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
#line 554 "indrilang.g"
	 ExtentAnd* fields ;
#line 1625 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  first = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  additional = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 554 "indrilang.g"
	
	fields = new ExtentAnd;
	_nodes.push_back( fields );
	
#line 1633 "QueryParser.cpp"
	
	try {      // for error handling
		first = LT(1);
		match(TERM);
		if ( inputState->guessing==0 ) {
#line 560 "indrilang.g"
			
			Field* firstField = new indri::lang::Field( first->getText() );
			_nodes.push_back( firstField );
			fields->addChild( firstField );
			
#line 1645 "QueryParser.cpp"
		}
		{ // ( ... )*
		for (;;) {
			if ((LA(1) == COMMA)) {
				match(COMMA);
				additional = LT(1);
				match(TERM);
				if ( inputState->guessing==0 ) {
#line 567 "indrilang.g"
					
					Field* additionalField = new Field(additional->getText());
					_nodes.push_back( additionalField );
					fields->addChild( additionalField );
					
#line 1660 "QueryParser.cpp"
				}
			}
			else {
				goto _loop193;
			}
			
		}
		_loop193:;
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
#line 603 "indrilang.g"
	 FieldLessNode* extent ;
#line 1686 "QueryParser.cpp"
#line 603 "indrilang.g"
	
	UINT64 d = 0;
	Field* dateField = 0;
	extent = 0;
	
#line 1693 "QueryParser.cpp"
	
	try {      // for error handling
		match(DATEBEFORE);
		match(O_PAREN);
		d=date();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 608 "indrilang.g"
			
			dateField = new Field("date");
			extent = new FieldLessNode( dateField, d );
			_nodes.push_back( dateField );
			_nodes.push_back( extent );
			
#line 1708 "QueryParser.cpp"
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
#line 615 "indrilang.g"
	 FieldGreaterNode* extent ;
#line 1726 "QueryParser.cpp"
#line 615 "indrilang.g"
	
	UINT64 d = 0;
	Field* dateField = 0;
	extent = 0;
	
#line 1733 "QueryParser.cpp"
	
	try {      // for error handling
		match(DATEAFTER);
		match(O_PAREN);
		d=date();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 620 "indrilang.g"
			
			dateField = new Field("date");
			extent = new FieldGreaterNode( dateField, d );
			_nodes.push_back( dateField );
			_nodes.push_back( extent );
			
#line 1748 "QueryParser.cpp"
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
#line 627 "indrilang.g"
	 FieldBetweenNode* extent ;
#line 1766 "QueryParser.cpp"
#line 627 "indrilang.g"
	
	UINT64 low = 0;
	UINT64 high = 0;
	Field* dateField = 0;
	extent = 0;
	
#line 1774 "QueryParser.cpp"
	
	try {      // for error handling
		match(DATEBETWEEN);
		match(O_PAREN);
		low=date();
		match(COMMA);
		high=date();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 633 "indrilang.g"
			
			dateField = new Field("date");
			extent = new FieldBetweenNode( dateField, low, high );
			_nodes.push_back( dateField );
			_nodes.push_back( extent );
			
#line 1791 "QueryParser.cpp"
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
#line 545 "indrilang.g"
	 indri::lang::ExtentOr* s ;
#line 1809 "QueryParser.cpp"
#line 545 "indrilang.g"
	
	indri::lang::RawExtentNode* term = 0;
	s = new indri::lang::ExtentOr;
	_nodes.push_back(s);
	
#line 1816 "QueryParser.cpp"
	
	try {      // for error handling
		match(O_ANGLE);
		{ // ( ... )+
		int _cnt190=0;
		for (;;) {
			if ((_tokenSet_3.member(LA(1)))) {
				term=unscoredTerm();
				if ( inputState->guessing==0 ) {
#line 551 "indrilang.g"
					s->addChild(term);
#line 1828 "QueryParser.cpp"
				}
			}
			else {
				if ( _cnt190>=1 ) { goto _loop190; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
			}
			
			_cnt190++;
		}
		_loop190:;
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

 FieldLessNode*  QueryParser::lessNode() {
#line 756 "indrilang.g"
	 FieldLessNode* ln ;
#line 1856 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 756 "indrilang.g"
	
	ln = 0;
	Field* compareField = 0;
	INT64 high = 0;
	
#line 1864 "QueryParser.cpp"
	
	try {      // for error handling
		match(LESS);
		match(O_PAREN);
		field = LT(1);
		match(TERM);
		high=number();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 761 "indrilang.g"
			
			compareField = new Field(field->getText());
			ln = new FieldLessNode( compareField, high );
			_nodes.push_back( compareField );
			_nodes.push_back( ln );
			
#line 1881 "QueryParser.cpp"
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
#line 744 "indrilang.g"
	 FieldGreaterNode* gn ;
#line 1899 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 744 "indrilang.g"
	
	gn = 0;
	Field* compareField = 0;
	INT64 low = 0;
	
#line 1907 "QueryParser.cpp"
	
	try {      // for error handling
		match(GREATER);
		match(O_PAREN);
		field = LT(1);
		match(TERM);
		low=number();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 749 "indrilang.g"
			
			compareField = new Field(field->getText());
			gn = new FieldGreaterNode( compareField, low );
			_nodes.push_back( compareField );
			_nodes.push_back( gn );
			
#line 1924 "QueryParser.cpp"
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
#line 768 "indrilang.g"
	 FieldBetweenNode* bn ;
#line 1942 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 768 "indrilang.g"
	
	bn = 0;
	Field* compareField = 0;
	INT64 low = 0;
	INT64 high = 0;
	
#line 1951 "QueryParser.cpp"
	
	try {      // for error handling
		match(BETWEEN);
		match(O_PAREN);
		field = LT(1);
		match(TERM);
		low=number();
		high=number();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 774 "indrilang.g"
			
			compareField = new Field(field->getText());
			bn = new FieldBetweenNode( compareField, low, high );
			_nodes.push_back( compareField );
			_nodes.push_back( bn );
			
#line 1969 "QueryParser.cpp"
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
#line 781 "indrilang.g"
	 FieldEqualsNode* en ;
#line 1987 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 781 "indrilang.g"
	
	en = 0;
	Field* compareField = 0;
	INT64 eq = 0;
	
#line 1995 "QueryParser.cpp"
	
	try {      // for error handling
		match(EQUALS);
		match(O_PAREN);
		field = LT(1);
		match(TERM);
		eq=number();
		match(C_PAREN);
		if ( inputState->guessing==0 ) {
#line 786 "indrilang.g"
			
			compareField = new Field(field->getText());
			en = new FieldEqualsNode( compareField, eq );
			_nodes.push_back( compareField );
			_nodes.push_back( en );
			
#line 2012 "QueryParser.cpp"
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
#line 689 "indrilang.g"
	 IndexTerm* t ;
#line 2030 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  f = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  et = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  qet = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 689 "indrilang.g"
	
	t = 0;
	
#line 2040 "QueryParser.cpp"
	
	try {      // for error handling
		switch ( LA(1)) {
		case TERM:
		{
			id = LT(1);
			match(TERM);
			if ( inputState->guessing==0 ) {
#line 692 "indrilang.g"
				
				t = new indri::lang::IndexTerm(id->getText());
				_nodes.push_back(t);
				
#line 2054 "QueryParser.cpp"
			}
			break;
		}
		case NUMBER:
		{
			n = LT(1);
			match(NUMBER);
			if ( inputState->guessing==0 ) {
#line 696 "indrilang.g"
				
				t = new indri::lang::IndexTerm(n->getText());
				_nodes.push_back(t);
				
#line 2068 "QueryParser.cpp"
			}
			break;
		}
		case FLOAT:
		{
			f = LT(1);
			match(FLOAT);
			if ( inputState->guessing==0 ) {
#line 700 "indrilang.g"
				
				t = new indri::lang::IndexTerm(f->getText());
				_nodes.push_back(t);
				
#line 2082 "QueryParser.cpp"
			}
			break;
		}
		case DBL_QUOTE:
		{
			match(DBL_QUOTE);
			t=rawText();
			match(DBL_QUOTE);
			if ( inputState->guessing==0 ) {
#line 704 "indrilang.g"
				
				// if a text term appears in quotes, consider it stemmed
				t->setStemmed(true);
				
#line 2097 "QueryParser.cpp"
			}
			break;
		}
		case ENCODED_TERM:
		{
			et = LT(1);
			match(ENCODED_TERM);
			if ( inputState->guessing==0 ) {
#line 708 "indrilang.g"
				
				std::string decodedString; 
				base64_decode_string(decodedString, et->getText());
				t = new indri::lang::IndexTerm( decodedString );
				_nodes.push_back(t);
				
#line 2113 "QueryParser.cpp"
			}
			break;
		}
		case ENCODED_QUOTED_TERM:
		{
			qet = LT(1);
			match(ENCODED_QUOTED_TERM);
			if ( inputState->guessing==0 ) {
#line 714 "indrilang.g"
				
				std::string decodedString; 
				base64_decode_string(decodedString, et->getText());
				t = new indri::lang::IndexTerm( decodedString );
				t->setStemmed(true);
				_nodes.push_back(t);
				
#line 2130 "QueryParser.cpp"
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
#line 595 "indrilang.g"
	 Field* extent ;
#line 2155 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  fieldName = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	try {      // for error handling
		match(O_SQUARE);
		fieldName = LT(1);
		match(TERM);
		if ( inputState->guessing==0 ) {
#line 597 "indrilang.g"
			
			extent = new Field( fieldName->getText() );
			_nodes.push_back( extent );
			
#line 2168 "QueryParser.cpp"
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
#line 655 "indrilang.g"
	 UINT64 d ;
#line 2187 "QueryParser.cpp"
	
	try {      // for error handling
		bool synPredMatched203 = false;
		if (((LA(1) == NUMBER))) {
			int _m203 = mark();
			synPredMatched203 = true;
			inputState->guessing++;
			try {
				{
				match(NUMBER);
				match(SLASH);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched203 = false;
			}
			rewind(_m203);
			inputState->guessing--;
		}
		if ( synPredMatched203 ) {
			d=slashDate();
		}
		else {
			bool synPredMatched205 = false;
			if (((LA(1) == NUMBER))) {
				int _m205 = mark();
				synPredMatched205 = true;
				inputState->guessing++;
				try {
					{
					match(NUMBER);
					}
				}
				catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
					synPredMatched205 = false;
				}
				rewind(_m205);
				inputState->guessing--;
			}
			if ( synPredMatched205 ) {
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
#line 674 "indrilang.g"
	 UINT64 d ;
#line 2253 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  month = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  day = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  year = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 674 "indrilang.g"
	
	d = 0;
	
#line 2261 "QueryParser.cpp"
	
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
#line 677 "indrilang.g"
			
			d = DateParse::convertDate( year->getText(), month->getText(), day->getText() ); 
			
#line 2277 "QueryParser.cpp"
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
#line 681 "indrilang.g"
	 UINT64 d ;
#line 2295 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  day = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  month = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  year = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 681 "indrilang.g"
	
	d = 0;
	
#line 2303 "QueryParser.cpp"
	
	try {      // for error handling
		day = LT(1);
		match(NUMBER);
		month = LT(1);
		match(TERM);
		year = LT(1);
		match(NUMBER);
		if ( inputState->guessing==0 ) {
#line 684 "indrilang.g"
			
			d = DateParse::convertDate( year->getText(), month->getText(), day->getText() );
			
#line 2317 "QueryParser.cpp"
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
#line 660 "indrilang.g"
	 UINT64 d ;
#line 2335 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  dmy = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 660 "indrilang.g"
	
	d = 0;
	
#line 2341 "QueryParser.cpp"
	
	try {      // for error handling
		dmy = LT(1);
		match(TERM);
		if ( inputState->guessing==0 ) {
#line 663 "indrilang.g"
			
			const std::string& text = dmy->getText();
			int firstDash = text.find('-');
			int secondDash = text.find('-', firstDash+1);
			std::string day = text.substr( 0, firstDash ); 
			std::string month = text.substr( firstDash+1, secondDash-firstDash-1 );
			std::string year = text.substr( secondDash );
			
			d = DateParse::convertDate( year, month, day ); 
			
#line 2358 "QueryParser.cpp"
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
#line 734 "indrilang.g"
	 INT64 v ;
#line 2376 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  nn = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 734 "indrilang.g"
	
	v = 0;
	
#line 2383 "QueryParser.cpp"
	
	try {      // for error handling
		switch ( LA(1)) {
		case NUMBER:
		{
			n = LT(1);
			match(NUMBER);
			if ( inputState->guessing==0 ) {
#line 737 "indrilang.g"
				
				v = string_to_i64(n->getText());
				
#line 2396 "QueryParser.cpp"
			}
			break;
		}
		case NEGATIVE_NUMBER:
		{
			nn = LT(1);
			match(NEGATIVE_NUMBER);
			if ( inputState->guessing==0 ) {
#line 740 "indrilang.g"
				
				v = string_to_i64(nn->getText());
				
#line 2409 "QueryParser.cpp"
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
	"\"#not\"",
	"\"#uw\"",
	"\"#combine\"",
	"\"#weight\"",
	"\"#max\"",
	"\"#filreq\"",
	"\"#filrej\"",
	"\"#any\"",
	"\"#band\"",
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
	"\"passage\"",
	"SLASH",
	0
};

const unsigned long QueryParser::_tokenSet_0_data_[] = { 402653152UL, 31457297UL, 0UL, 0UL };
// "#wsum" "#wand" "#od" "#or" "#not" "#uw" "#combine" "#weight" "#max" 
// "#filreq" "#filrej" "#any" "#band" "#prior" "#date:after" "#date:before" 
// "#date:between" "#less" "#greater" "#between" "#equals" NUMBER FLOAT 
// O_ANGLE DBL_QUOTE TERM ENCODED_QUOTED_TERM ENCODED_TERM OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_0(_tokenSet_0_data_,4);
const unsigned long QueryParser::_tokenSet_1_data_[] = { 2UL, 0UL, 0UL, 0UL };
// EOF 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_1(_tokenSet_1_data_,4);
const unsigned long QueryParser::_tokenSet_2_data_[] = { 2550136802UL, 31457297UL, 0UL, 0UL };
// EOF "#wsum" "#wand" "#od" "#or" "#not" "#uw" "#combine" "#weight" "#max" 
// "#filreq" "#filrej" "#any" "#band" "#prior" "#date:after" "#date:before" 
// "#date:between" "#less" "#greater" "#between" "#equals" NUMBER FLOAT 
// C_PAREN O_ANGLE DBL_QUOTE TERM ENCODED_QUOTED_TERM ENCODED_TERM OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_2(_tokenSet_2_data_,4);
const unsigned long QueryParser::_tokenSet_3_data_[] = { 402375808UL, 31457297UL, 0UL, 0UL };
// "#od" "#uw" "#filreq" "#filrej" "#any" "#band" "#date:after" "#date:before" 
// "#date:between" "#less" "#greater" "#between" "#equals" NUMBER FLOAT 
// O_ANGLE DBL_QUOTE TERM ENCODED_QUOTED_TERM ENCODED_TERM OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_3(_tokenSet_3_data_,4);
const unsigned long QueryParser::_tokenSet_4_data_[] = { 3623878626UL, 31457299UL, 0UL, 0UL };
// EOF "#wsum" "#wand" "#od" "#or" "#not" "#uw" "#combine" "#weight" "#max" 
// "#filreq" "#filrej" "#any" "#band" "#prior" "#date:after" "#date:before" 
// "#date:between" "#less" "#greater" "#between" "#equals" NUMBER FLOAT 
// O_PAREN C_PAREN O_ANGLE C_ANGLE DBL_QUOTE TERM ENCODED_QUOTED_TERM ENCODED_TERM 
// OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_4(_tokenSet_4_data_,4);
const unsigned long QueryParser::_tokenSet_5_data_[] = { 2550136802UL, 31457363UL, 0UL, 0UL };
// EOF "#wsum" "#wand" "#od" "#or" "#not" "#uw" "#combine" "#weight" "#max" 
// "#filreq" "#filrej" "#any" "#band" "#prior" "#date:after" "#date:before" 
// "#date:between" "#less" "#greater" "#between" "#equals" NUMBER FLOAT 
// C_PAREN O_ANGLE C_ANGLE DBL_QUOTE DOT TERM ENCODED_QUOTED_TERM ENCODED_TERM 
// OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_5(_tokenSet_5_data_,4);
const unsigned long QueryParser::_tokenSet_6_data_[] = { 1073741824UL, 0UL, 0UL, 0UL };
// O_PAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_6(_tokenSet_6_data_,4);
const unsigned long QueryParser::_tokenSet_7_data_[] = { 2549859456UL, 31457299UL, 0UL, 0UL };
// "#od" "#uw" "#filreq" "#filrej" "#any" "#band" "#date:after" "#date:before" 
// "#date:between" "#less" "#greater" "#between" "#equals" NUMBER FLOAT 
// C_PAREN O_ANGLE C_ANGLE DBL_QUOTE TERM ENCODED_QUOTED_TERM ENCODED_TERM 
// OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_7(_tokenSet_7_data_,4);
const unsigned long QueryParser::_tokenSet_8_data_[] = { 2147483648UL, 128UL, 0UL, 0UL };
// C_PAREN COMMA 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_8(_tokenSet_8_data_,4);
const unsigned long QueryParser::_tokenSet_9_data_[] = { 2348810240UL, 0UL, 0UL, 0UL };
// NUMBER NEGATIVE_NUMBER C_PAREN 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_9(_tokenSet_9_data_,4);


ANTLR_END_NAMESPACE
ANTLR_END_NAMESPACE
