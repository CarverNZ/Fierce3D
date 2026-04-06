#pragma once
#include "Fierce3D.h"
#include "CF3DEventManager.h"


// Fierce3D Server is the base for all event handling.
namespace Fierce3D
{
	class F3DLIBRARY_API CF3DEventsServer
	{
	public:
		CF3DEventsServer( CF3DApp* app );
		F3DError RegisterEventHandler( CF3DEventManager* classPtr );
		F3DError DeRegisterEventHandler( CF3DEventManager* classPtr );

	private:
		void OnHeartbeat();
		std::list< CF3DEventManager* > eventHandlers;
		CF3DApp* _app;
	};

}
