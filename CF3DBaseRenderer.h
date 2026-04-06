#pragma once
#include "Fierce3D.h"

namespace Fierce3D
{
	// Define supported rendering APIs


	struct SGrfxSettings {

		EF3DRenderer _renderer;
	};

	//typedef <T> indiceType;


	class CF3DBaseRenderer {
	public:
		virtual ~CF3DBaseRenderer() = default;
		virtual void InitializeGPU( CF3DApp* _app ) {
			std::cout << "CF3DBaseRenderer Initialized." << std::endl << "Debug Console Output Started." << std::endl << std::endl;
		}
		virtual const std::string  GetGPUDriverName()
		{
			return ("1.0");
		}
		virtual const std::string  GetGPUVersion()
		{
			return ("1.0");
		}
		virtual void OnCleanup();
		virtual void OnPostRender();
		virtual void OnPreRender();
		virtual void OnRender();
		virtual void SetClearColor();
	//	virtual voidptr CreateMeshBuffer(<T> indiceType);
	};


	class RendererClass
	{
	public:
		std::unique_ptr<CF3DBaseRenderer> GPU_API_Ptr; // Pointer to BaseClass

	};

}
