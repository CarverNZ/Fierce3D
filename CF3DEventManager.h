#pragma once

// Fierce3D Event Manager
//
// Fierce3D Engine (C) 2024 Dave Smith, Otago, New Zealand
//
// May be used freelie, but not misrepresented. Any work or fork
// using the Fierce3D engine MUST (A) Give credit (B) Let me know! :)
//
// email fierce3d@outlook.com
//
// An EventManager is created to handle different events for a particular object, either physical or virtual.
//
// Examples of physical EventManagers are keyboard and Mouse handlers.
// Examples of virtual EventManagers are Object creation, collisions, movement
#pragma once

#include "Fierce3D.h"
#include <iostream>
#include <list>

#pragma warning(disable : 4251)

namespace Fierce3D
{

	// Every event has a function associated with it that is set by either
	// the base class OR a deriving class.
	// To set an Events "go to" function call RegisterEventHandler.
	// An example of this would be to have a function called CF3DError OnMouseMove, so
	// RegisterEventHandler( EVT_MOUSEMOVE, *OnMouseMOve );

	class CF3DEventHandler
	{
	public:
		int evtType;				// Event type
		EvtFunctionPtr* funcPtr;
	};


	class F3DLIBRARY_API CF3DEventManager
	{
	public:
		CF3DEventManager();
		F3DError RegisterEventHandler( int evtType, EvtFunctionPtr* ptr );
		F3DError DeregisterEventHandler( EvtFunctionPtr* ptr );
		F3DError ReceiveEvent( CF3DEvent* evt );
		void OnHeartbeat();
		void Sleep();
		void Wake();

	private:
		::std::list< CF3DEventHandler* > RegisteredEventsList;
		void ProcessEvents();
	};
}
