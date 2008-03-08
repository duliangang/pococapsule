/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "reader.h"
#include <iostream>

void read_props(std::map<std::string, std::string> &props)
{
	std::map<std::string, std::string>::iterator cur = props.begin();

	std::cout << "map content:" << std::endl; 
	for(;cur!= props.end();cur++) {
		std::cout << cur->first << ":\t" 
		          << cur->second << std::endl;
	}
}
