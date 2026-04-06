#include "Fierce3D.h"
#include "CF3DEventsServer.h"


namespace Fierce3D
{
	CF3DEventsServer::CF3DEventsServer(CF3DApp* app) :_app(app)
	{
	}


	F3DError CF3DEventsServer::RegisterEventHandler( CF3DEventManager* classPtr )
	{
		eventHandlers.push_back(classPtr);
		return F3D_OK;
	}


	F3DError CF3DEventsServer::DeRegisterEventHandler(CF3DEventManager* classPtr)
	{
		eventHandlers.remove(classPtr);
		return F3D_OK;
	}


	void CF3DEventsServer::OnHeartbeat()
	{

	}
}
