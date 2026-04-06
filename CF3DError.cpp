#include "Fierce3D.h"
#include "CF3DError.h"

/*
* Fierce3D errors are handled in a class. When an error occurs a Flag is raised and a
* reference number set.
* Developers can use ToStr() to return an descriptor string.
*/

namespace Fierce3D
{
	CF3DError::CF3DError() {
		_code = new (int);
		_critical = false;
	}

	CF3DError::CF3DError(int* code, bool critical) : _code(code), _critical(critical) {
	}


	CF3DError::~CF3DError() {
		if (_code != NULL )
			delete(_code);
		_code = (0);
	}

	// Set the critical error code and error flag
	void CF3DError::Set(int code, bool critical ) {
		*this->_code = code;
		this->_critical = critical;
	}
	
	// Set the critical error code
	void CF3DError::Set(int code ) {
		*this->_code = code;
		this->_critical = false;
	}

	// Return an English string descriptor
	std::string CF3DError::ToStr() {
		std::string str;
		switch (*this->_code) {
			case 0x00000000: str = std::string("F3DERR_NONE"); break;
			case 0x00000001: str = std::string("F3DERR_UNKNOWN"); break;
			// OpenGL API Error support
			case 0x00000002: str = std::string("F3DERR_API_NOT_SUPPORTED"); break;
			case 0x00000004: str = std::string("F3DERR_API_INVALID_ENUM"); break;
			case 0x00000008: str = std::string("F3DERR_API_INVALID_VALUE"); break;
			case 0x00000016: str = std::string("F3DERR_API_INVALID_OPERATION"); break;
			case 0x00000032: str = std::string("F3DERR_API_STACK_OVERFLOW"); break;
			case 0x00000064: str = std::string("F3DERR_API_STACK_UNDERFLOW"); break;
			case 0x00000128: str = std::string("F3DERR_API_OUT_OF_MEMORY"); break;
			case 0x000000256: str = std::string("F3DERR_API_INVALID_FRAMEBUFFER_OPERATION"); break;
			case 0x000000512: str = std::string("F3DERR_API_CONTEXT_LOST"); break;
			case 0x000001024: str = std::string("F3DERR_API_TABLE_TOO_LARGE"); break;
			// Vulkan API Error support
			case 0x000002048: str = std::string("F3DERR_API_TABLE_TOO_LARGE"); break;

			// Misc system Error support
			default:
				str = std::string("F3DERR_UNKNOWN"); break;
		} 
		return str;
	}


	bool CF3DError::isCritical() {
		return this->_critical;
	}

}; // end of namespace Fierce3D

