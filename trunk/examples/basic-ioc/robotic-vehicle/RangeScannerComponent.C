/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "RangeScannerComponent.h"
#include "objImpls.h"

RangeScannerComponent::RangeScannerComponent(const char* name) : _name(name)
{ 
	char buf[256];
	sprintf(buf, "ControlIn reference of the %s", name);
	_control = new ScanControl(buf); 
}

ScanControl* RangeScannerComponent::ControlIn()
{
	return _control;
}

void RangeScannerComponent::RangeScanDataOut(RangeScanDataConsumer* consumer)
{
	printf("[wire]: The %s is injected into the RangeScanDataOut port of the %s\n", 
			consumer->name(), name());
	_consumer = consumer;
}
