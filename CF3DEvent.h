#pragma once
#include "Fierce3D.h"


namespace Fierce3D
{
	class CF3DEvent;

	// userData can be any format of information required by the receiving class.
	// an example of this would be a mouse handler that stored the following data
	// button states, x/y coordinates, mouse enter/exits, mouse move etc.
	// which may be stored such as

	// struct MOUSE_EVT {
	//	int button_states;
	//  int x_loc, y_loc;
	// };

	// userData = &MouseEvtData	
	// userDataSize = sizeof( MOUSE_EVT )


	class F3DLIBRARY_API CF3DEvent {
	public:
		CF3DEvent( F3DEventType evtType, F3D_ID sender, void* userData , int userDataSize, int flags );
		~CF3DEvent();

	private:
		F3DEventType evtType;
		F3D_ID sender;
		void* userData;
		int userDataSize;
	};

};
