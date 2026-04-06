#pragma once
#include "Fierce3D.h"
//#include "CF3DError.h"	// error storage
//#include "CF3DObject.h"
//#include "CF3DEventsServer.h"

#include <thread> // Required for std::thread
#include <chrono> // Required for std::chrono::seconds
#include <mutex>  // Required for std::mutex (for thread safety)
#include <list>


namespace Fierce3D
{

	class F3DLIBRARY_API CF3DApp
	{
	public:

		using FuncPtr = std::function<void()>;

		CF3DApp( const char* appName, const char* winName );
		~CF3DApp();

		F3DError CreateRenderContext( const SVideoCreateStruct& VCStruct, const bool show );
		CF3DObject* getRootObject();
		const SVideoCreateStruct* GetVideoCreateInfo();

		int getAvailableThreads();
	private:
		
		::std::string _appName;
		::std::string _winName;

//		CF3DBaseRenderer BaseRenderer;
		std::unique_ptr<CF3DBaseRenderer> Renderer;

		CF3DObject* _rootObject;

		F3DError  _last_err = 0;
		void OnHeartbeat();

		CF3DEventsServer* _appEventServer;

		void DiscoverSystemInformation();

		std::list<std::thread> threads;

	protected:
		//sPlatformInfo* _sPlatformInfo;
		SVideoCreateStruct VCStruct;
		std::unique_ptr<Fierce3D::sPlatformInfo> _sPlatformInfo;
		sPlatformInfo* GetSystemInformation();
	};
}; // end of namespace Fierce3D

extern std::mutex g_Mutex; // A global mutex to protect shared resources
