#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <list>
#include "Fierce3D.h"
#include "CF3DError.h"
#include "CF3DEventManager.h"


namespace Fierce3D
{
	constexpr int F3DOBJECT_ROOTID = 0x00000001;
	class F3DLIBRARY_API CF3DObject	: private CF3DEventManager 
	{
	public:
		CF3DObject();
		CF3DObject( CF3DObject* parent );
		~CF3DObject();

		// Every Object has a Heartbeat that's called by the timer
		virtual void OnHeartBeat();

		// Recurse children for an id - NULL if not found
		// Usefull for finding objects in a loaded model etc.
		bool		AddChild(CF3DObject* child);
		F3Did		GetChildCount();
		CF3DObject* FindChild( F3Did id );
		void		SetParent( CF3DObject* parent );
		void		RemoveChild( CF3DObject* child );
		void		SetAllowChildren(bool allow);
		bool		SetObjectType( EF3DOBJECT_TYPE newType );
		EF3DOBJECT_TYPE GetObjectType();

		// Get this Objects ID
		F3Did		GetID();

	protected:
		EF3DOBJECT_TYPE _objectType;
		bool		_allowsChildren;
		F3Did		_id;		// Object Unique ID
		const char* _name;		// human readable name
		CF3DObject* _parent;	// ptr to Parent
		F3DError   _error;		// Error
		std::list<CF3DObject*> * _childList;
	};

}
