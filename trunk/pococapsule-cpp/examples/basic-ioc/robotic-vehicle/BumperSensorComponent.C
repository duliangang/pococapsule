/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "BumperSensorComponent.h"
#include "objImpls.h"

BumperSensorComponent::BumperSensorComponent(const char* name) : _name(name)
{ 
}

void BumperSensorComponent::SensorDataOut(BumperDataConsumer* consumer)
{
	printf("[wire]: The %s is injected into the BumperDataOut port of the %s\n", 
			consumer->name(), name());
	_consumer = consumer;
}
