/* $ANTLR 2.7.5 (20050128): "indrilang.g" -> "QueryParser.cpp"$ */
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
#line 168 "indrilang.g"
	 indri::lang::ScoredExtentNode* q ;
#line 37 "QueryParser.cpp"
#line 168 "indrilang.g"
	
	indri::lang::CombineNode* c = 0;
	indri::lang::ScoredExtentNode* s = 0;
	q = 0;
	
#line 44 "QueryParser.cpp"
	
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
#line 175 "indrilang.g"
			
			c = new CombineNode;
			c->addChild(q);
			c->addChild(s);
			_nodes.push_back(c);
			q = c;
			
#line 92 "QueryParser.cpp"
		}
		{ // ( ... )*
		for (;;) {
			if ((_tokenSet_0.member(LA(1)))) {
				s=scoredExtentNode();
				if ( inputState->guessing==0 ) {
#line 183 "indrilang.g"
					
					c->addChild(s);
					
#line 103 "QueryParser.cpp"
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
	return q ;
}

 indri::lang::ScoredExtentNode*  QueryParser::scoredExtentNode() {
#line 189 "indrilang.g"
	 indri::lang::ScoredExtentNode* s ;
#line 132 "QueryParser.cpp"
	
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
	return s ;
}

 indri::lang::ScoredExtentNode*  QueryParser::weightNode() {
#line 286 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 226 "QueryParser.cpp"
#line 286 "indrilang.g"
	
	indri::lang::WeightNode* wn = new indri::lang::WeightNode;
	_nodes.push_back(wn);
	
#line 232 "QueryParser.cpp"
	
	match(WEIGHT);
	r=weightedList(wn);
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::combineNode() {
#line 293 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 242 "QueryParser.cpp"
#line 293 "indrilang.g"
	
	indri::lang::CombineNode* cn = new indri::lang::CombineNode;
	_nodes.push_back(cn);
	
#line 248 "QueryParser.cpp"
	
	match(COMBINE);
	r=unweightedList(cn);
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::orNode() {
#line 321 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 258 "QueryParser.cpp"
#line 321 "indrilang.g"
	
	indri::lang::OrNode* on = new indri::lang::OrNode;
	_nodes.push_back(on);
	
#line 264 "QueryParser.cpp"
	
	match(OR);
	r=unweightedList(on);
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::identSimNode() {
#line 328 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 274 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  variation = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 328 "indrilang.g"
	
	indri::lang::IdentSimNode* isn = new indri::lang::IdentSimNode;
	_nodes.push_back(isn);
	
#line 281 "QueryParser.cpp"
	
	match(IDENTSIM);
	variation = LT(1);
	match(NUMBER);
	r=unweightedList(isn);
	if ( inputState->guessing==0 ) {
#line 333 "indrilang.g"
		
		isn->setVariation( variation->getText() );
		
#line 292 "QueryParser.cpp"
	}
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::notNode() {
#line 344 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 300 "QueryParser.cpp"
#line 344 "indrilang.g"
	
	indri::lang::NotNode* n = new indri::lang::NotNode;
	indri::lang::ScoredExtentNode* c = 0;
	_nodes.push_back(n);
	r = n;
	
#line 308 "QueryParser.cpp"
	
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
#line 352 "indrilang.g"
		
		n->setChild(c);
		
#line 336 "QueryParser.cpp"
	}
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::wandNode() {
#line 314 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 344 "QueryParser.cpp"
#line 314 "indrilang.g"
	
	indri::lang::WAndNode* wn = new indri::lang::WAndNode;
	_nodes.push_back(wn);
	
#line 350 "QueryParser.cpp"
	
	match(WAND);
	r=weightedList(wn);
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::wsumNode() {
#line 307 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 360 "QueryParser.cpp"
#line 307 "indrilang.g"
	
	indri::lang::WSumNode* wn = new indri::lang::WSumNode;
	_nodes.push_back(wn);
	
#line 366 "QueryParser.cpp"
	
	match(WSUM);
	r=weightedList(wn);
	return r ;
}

 indri::lang::ScoredExtentNode*  QueryParser::maxNode() {
#line 337 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 376 "QueryParser.cpp"
#line 337 "indrilang.g"
	
	indri::lang::MaxNode* mn = new indri::lang::MaxNode;
	_nodes.push_back(mn);
	
#line 382 "QueryParser.cpp"
	
	match(MAX);
	r=unweightedList(mn);
	return r ;
}

 indri::lang::PriorNode*  QueryParser::priorNode() {
#line 356 "indrilang.g"
	 indri::lang::PriorNode* p ;
#line 392 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  name = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 356 "indrilang.g"
	
	indri::lang::Field* field = 0;
	p = 0;
	
#line 399 "QueryParser.cpp"
	
	match(PRIOR);
	match(O_PAREN);
	name = LT(1);
	match(TERM);
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 361 "indrilang.g"
		
		p = _priorFactory->create( name->getText() );
		
		if( p->getFieldName().length() ) {
		field = new Field( p->getFieldName() );
		p->setField( field );
		_nodes.push_back(field);
		}
		
		_nodes.push_back(p);
		
#line 419 "QueryParser.cpp"
	}
	return p ;
}

 indri::lang::FilRejNode*  QueryParser::filrejNode() {
#line 432 "indrilang.g"
	 indri::lang::FilRejNode* fj ;
#line 427 "QueryParser.cpp"
#line 432 "indrilang.g"
	
	RawExtentNode* filter = 0;
	ScoredExtentNode* disallowed = 0;
	
#line 433 "QueryParser.cpp"
	
	match(FILREJ);
	match(O_PAREN);
	filter=unscoredTerm();
	disallowed=scoredExtentNode();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 438 "indrilang.g"
		
		fj = new FilRejNode( filter, disallowed );
		_nodes.push_back(fj);
		
#line 446 "QueryParser.cpp"
	}
	return fj ;
}

 indri::lang::FilReqNode*  QueryParser::filreqNode() {
#line 443 "indrilang.g"
	 indri::lang::FilReqNode* fq ;
#line 454 "QueryParser.cpp"
#line 443 "indrilang.g"
	
	RawExtentNode* filter = 0;
	ScoredExtentNode* required = 0;
	
#line 460 "QueryParser.cpp"
	
	match(FILREQ);
	match(O_PAREN);
	filter=unscoredTerm();
	required=scoredExtentNode();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 449 "indrilang.g"
		
		fq = new FilReqNode( filter, required );
		_nodes.push_back(fq);
		
#line 473 "QueryParser.cpp"
	}
	return fq ;
}

 ScoredExtentNode*  QueryParser::scoredRaw() {
#line 209 "indrilang.g"
	 ScoredExtentNode* sn ;
#line 481 "QueryParser.cpp"
#line 209 "indrilang.g"
	
	RawExtentNode* raw = 0;
	RawExtentNode* contexts = 0;
	sn = 0;
	
#line 488 "QueryParser.cpp"
	
	bool synPredMatched108 = false;
	if (((_tokenSet_1.member(LA(1))))) {
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
#line 216 "indrilang.g"
			
			sn = new indri::lang::RawScorerNode( raw, contexts );
			_nodes.push_back(sn);
			
#line 516 "QueryParser.cpp"
		}
	}
	else {
		bool synPredMatched110 = false;
		if (((_tokenSet_1.member(LA(1))))) {
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
#line 221 "indrilang.g"
				
				sn = new indri::lang::RawScorerNode( raw, contexts );
				_nodes.push_back(sn);
				
#line 544 "QueryParser.cpp"
			}
		}
		else {
			bool synPredMatched112 = false;
			if (((_tokenSet_1.member(LA(1))))) {
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
#line 226 "indrilang.g"
					
					sn = new indri::lang::RawScorerNode( raw, contexts );
					_nodes.push_back(sn);
					
#line 575 "QueryParser.cpp"
				}
			}
			else if ((_tokenSet_1.member(LA(1)))) {
				raw=unqualifiedTerm();
				if ( inputState->guessing==0 ) {
#line 231 "indrilang.g"
					
					sn = new indri::lang::RawScorerNode( raw, contexts );
					_nodes.push_back(sn);
					
#line 586 "QueryParser.cpp"
				}
			}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}}
	return sn ;
}

 RawExtentNode*  QueryParser::qualifiedTerm() {
#line 470 "indrilang.g"
	 RawExtentNode* t ;
#line 599 "QueryParser.cpp"
#line 470 "indrilang.g"
	
	RawExtentNode* synonyms = 0;
	RawExtentNode* fields = 0;
	t = 0;
	
#line 606 "QueryParser.cpp"
	
	synonyms=unqualifiedTerm();
	match(DOT);
	fields=field_list();
	if ( inputState->guessing==0 ) {
#line 477 "indrilang.g"
		
		if( fields ) {
		t = new indri::lang::ExtentInside( synonyms, fields );
		_nodes.push_back(t);
		} else {
		t = synonyms;
		}
		
#line 621 "QueryParser.cpp"
	}
	return t ;
}

 ExtentOr*  QueryParser::context_list() {
#line 578 "indrilang.g"
	 ExtentOr* contexts ;
#line 629 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  first = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  additional = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 578 "indrilang.g"
	
	contexts = new ExtentOr;
	_nodes.push_back( contexts );
	
#line 637 "QueryParser.cpp"
	
	match(O_PAREN);
	first = LT(1);
	match(TERM);
	if ( inputState->guessing==0 ) {
#line 584 "indrilang.g"
		
		Field* firstField = new indri::lang::Field( first->getText() );
		_nodes.push_back( firstField );
		contexts->addChild( firstField );
		
#line 649 "QueryParser.cpp"
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			additional = LT(1);
			match(TERM);
			if ( inputState->guessing==0 ) {
#line 591 "indrilang.g"
				
				Field* additionalField = new Field(additional->getText());
				_nodes.push_back( additionalField );
				contexts->addChild( additionalField );
				
#line 664 "QueryParser.cpp"
			}
		}
		else {
			goto _loop211;
		}
		
	}
	_loop211:;
	} // ( ... )*
	match(C_PAREN);
	return contexts ;
}

 RawExtentNode*  QueryParser::unqualifiedTerm() {
#line 486 "indrilang.g"
	 RawExtentNode* re ;
#line 681 "QueryParser.cpp"
	
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
	return re ;
}

 indri::lang::ScoredExtentNode*  QueryParser::weightedList(
	 indri::lang::WeightedCombinationNode* wn 
) {
#line 247 "indrilang.g"
	 indri::lang::ScoredExtentNode* sr ;
#line 778 "QueryParser.cpp"
#line 247 "indrilang.g"
	
	double w = 0;
	ScoredExtentNode* n = 0;
	sr = wn;
	
#line 785 "QueryParser.cpp"
	
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
#line 258 "indrilang.g"
				wn->addChild( w, n );
#line 814 "QueryParser.cpp"
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
	return sr ;
}

 indri::lang::ScoredExtentNode*  QueryParser::extentRestriction(
	 indri::lang::ScoredExtentNode* sn 
) {
#line 503 "indrilang.g"
	 indri::lang::ScoredExtentNode* er ;
#line 834 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  passageWindowSize = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  inc = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 503 "indrilang.g"
	
	indri::lang::Field* f = 0;
	er = 0;
	
#line 843 "QueryParser.cpp"
	
	bool synPredMatched196 = false;
	if (((LA(1) == O_SQUARE))) {
		int _m196 = mark();
		synPredMatched196 = true;
		inputState->guessing++;
		try {
			{
			match(O_SQUARE);
			match(TERM);
			match(COLON);
			}
		}
		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
			synPredMatched196 = false;
		}
		rewind(_m196);
		inputState->guessing--;
	}
	if ( synPredMatched196 ) {
		match(O_SQUARE);
		passageWindowSize = LT(1);
		match(TERM);
		match(COLON);
		inc = LT(1);
		match(NUMBER);
		match(C_SQUARE);
		if ( inputState->guessing==0 ) {
#line 508 "indrilang.g"
			
			int startWindow;
			
			for( startWindow = 0; startWindow < passageWindowSize->getText().size(); startWindow++ ) {
			if( isdigit( passageWindowSize->getText()[startWindow] ) )
			break;
			}
			
			int increment = atoi(inc->getText().c_str());
			int windowSize = atoi(passageWindowSize->getText().c_str() + startWindow );
			
			er = new indri::lang::FixedPassage(sn, windowSize, increment);
			
#line 886 "QueryParser.cpp"
		}
	}
	else if ((LA(1) == O_SQUARE)) {
		match(O_SQUARE);
		field = LT(1);
		match(TERM);
		match(C_SQUARE);
		if ( inputState->guessing==0 ) {
#line 522 "indrilang.g"
			
			f = new indri::lang::Field(field->getText());
			_nodes.push_back(f);
			er = new indri::lang::ExtentRestriction(sn, f);
			_nodes.push_back(er);
			
#line 902 "QueryParser.cpp"
		}
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	return er ;
}

 double  QueryParser::floating() {
#line 728 "indrilang.g"
	 double d ;
#line 915 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  f = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 728 "indrilang.g"
	
	d = 0;
	
#line 922 "QueryParser.cpp"
	
	switch ( LA(1)) {
	case FLOAT:
	{
		f = LT(1);
		match(FLOAT);
		if ( inputState->guessing==0 ) {
#line 731 "indrilang.g"
			
			d = atof(f->getText().c_str());
			
#line 934 "QueryParser.cpp"
		}
		break;
	}
	case NUMBER:
	{
		n = LT(1);
		match(NUMBER);
		if ( inputState->guessing==0 ) {
#line 734 "indrilang.g"
			
			d = atof(n->getText().c_str());
			
#line 947 "QueryParser.cpp"
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return d ;
}

 indri::lang::ScoredExtentNode*  QueryParser::sumList(
	 indri::lang::WSumNode* wn 
) {
#line 263 "indrilang.g"
	 indri::lang::ScoredExtentNode* sr ;
#line 964 "QueryParser.cpp"
#line 263 "indrilang.g"
	
	double w = 0;
	ScoredExtentNode* n = 0;
	sr = wn;
	
#line 971 "QueryParser.cpp"
	
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
#line 271 "indrilang.g"
				wn->addChild( 1.0, n );
#line 999 "QueryParser.cpp"
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
	return sr ;
}

 indri::lang::ScoredExtentNode*  QueryParser::unweightedList(
	 indri::lang::UnweightedCombinationNode* cn 
) {
#line 275 "indrilang.g"
	 indri::lang::ScoredExtentNode* sr ;
#line 1019 "QueryParser.cpp"
#line 275 "indrilang.g"
	
	ScoredExtentNode* n = 0;
	sr = cn;
	
#line 1025 "QueryParser.cpp"
	
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
#line 282 "indrilang.g"
				cn->addChild( n );
#line 1053 "QueryParser.cpp"
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
	return sr ;
}

 indri::lang::ScoredExtentNode*  QueryParser::sumNode() {
#line 300 "indrilang.g"
	 indri::lang::ScoredExtentNode* r ;
#line 1071 "QueryParser.cpp"
#line 300 "indrilang.g"
	
	indri::lang::WSumNode* wn = new indri::lang::WSumNode;
	_nodes.push_back(wn);
	
#line 1077 "QueryParser.cpp"
	
	match(SUM);
	r=sumList(wn);
	return r ;
}

 indri::lang::ODNode*  QueryParser::odNode() {
#line 383 "indrilang.g"
	 indri::lang::ODNode* od ;
#line 1087 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  n1 = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  n2 = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 383 "indrilang.g"
	
	RawExtentNode* rn = 0;
	od = new indri::lang::ODNode;
	_nodes.push_back(od);
	
#line 1096 "QueryParser.cpp"
	
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
#line 392 "indrilang.g"
			od->setWindowSize( n1->getText() );
#line 1124 "QueryParser.cpp"
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
#line 396 "indrilang.g"
				od->setWindowSize( n2->getText() );
#line 1158 "QueryParser.cpp"
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
		if ((_tokenSet_1.member(LA(1)))) {
			rn=unscoredTerm();
			if ( inputState->guessing==0 ) {
#line 400 "indrilang.g"
				od->addChild( rn );
#line 1176 "QueryParser.cpp"
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
	return od ;
}

 RawExtentNode*  QueryParser::unscoredTerm() {
#line 463 "indrilang.g"
	 RawExtentNode* t ;
#line 1194 "QueryParser.cpp"
#line 463 "indrilang.g"
	
	t = 0;
	
#line 1199 "QueryParser.cpp"
	
	bool synPredMatched163 = false;
	if (((_tokenSet_1.member(LA(1))))) {
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
	else if ((_tokenSet_1.member(LA(1)))) {
		t=unqualifiedTerm();
	}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	
	return t ;
}

 indri::lang::UWNode*  QueryParser::uwNode() {
#line 404 "indrilang.g"
	 indri::lang::UWNode* uw ;
#line 1233 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 404 "indrilang.g"
	
	uw = new indri::lang::UWNode;
	RawExtentNode* rn = 0;
	_nodes.push_back(uw);
	
#line 1241 "QueryParser.cpp"
	
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
#line 412 "indrilang.g"
			uw->setWindowSize( n->getText() );
#line 1269 "QueryParser.cpp"
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
		if ((_tokenSet_1.member(LA(1)))) {
			rn=unscoredTerm();
			if ( inputState->guessing==0 ) {
#line 418 "indrilang.g"
				uw->addChild( rn );
#line 1292 "QueryParser.cpp"
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
	return uw ;
}

 indri::lang::BAndNode*  QueryParser::bandNode() {
#line 421 "indrilang.g"
	 indri::lang::BAndNode* b ;
#line 1310 "QueryParser.cpp"
#line 421 "indrilang.g"
	
	b = new indri::lang::BAndNode;
	RawExtentNode* rn = 0;
	_nodes.push_back(b);
	
#line 1317 "QueryParser.cpp"
	
	match(BAND);
	match(O_PAREN);
	{ // ( ... )+
	int _cnt157=0;
	for (;;) {
		if ((_tokenSet_1.member(LA(1)))) {
			rn=unscoredTerm();
			if ( inputState->guessing==0 ) {
#line 429 "indrilang.g"
				b->addChild( rn );
#line 1329 "QueryParser.cpp"
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
	return b ;
}

 indri::lang::Field*  QueryParser::anyField() {
#line 454 "indrilang.g"
	 indri::lang::Field* f ;
#line 1347 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  t = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 454 "indrilang.g"
	
	f = 0;
	
#line 1353 "QueryParser.cpp"
	
	match(ANY);
	match(COLON);
	t = LT(1);
	match(TERM);
	if ( inputState->guessing==0 ) {
#line 458 "indrilang.g"
		
		f = new Field(t->getText());
		_nodes.push_back(f);
		
#line 1365 "QueryParser.cpp"
	}
	return f ;
}

 ExtentAnd*  QueryParser::field_list() {
#line 558 "indrilang.g"
	 ExtentAnd* fields ;
#line 1373 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  first = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  additional = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 558 "indrilang.g"
	
	fields = new ExtentAnd;
	_nodes.push_back( fields );
	
#line 1381 "QueryParser.cpp"
	
	first = LT(1);
	match(TERM);
	if ( inputState->guessing==0 ) {
#line 564 "indrilang.g"
		
		Field* firstField = new indri::lang::Field( first->getText() );
		_nodes.push_back( firstField );
		fields->addChild( firstField );
		
#line 1392 "QueryParser.cpp"
	}
	{ // ( ... )*
	for (;;) {
		if ((LA(1) == COMMA)) {
			match(COMMA);
			additional = LT(1);
			match(TERM);
			if ( inputState->guessing==0 ) {
#line 571 "indrilang.g"
				
				Field* additionalField = new Field(additional->getText());
				_nodes.push_back( additionalField );
				fields->addChild( additionalField );
				
#line 1407 "QueryParser.cpp"
			}
		}
		else {
			goto _loop208;
		}
		
	}
	_loop208:;
	} // ( ... )*
	return fields ;
}

 FieldLessNode*  QueryParser::dateBefore() {
#line 607 "indrilang.g"
	 FieldLessNode* extent ;
#line 1423 "QueryParser.cpp"
#line 607 "indrilang.g"
	
	UINT64 d = 0;
	Field* dateField = 0;
	extent = 0;
	
#line 1430 "QueryParser.cpp"
	
	match(DATEBEFORE);
	match(O_PAREN);
	d=date();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 612 "indrilang.g"
		
		dateField = new Field("date");
		extent = new FieldLessNode( dateField, d );
		_nodes.push_back( dateField );
		_nodes.push_back( extent );
		
#line 1444 "QueryParser.cpp"
	}
	return extent ;
}

 FieldGreaterNode*  QueryParser::dateAfter() {
#line 619 "indrilang.g"
	 FieldGreaterNode* extent ;
#line 1452 "QueryParser.cpp"
#line 619 "indrilang.g"
	
	UINT64 d = 0;
	Field* dateField = 0;
	extent = 0;
	
#line 1459 "QueryParser.cpp"
	
	match(DATEAFTER);
	match(O_PAREN);
	d=date();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 624 "indrilang.g"
		
		dateField = new Field("date");
		extent = new FieldGreaterNode( dateField, d );
		_nodes.push_back( dateField );
		_nodes.push_back( extent );
		
#line 1473 "QueryParser.cpp"
	}
	return extent ;
}

 FieldBetweenNode*  QueryParser::dateBetween() {
#line 631 "indrilang.g"
	 FieldBetweenNode* extent ;
#line 1481 "QueryParser.cpp"
#line 631 "indrilang.g"
	
	UINT64 low = 0;
	UINT64 high = 0;
	Field* dateField = 0;
	extent = 0;
	
#line 1489 "QueryParser.cpp"
	
	match(DATEBETWEEN);
	match(O_PAREN);
	low=date();
	match(COMMA);
	high=date();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 637 "indrilang.g"
		
		dateField = new Field("date");
		extent = new FieldBetweenNode( dateField, low, high );
		_nodes.push_back( dateField );
		_nodes.push_back( extent );
		
#line 1505 "QueryParser.cpp"
	}
	return extent ;
}

 indri::lang::ExtentOr*  QueryParser::synonym_list() {
#line 529 "indrilang.g"
	 indri::lang::ExtentOr* s ;
#line 1513 "QueryParser.cpp"
#line 529 "indrilang.g"
	
	indri::lang::RawExtentNode* term = 0;
	s = new indri::lang::ExtentOr;
	_nodes.push_back(s);
	
#line 1520 "QueryParser.cpp"
	
	match(O_ANGLE);
	{ // ( ... )+
	int _cnt199=0;
	for (;;) {
		if ((_tokenSet_1.member(LA(1)))) {
			term=unscoredTerm();
			if ( inputState->guessing==0 ) {
#line 535 "indrilang.g"
				s->addChild(term);
#line 1531 "QueryParser.cpp"
			}
		}
		else {
			if ( _cnt199>=1 ) { goto _loop199; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt199++;
	}
	_loop199:;
	}  // ( ... )+
	match(C_ANGLE);
	return s ;
}

 indri::lang::ExtentOr*  QueryParser::synonym_list_brace() {
#line 538 "indrilang.g"
	 indri::lang::ExtentOr* s ;
#line 1549 "QueryParser.cpp"
#line 538 "indrilang.g"
	
	indri::lang::RawExtentNode* term = 0;
	s = new indri::lang::ExtentOr;
	_nodes.push_back(s);
	
#line 1556 "QueryParser.cpp"
	
	match(O_BRACE);
	{ // ( ... )+
	int _cnt202=0;
	for (;;) {
		if ((_tokenSet_1.member(LA(1)))) {
			term=unscoredTerm();
			if ( inputState->guessing==0 ) {
#line 544 "indrilang.g"
				s->addChild(term);
#line 1567 "QueryParser.cpp"
			}
		}
		else {
			if ( _cnt202>=1 ) { goto _loop202; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt202++;
	}
	_loop202:;
	}  // ( ... )+
	match(C_BRACE);
	return s ;
}

 indri::lang::ExtentOr*  QueryParser::synonym_list_alt() {
#line 547 "indrilang.g"
	 indri::lang::ExtentOr* s ;
#line 1585 "QueryParser.cpp"
#line 547 "indrilang.g"
	
	indri::lang::RawExtentNode* term = 0;
	// semantics of this node will change
	s = new indri::lang::ExtentOr;
	_nodes.push_back(s);
	
#line 1593 "QueryParser.cpp"
	
	match(SYN);
	match(O_PAREN);
	{ // ( ... )+
	int _cnt205=0;
	for (;;) {
		if ((_tokenSet_1.member(LA(1)))) {
			term=unscoredTerm();
			if ( inputState->guessing==0 ) {
#line 555 "indrilang.g"
				s->addChild(term);
#line 1605 "QueryParser.cpp"
			}
		}
		else {
			if ( _cnt205>=1 ) { goto _loop205; } else {throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());}
		}
		
		_cnt205++;
	}
	_loop205:;
	}  // ( ... )+
	match(C_PAREN);
	return s ;
}

 FieldLessNode*  QueryParser::lessNode() {
#line 760 "indrilang.g"
	 FieldLessNode* ln ;
#line 1623 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 760 "indrilang.g"
	
	ln = 0;
	Field* compareField = 0;
	INT64 high = 0;
	
#line 1631 "QueryParser.cpp"
	
	match(LESS);
	match(O_PAREN);
	field = LT(1);
	match(TERM);
	high=number();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 765 "indrilang.g"
		
		compareField = new Field(field->getText());
		ln = new FieldLessNode( compareField, high );
		_nodes.push_back( compareField );
		_nodes.push_back( ln );
		
#line 1647 "QueryParser.cpp"
	}
	return ln ;
}

 FieldGreaterNode*  QueryParser::greaterNode() {
#line 748 "indrilang.g"
	 FieldGreaterNode* gn ;
#line 1655 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 748 "indrilang.g"
	
	gn = 0;
	Field* compareField = 0;
	INT64 low = 0;
	
#line 1663 "QueryParser.cpp"
	
	match(GREATER);
	match(O_PAREN);
	field = LT(1);
	match(TERM);
	low=number();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 753 "indrilang.g"
		
		compareField = new Field(field->getText());
		gn = new FieldGreaterNode( compareField, low );
		_nodes.push_back( compareField );
		_nodes.push_back( gn );
		
#line 1679 "QueryParser.cpp"
	}
	return gn ;
}

 FieldBetweenNode*  QueryParser::betweenNode() {
#line 772 "indrilang.g"
	 FieldBetweenNode* bn ;
#line 1687 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 772 "indrilang.g"
	
	bn = 0;
	Field* compareField = 0;
	INT64 low = 0;
	INT64 high = 0;
	
#line 1696 "QueryParser.cpp"
	
	match(BETWEEN);
	match(O_PAREN);
	field = LT(1);
	match(TERM);
	low=number();
	high=number();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 778 "indrilang.g"
		
		compareField = new Field(field->getText());
		bn = new FieldBetweenNode( compareField, low, high );
		_nodes.push_back( compareField );
		_nodes.push_back( bn );
		
#line 1713 "QueryParser.cpp"
	}
	return bn ;
}

 FieldEqualsNode*  QueryParser::equalsNode() {
#line 785 "indrilang.g"
	 FieldEqualsNode* en ;
#line 1721 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  field = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 785 "indrilang.g"
	
	en = 0;
	Field* compareField = 0;
	INT64 eq = 0;
	
#line 1729 "QueryParser.cpp"
	
	match(EQUALS);
	match(O_PAREN);
	field = LT(1);
	match(TERM);
	eq=number();
	match(C_PAREN);
	if ( inputState->guessing==0 ) {
#line 790 "indrilang.g"
		
		compareField = new Field(field->getText());
		en = new FieldEqualsNode( compareField, eq );
		_nodes.push_back( compareField );
		_nodes.push_back( en );
		
#line 1745 "QueryParser.cpp"
	}
	return en ;
}

 IndexTerm*  QueryParser::rawText() {
#line 693 "indrilang.g"
	 IndexTerm* t ;
#line 1753 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  id = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  f = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  et = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  qet = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 693 "indrilang.g"
	
	t = 0;
	
#line 1763 "QueryParser.cpp"
	
	switch ( LA(1)) {
	case TERM:
	{
		id = LT(1);
		match(TERM);
		if ( inputState->guessing==0 ) {
#line 696 "indrilang.g"
			
			t = new indri::lang::IndexTerm(id->getText());
			_nodes.push_back(t);
			
#line 1776 "QueryParser.cpp"
		}
		break;
	}
	case NUMBER:
	{
		n = LT(1);
		match(NUMBER);
		if ( inputState->guessing==0 ) {
#line 700 "indrilang.g"
			
			t = new indri::lang::IndexTerm(n->getText());
			_nodes.push_back(t);
			
#line 1790 "QueryParser.cpp"
		}
		break;
	}
	case FLOAT:
	{
		f = LT(1);
		match(FLOAT);
		if ( inputState->guessing==0 ) {
#line 704 "indrilang.g"
			
			t = new indri::lang::IndexTerm(f->getText());
			_nodes.push_back(t);
			
#line 1804 "QueryParser.cpp"
		}
		break;
	}
	case DBL_QUOTE:
	{
		match(DBL_QUOTE);
		t=rawText();
		match(DBL_QUOTE);
		if ( inputState->guessing==0 ) {
#line 708 "indrilang.g"
			
			// if a text term appears in quotes, consider it stemmed
			t->setStemmed(true);
			
#line 1819 "QueryParser.cpp"
		}
		break;
	}
	case ENCODED_TERM:
	{
		et = LT(1);
		match(ENCODED_TERM);
		if ( inputState->guessing==0 ) {
#line 712 "indrilang.g"
			
			std::string decodedString; 
			base64_decode_string(decodedString, et->getText());
			t = new indri::lang::IndexTerm( decodedString );
			_nodes.push_back(t);
			
#line 1835 "QueryParser.cpp"
		}
		break;
	}
	case ENCODED_QUOTED_TERM:
	{
		qet = LT(1);
		match(ENCODED_QUOTED_TERM);
		if ( inputState->guessing==0 ) {
#line 718 "indrilang.g"
			
			std::string decodedString; 
			base64_decode_string(decodedString, et->getText());
			t = new indri::lang::IndexTerm( decodedString );
			t->setStemmed(true);
			_nodes.push_back(t);
			
#line 1852 "QueryParser.cpp"
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return t ;
}

 Field*  QueryParser::field_restriction() {
#line 599 "indrilang.g"
	 Field* extent ;
#line 1867 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  fieldName = ANTLR_USE_NAMESPACE(antlr)nullToken;
	
	match(O_SQUARE);
	fieldName = LT(1);
	match(TERM);
	if ( inputState->guessing==0 ) {
#line 601 "indrilang.g"
		
		extent = new Field( fieldName->getText() );
		_nodes.push_back( extent );
		
#line 1879 "QueryParser.cpp"
	}
	match(C_SQUARE);
	return extent ;
}

 UINT64  QueryParser::date() {
#line 659 "indrilang.g"
	 UINT64 d ;
#line 1888 "QueryParser.cpp"
	
	bool synPredMatched218 = false;
	if (((LA(1) == NUMBER))) {
		int _m218 = mark();
		synPredMatched218 = true;
		inputState->guessing++;
		try {
			{
			match(NUMBER);
			match(SLASH);
			}
		}
		catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
			synPredMatched218 = false;
		}
		rewind(_m218);
		inputState->guessing--;
	}
	if ( synPredMatched218 ) {
		d=slashDate();
	}
	else {
		bool synPredMatched220 = false;
		if (((LA(1) == NUMBER))) {
			int _m220 = mark();
			synPredMatched220 = true;
			inputState->guessing++;
			try {
				{
				match(NUMBER);
				}
			}
			catch (ANTLR_USE_NAMESPACE(antlr)RecognitionException& pe) {
				synPredMatched220 = false;
			}
			rewind(_m220);
			inputState->guessing--;
		}
		if ( synPredMatched220 ) {
			d=spaceDate();
		}
		else if ((LA(1) == TERM)) {
			d=dashDate();
		}
	else {
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
	}
	}
	return d ;
}

 UINT64  QueryParser::slashDate() {
#line 678 "indrilang.g"
	 UINT64 d ;
#line 1943 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  month = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  day = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  year = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 678 "indrilang.g"
	
	d = 0;
	
#line 1951 "QueryParser.cpp"
	
	month = LT(1);
	match(NUMBER);
	match(SLASH);
	day = LT(1);
	match(TERM);
	match(SLASH);
	year = LT(1);
	match(NUMBER);
	if ( inputState->guessing==0 ) {
#line 681 "indrilang.g"
		
		d = DateParse::convertDate( year->getText(), month->getText(), day->getText() ); 
		
#line 1966 "QueryParser.cpp"
	}
	return d ;
}

 UINT64  QueryParser::spaceDate() {
#line 685 "indrilang.g"
	 UINT64 d ;
#line 1974 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  day = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  month = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  year = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 685 "indrilang.g"
	
	d = 0;
	
#line 1982 "QueryParser.cpp"
	
	day = LT(1);
	match(NUMBER);
	month = LT(1);
	match(TERM);
	year = LT(1);
	match(NUMBER);
	if ( inputState->guessing==0 ) {
#line 688 "indrilang.g"
		
		d = DateParse::convertDate( year->getText(), month->getText(), day->getText() );
		
#line 1995 "QueryParser.cpp"
	}
	return d ;
}

 UINT64  QueryParser::dashDate() {
#line 664 "indrilang.g"
	 UINT64 d ;
#line 2003 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  dmy = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 664 "indrilang.g"
	
	d = 0;
	
#line 2009 "QueryParser.cpp"
	
	dmy = LT(1);
	match(TERM);
	if ( inputState->guessing==0 ) {
#line 667 "indrilang.g"
		
		const std::string& text = dmy->getText();
		int firstDash = text.find('-');
		int secondDash = text.find('-', firstDash+1);
		std::string day = text.substr( 0, firstDash ); 
		std::string month = text.substr( firstDash+1, secondDash-firstDash-1 );
		std::string year = text.substr( secondDash );
		
		d = DateParse::convertDate( year, month, day ); 
		
#line 2025 "QueryParser.cpp"
	}
	return d ;
}

 INT64  QueryParser::number() {
#line 738 "indrilang.g"
	 INT64 v ;
#line 2033 "QueryParser.cpp"
	ANTLR_USE_NAMESPACE(antlr)RefToken  n = ANTLR_USE_NAMESPACE(antlr)nullToken;
	ANTLR_USE_NAMESPACE(antlr)RefToken  nn = ANTLR_USE_NAMESPACE(antlr)nullToken;
#line 738 "indrilang.g"
	
	v = 0;
	
#line 2040 "QueryParser.cpp"
	
	switch ( LA(1)) {
	case NUMBER:
	{
		n = LT(1);
		match(NUMBER);
		if ( inputState->guessing==0 ) {
#line 741 "indrilang.g"
			
			v = string_to_i64(n->getText());
			
#line 2052 "QueryParser.cpp"
		}
		break;
	}
	case NEGATIVE_NUMBER:
	{
		nn = LT(1);
		match(NEGATIVE_NUMBER);
		if ( inputState->guessing==0 ) {
#line 744 "indrilang.g"
			
			v = string_to_i64(nn->getText());
			
#line 2065 "QueryParser.cpp"
		}
		break;
	}
	default:
	{
		throw ANTLR_USE_NAMESPACE(antlr)NoViableAltException(LT(1), getFilename());
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
const unsigned long QueryParser::_tokenSet_1_data_[] = { 1609435264UL, 503316804UL, 0UL, 0UL };
// "#od" "#uw" "#any" "#band" "#syn" "#date:after" "#date:before" "#date:between" 
// "#less" "#greater" "#between" "#equals" NUMBER FLOAT O_ANGLE O_BRACE 
// DBL_QUOTE TERM ENCODED_QUOTED_TERM ENCODED_TERM OPERATOR 
const ANTLR_USE_NAMESPACE(antlr)BitSet QueryParser::_tokenSet_1(_tokenSet_1_data_,4);


ANTLR_END_NAMESPACE
ANTLR_END_NAMESPACE
