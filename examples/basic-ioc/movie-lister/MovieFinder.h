/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _movie_finder_h_
# define _movie_finder_h_

struct Movie {
	const char*	title;
	const char*	director;
};

//
// this class defines a interface of MovieFinder object
//
class MovieFinder {
  public:
    virtual ~MovieFinder() {}
    virtual const Movie*	findAll() = 0;
};

#endif
