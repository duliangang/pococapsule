/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _my_listener_impl_h
# define _my_listener_impl_h

#include "MyDataTypeDefs_c.hh"
#include "dds_dcps_c.hh"

#include <pocodefs.h> // for the macro POCO_EXPORT -- to export on Windows

//
// Typed event listener. 
//
class MyDataReaderListenerImpl : public DDS::DataReaderListener {
  public:
     void on_requested_deadline_missed(DDS::DataReader_ptr, const DDS::RequestedDeadlineMissedStatus&) {}
     void on_requested_incompatible_qos(DDS::DataReader_ptr, const DDS::RequestedIncompatibleQosStatus&) {}
     void on_sample_rejected(DDS::DataReader_ptr, const DDS::SampleRejectedStatus&) {} 
     void on_liveliness_changed(DDS::DataReader_ptr, const DDS::LivelinessChangedStatus&) {}
     void on_subscription_matched(DDS::DataReader_ptr, const DDS::SubscriptionMatchedStatus&) {}
     void on_sample_lost(DDS::DataReader_ptr, const DDS::SampleLostStatus&) {}
};

class POCO_EXPORT MyFooDataReaderListenerImpl : public MyDataReaderListenerImpl {
  public:
     void on_data_available(DDS::DataReader_ptr reader);
};

class POCO_EXPORT MyBarDataReaderListenerImpl : public MyDataReaderListenerImpl {
  public:
     void on_data_available(DDS::DataReader_ptr reader);
};

#endif
