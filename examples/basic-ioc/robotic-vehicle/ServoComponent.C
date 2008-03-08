/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "ServoComponent.h"
#include "objImpls.h"

ServoComponent::ServoComponent(const char* name) : _name(name)
{
	char buf[256];
	sprintf(buf, "ControlIn reference of the %s", name);
	_control = new ServoControl(buf); 
}

ServoControl* ServoComponent::ControlIn()
{
	return _control;
}

void ServoComponent::SpeedDataOut(SpeedDataConsumer* consumer)
{
	printf("[wire]: The %s is injected into the SpeedDataOut port of the %s\n", 
			consumer->name(), name());
	_consumer = consumer;
}
