/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC. All Rights Reserved.	    */
/*									    */
/*  This code is the property of Pocomatic Software LLC., and is	    */
/*  protected  by  copyright under the laws of the United States.	    */
/*  This  program is confidential, proprietary, and  a	trade secret,	    */
/*  not  to  be disclosed  without written authorization from Pocomatic     */
/*  Software, Inc.  Any use,  duplication,  or disclosure of this program   */
/*  by other than  Pocomatic Software Inc. and its assigned licensees	    */
/*  is strictly forbidden by law.					    */
/*									    */
/*  Author: Ke Jin <kejin@pocomatic.com>				    */
/*									    */
/****************************************************************************/
#ifndef _poco_string_h_
# define _poco_string_h_

class POCO_String {
	char*	_str;
  public:
	POCO_String();
	POCO_String(char*);
	POCO_String(const char*);
	~POCO_String();

	const char* in() { return _str; }
	char*&	    inout() { return _str; }
	char*	    retn();

	POCO_String& operator=(const char*);
	POCO_String& operator=(char*);
	
	operator const char *() const { return _str; }

	// POCO_String& operator+(const char*);
	POCO_String& operator+=(const char*);

	static char*	alloc(int size);
	static char*	dup(const char* str);
	static void	free(char* str);

	void replace(char old_char, char new_char);
};

#endif
