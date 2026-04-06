#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include "Fierce3D.h"

namespace Fierce3D
{
	class F3DLIBRARY_API CF3DError
	{
	public:
		CF3DError();
		CF3DError(int* code, bool critical);
		~CF3DError();
		void Set( int code, bool );
		void Set( int code );
		std::string ToStr();
		bool isCritical();
	protected:
		int* _code;
		bool _critical;
	};
}; // end namespace Fierce3D
