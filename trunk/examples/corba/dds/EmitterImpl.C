/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "EmitterImpl.h"
#include <stdio.h>

void MyDataWriterEmitterImpl::connect_foo_writer(DDS::DataWriter_ptr writer)
{
	FooDataWriter_ptr foo_writer = FooDataWriter::_narrow(writer);
	Foo foo;
	DDS::InstanceHandle_t h = 0;
	for(int i=0;i<20;i++) {
		foo.a = (const char*)"foo.a";
		foo.b = (const char*)"foo.b";
		foo.c = i;
		foo.d = 20;
		foo_writer->write(foo, h);
	}

	printf("wrote 20 Foo data to DDS\n");
}

void MyDataWriterEmitterImpl::connect_bar_writer(DDS::DataWriter_ptr writer)
{
	BarDataWriter_ptr bar_writer = BarDataWriter::_narrow(writer);
	Bar bar;
	DDS::InstanceHandle_t h = 0;
	for(int i=0;i<20;i++) {
		bar.a = i;
		bar.b = 20;
		bar.c = "bar.c";
		bar.d = "bar.d";
		bar_writer->write(bar, h);
	}

	printf("wrote 20 Bar data to DDS\n");
}
