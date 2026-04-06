#include "Fierce3D.h"
#include "CF3DObject.h"
#include <atomic>

// rootNode is a ptr to the first CF3DObject created so we always have someone to look up to :)
static Fierce3D::CF3DObject* rootNode;
static bool rootMade = false;
static std::atomic<uint64_t> counter = 0;// F3DOBJECT_ROOTID;


static uint64_t generateUID() {
	return counter++;
}


namespace Fierce3D
{
	CF3DObject::CF3DObject()
	{
		_parent = nullptr;
		if (rootMade == false) {
			_id = F3DOBJECT_ROOTID;
			rootMade = true;
			rootNode = this;
		}
		else {
			_id = generateUID();
		}
		_allowsChildren = true;
		_childList = new std::list< CF3DObject* >();
		_childList->clear();
		if (_parent != nullptr)
			_parent->AddChild(this);

	}

	// NO NULL parents - Special case is the root node
	CF3DObject::CF3DObject(CF3DObject* parent) {
		if ((parent == nullptr) && (rootMade == true)) {
			throw std::runtime_error("CF3DObject needs a valid parent");
			//_error.Set( F3DERR_F3DOBJECT_CONSTRUCTOR_FAILED, false );
			return;
		}
		_parent = parent;
		if (rootMade == false) {
			_id = F3DOBJECT_ROOTID;
			rootMade = true;
			rootNode = this;
		}
		else {
			_id = generateUID();
		}
		_allowsChildren = true;
		_childList = new std::list< CF3DObject* >();
		_childList->clear();
		if (_parent != nullptr)
			_parent->AddChild(this);
	}

	// If for some reason an Object no longer is allowed children
	// we need to detach them all and reparent them to rootNode;
	void CF3DObject::SetAllowChildren(bool allow) {
		_allowsChildren = allow;
		if (!allow) {
			for (const auto& element : *_childList) {
				_childList->remove(element);
				element->SetParent(rootNode);
			}
		}
	}

	// Add a child Object to this Objects child list
	bool  CF3DObject::AddChild(CF3DObject* child) {
		if (_allowsChildren) {
			_childList->push_back(child);
			return true;
		}
		else
			return false;
	}

	// Find a child by id in the Objects child list
	CF3DObject* CF3DObject::FindChild(F3Did id) {
		for (const auto& element : *_childList) {
			if (id == element->_id)
				return this;
			else
				element->FindChild(id);
		}
		return nullptr;
	}

	// Reparent this Object to a new parent. Remove from old parents child list
	// and add it to the new parents children list
	void CF3DObject::SetParent(CF3DObject* parent) {
		// remove from old parents list
		_parent->RemoveChild(this);
		parent->AddChild(this);
		_parent = parent;
	}

	// Detach a child object and reparent it to rootNode
	void CF3DObject::RemoveChild(CF3DObject* child) {
		for (const auto& element : *_childList) {
			if (child == element) {
				_childList->remove(element);
				element->SetParent(rootNode);
			}
		}
	}

	// Return how many children this Object has
	F3Did CF3DObject::GetChildCount() {
		return std::distance(_childList->begin(), _childList->end());
	}

	// Return this Objects id
	F3Did CF3DObject::GetID() {
		return _id;
	}

	bool CF3DObject::SetObjectType(EF3DOBJECT_TYPE newType) {
		return true;
	}

	EF3DOBJECT_TYPE CF3DObject::GetObjectType() {
		return _objectType;
	}

	// Remove all children then delete the list
	CF3DObject::~CF3DObject() {
		for (const auto& element : *_childList) {
			delete(element);
		}
	}

	// Every Object has a Heartbeat that's called by the timer
	void CF3DObject::OnHeartBeat()
	{
		for (const auto& element : *_childList)
		{
			element->OnHeartBeat();
		}
	}

}
