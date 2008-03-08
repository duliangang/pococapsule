/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "pocoapp.h"
#include <stdio.h>

#include "MovieFinder.h"
#include "MovieLister.h"

int main(int argc, char** argv)
{
	POCO_AppEnv* env = POCO_AppContext::initDefaultAppEnv(argc, argv);

	POCO_AppContext* ctx = POCO_AppContext::create("setup.xml", "file");

	if( ctx == NULL ) {
		printf("failure: %s\n", env->get_message());
		return -1;
	}

	MovieLister* lister = (MovieLister*)ctx->getBean("MovieLister");
	Movie* movies = lister->moviesDirectedBy("Ron Howard");

	//
	// show all titles of this director
	//
	printf("Some movies directed by Ron Howard:\n");
	for(int i=0;;i++) {
		if( movies[i].title == NULL ) {
			break;
		}

		printf("%d. %s\n", i+1, movies[i].title);
	}

	return 0;
}
