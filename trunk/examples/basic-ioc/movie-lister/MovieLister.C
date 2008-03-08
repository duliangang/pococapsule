/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include <stdlib.h>
#include <string.h>

#include "MovieLister.h"

void MovieLister::finder(MovieFinder* the_finder)
{
	_finder = the_finder;
}

Movie* MovieLister::moviesDirectedBy(const char* director)
{
	const Movie* all = _finder->findAll();

	int i, count=0;
	// count titles under this director
	for(i=0;;i++) {
		if( all[i].director == NULL ) {
			break;
		}

		if( strcmp(director, all[i].director) == 0 ) {
			count++;
		}
	}

	Movie* rset = new Movie[count+1];

	for(i=0, count=0;;i++) {
		if( all[i].director == NULL ) {
			break;
		}

		if( strcmp(director, all[i].director) == 0 ) {
			rset[count].title = all[i].title;
			rset[count].director = all[i].director;
			count++;
		}
	}

	rset[count].title = NULL;
	rset[count].director = NULL;

	return rset;
}
