/*==========================================================================
 * Copyright (c) 2013 University of Massachusetts.  All Rights Reserved.
 *
 * Use of the Lemur Toolkit for Language Modeling and Information Retrieval
 * is subject to the terms of the software license set forth in the LICENSE
 * file included with this software, and also available at
 * http://www.lemurproject.org/license.html
 *
 *==========================================================================
 */
#include "KrovetzStemmer.hpp"
#include <cstdlib>
#include <cstring>
#include <fstream>

int main(int argc, char *argv[])
{
  char word[80];
  char thestem[80];
  int ret;
  stem::KrovetzStemmer * stemmer = new stem::KrovetzStemmer();
   
  if (std::string(argv[1]) == "-w") {
      ret = stemmer->kstem_stem_tobuffer(argv[2], thestem);
    if (ret > 0) 
      printf("%s -> %s\n", argv[2],thestem);
    else 
      printf("%s -> %s\n", argv[2], argv[2]);
  } else {
    std::ifstream input(argv[1]);
    while (input.good()) {
      input.getline(word, 80);
      if (strlen(word) == 0) break;
      ret = stemmer->kstem_stem_tobuffer(word, thestem);
      if (ret > 0) 
        printf("%s %s\n", word,thestem);
      else 
        printf("%s %s\n", word, word);
      
    }
    input.close();
  }
  delete(stemmer);
  return(0);
}
