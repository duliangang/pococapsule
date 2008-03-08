/****************************************************************************/
/*									    */
/*  Copyright 2006, by Pocomatic Software, LLC.	All Rights Reserved.        */
/*									    */
/****************************************************************************/
#ifndef _Interfaces_h_
# define _Interfaces_h_

class EventListener {
  public:
	virtual ~EventListener() {}
	virtual void refresh() = 0;
};

class EventEmitter {
  public:
	virtual ~EventEmitter() {}
	virtual void subscribe(EventListener*) = 0;
	virtual void unsubscribe(EventListener*) = 0;
};

class TickGen : public EventEmitter
{
  public:
	virtual void start() = 0;
};

//
// multiple inheritance !!
//
class GPSLocator : public EventEmitter, public EventListener
{
  public:
	virtual int get_pos_x() = 0;
	virtual int get_pos_y() = 0;
};

class NavDisplay : public EventListener 
{
  public:
};

#endif
