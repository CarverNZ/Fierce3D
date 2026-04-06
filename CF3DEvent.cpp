#include "Fierce3D.h"
#include "CF3DEvent.h"

namespace Fierce3D
{
	CF3DEvent::CF3DEvent( F3DEventType evtType, F3D_ID sender, void* userData = nullptr, int userDataSize = 0, int flags = F3D_EVT_FLAG_DEFAULT )
	{
		this->evtType;
		this->sender;
		this->userData;
		this->userDataSize;
	}

	CF3DEvent::~CF3DEvent() 
	{
	}
};
