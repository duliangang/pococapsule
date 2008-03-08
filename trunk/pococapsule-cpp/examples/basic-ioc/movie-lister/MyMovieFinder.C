/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include <stdlib.h>
#include "MyMovieFinder.h"

static Movie all_movies[] = {
	{"The Godfather",	"Francis Ford Coppola"},
	{"Appollo 13",		"Ron Howard"},
	{"A Beautiful Mind",	"Ron Howard"},
	{"The Da Vinci Code",	"Ron Howard"},
	{"Waterworld",		"Kevin Reynolds"},
	{"Ghost",		"Jerry Zucker"},
	{"Titanic",		"James Cameron"},
	{"Dances with Wolves",	"Kevin Costner"},
	{NULL,			NULL} };

const Movie* MyMovieFinder::findAll() 
{
	return all_movies;
}
