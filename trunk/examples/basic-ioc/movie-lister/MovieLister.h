/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _movie_lister_h
# define _movie_lister_h

#include "MovieFinder.h"

class MovieLister {
	MovieFinder* _finder;
  public:
	void finder(MovieFinder* finder);
	Movie* moviesDirectedBy(const char* director);
};

#endif
