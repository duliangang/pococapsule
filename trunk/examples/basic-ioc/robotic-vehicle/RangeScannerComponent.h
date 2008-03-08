/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _range_scanner_component_impl_h
#define _range_scanner_component_impl_h

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows
#include "objImpls.h"

class POCO_EXPORT RangeScannerComponent
{
	std::string	_name;
	ScanControl*		_control;
	RangeScanDataConsumer*	_consumer;

  public:
	RangeScannerComponent(const char* name);
	const char* name() { return _name.c_str(); }

	// facets
	ScanControl* ControlIn();

	// emitters
	void RangeScanDataOut(RangeScanDataConsumer* consumer);
};

#endif
