#include "Fierce3D.h"
#include "CF3DEventManager.h"

namespace Fierce3D
{

	CF3DEventManager::CF3DEventManager()// Fierce3D::CF3DApp* app )
	{
		RegisteredEventsList.clear();
	}


	// Register an event the class wants to receive with the class Event Manager
	F3DError CF3DEventManager::RegisterEventHandler(int evtType, EvtFunctionPtr* ptr)
	{
		// Create a new CF3DEventHandler and add it to this classes Handlers list
		CF3DEventHandler* newHandler = new CF3DEventHandler();
		newHandler->evtType = evtType;				// Event type
		newHandler->funcPtr = ptr;
		RegisteredEventsList.push_back( newHandler );
		return F3D_OK;
	}


	// Remove an event the class no longer wants to receive from the class Event Manager
	F3DError CF3DEventManager::DeregisterEventHandler(EvtFunctionPtr* ptr)
	{
		for ( auto iter = RegisteredEventsList.begin(); iter != RegisteredEventsList.end(); iter++)
		{
			if ((*iter)->evtType)
				RegisteredEventsList.remove((*iter));
		}
		return F3D_OK;
	}

	F3DError CF3DEventManager::ReceiveEvent(CF3DEvent* evt)
	{
		return F3D_OK;
	}

	void CF3DEventManager::OnHeartbeat() 
	{
	}
	
	void CF3DEventManager::Sleep() 
	{
	}

	void CF3DEventManager::ProcessEvents()
	{

	}
	
	void CF3DEventManager::Wake() 
	{
	}
	
}
