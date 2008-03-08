/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _my_emitters_impl_h_
# define _my_emitters_impl_h_

#include "MyDataTypeDefs_c.hh"
#include "dds_dcps_c.hh"

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

class POCO_EXPORT MyDataWriterEmitterImpl {
  public:
	// method for IoC container to inject writers
	void connect_foo_writer(DDS::DataWriter_ptr);
	void connect_bar_writer(DDS::DataWriter_ptr);
};

#endif
