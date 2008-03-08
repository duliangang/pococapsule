/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#include "ListenerImpl.h"
#include <iostream>

void MyFooDataReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader)
{
	DDS::SampleInfo info;
	FooDataReader_ptr foo_reader = FooDataReader::_narrow(reader);
	Foo foo;
	for(;foo_reader->take_next_sample(foo, info) == DDS::RETCODE_OK;) {
		std::cout << "received Foo data: {"
		<< "\"" << foo.a.in() << "\", "
		<< "\"" << foo.b.in() << "\", "
		<< foo.c << ", "
		<< foo.d << "}" << std::endl;
	}
}

void MyBarDataReaderListenerImpl::on_data_available(DDS::DataReader_ptr reader)
{
	DDS::SampleInfo info;
	BarDataReader_ptr bar_reader = BarDataReader::_narrow(reader);
	Bar bar;
	for(;bar_reader->take_next_sample(bar, info) == DDS::RETCODE_OK;) {
		std::cout << "received Bar data: {"
			<< bar.a << ", "
			<< bar.b << ", "
			<< "\"" << bar.c.in() << "\", "
			<< "\"" << bar.d.in() << "\"}" << std::endl;
	}
}
