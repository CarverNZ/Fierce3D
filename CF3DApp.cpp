#include "Fierce3D.h"
#include "PlatformWin64.h"
#include "GPUVulkan.h"
#include <memory>

std::mutex g_Mutex; // A global mutex to protect shared resources

#if defined( __linux__ )
#include <sys/sysinfo.h>
#endif


#if defined( __linux__ )
bool isRunningWayland()
{
	const char* xdgSessionType = std::getenv("XDG_SESSION_TYPE");
	if (xdgSessionType != nullptr && std::string(xdgSessionType) == "wayland")
	{
		return true;
	}
	const char* waylandDisplay = std::getenv("WAYLAND_DISPLAY");
	return waylandDisplay != nullptr;
}

bool isRunningX11()
{
	const char* xdgSessionType = std::getenv("XDG_SESSION_TYPE");
	if (xdgSessionType != nullptr && std::string(xdgSessionType) == "x11")
	{
		return true;
	}
	const char* display = std::getenv("DISPLAY");
	return display != nullptr;
}
#endif


namespace Fierce3D
{
	CF3DApp::CF3DApp(const char* appName, const char* winName) : _appName(appName), _winName(winName), _last_err(0)
	{

		// Create inst of Error class
		_appName = appName;
		_winName = winName;

		// Find out all the System information we might need.
		DiscoverSystemInformation();
		// If using console, lets let the user know what info we found.
#ifdef _DEBUG 
		std::cout << "Fierce3D DEBUG MODE" << std::endl;
		std::cout << "CPU: " << _sPlatformInfo->PlatformCPU << std::endl;
		std::cout << "Maximum Threads: " << _sPlatformInfo->PlatformMaxThreads << std::endl;
		std::cout << "Memory: " << _sPlatformInfo->PlatformMemory << std::endl;
		std::cout << "OS: " << _sPlatformInfo->PlatformOS << std::endl << std::endl;
#endif

		// Create an Event Server
		_appEventServer = new CF3DEventsServer(this);
		// seed the randon number generator
		std::srand(static_cast<unsigned int>(std::time(nullptr)));

		// Create The Object Manager that manages all Objects
		_rootObject = new CF3DObject(NULL);
	}


	void CF3DApp::DiscoverSystemInformation()
	{
		// Store Operating System and Environment
		_sPlatformInfo = std::make_unique<sPlatformInfo>();
		_sPlatformInfo->PlatformCPU = UnknownCPU;
		_sPlatformInfo->PlatformOS = UnknownOS;
		_sPlatformInfo->PlatformEnv = UnknownEnv;
		_sPlatformInfo->PlatformMemory = 0;
		_sPlatformInfo->PlatformMaxThreads = getAvailableThreads();
#if defined( _WIN64 )
		_sPlatformInfo->PlatformOS = Windows;
		_sPlatformInfo->PlatformEnv = WinAPI;
#endif
#if defined( __linux__ )
		_sPlatformInfo->PlatformOS = Linux;
		if (isRunningWayland())
			eplatformEnv = Wayland)
		else
			eplatformEnv = X11;
#endif
		// Get System Physical Memory
#if defined(_WIN64)
		MEMORYSTATUSEX memoryStatus;
		memoryStatus.dwLength = sizeof(memoryStatus);
		// Get system memory information
		if (GlobalMemoryStatusEx(&memoryStatus))
			_sPlatformInfo->PlatformMemory = memoryStatus.ullTotalPhys / (1024 * 1024);
#endif
#if defined(_linux_)
		struct sysinfo memInfo;
		sysinfo(&memInfo);
		unsigned long totalMemory = memInfo.totalram;  // Total RAM in bytes
		totalMemory *= memInfo.mem_unit;               // Convert to bytes
		_sPlatformInfo->PlatformMemory = totalMemory / (1024 * 1024);	// Convert to MB		
#endif
	}

	F3DError CF3DApp::CreateRenderContext(const SVideoCreateStruct& VCStruct, const bool show)
	{
		this->VCStruct = VCStruct;
		//Fierce3D::CF3DApp* app, const char* appName, const char* winName, Fierce3D::SVideoCreateStruct& VCStruct, bool show
#if defined(_WIN64)
		CreateWin64Window(this, _appName.c_str(), _winName.c_str(), VCStruct, show);
		this->VCStruct.hWnd = GetWindowHandle();
		this->VCStruct.hInstance = GetInstance();
#endif

#if defined( __linux__ )
		if (isRunningWayland()
#endif

			switch (VCStruct.renderer)
			{
			case EF3DRenderer::OpenGL:
			{
				break;
			};
			case EF3DRenderer::Vulkan:
			{
				Renderer = std::make_unique<GPUVulkan>(this);
				break;
			};
			default:
				Renderer = std::make_unique<CF3DBaseRenderer>();
				break;
			}
		Renderer->InitializeGPU(this);

		return 0;
	}


	const SVideoCreateStruct* CF3DApp::GetVideoCreateInfo()
	{
		return &VCStruct;
	}


	sPlatformInfo* CF3DApp::GetSystemInformation()
	{
		return this->_sPlatformInfo.get();
	}


	CF3DObject* CF3DApp::getRootObject()
	{
		return _rootObject;
	}


	void CF3DApp::OnHeartbeat()
	{
		// go through each object and have it process any events needing dealt with
		_rootObject->OnHeartBeat();
	}

	// Get available thread count the system can give the App.
	int CF3DApp::getAvailableThreads()
	{
		return std::thread::hardware_concurrency();
	}

	/*
	CEventManager* CreateEventManager()
	{
		return new CEventManager();
	}
	*/

	Fierce3D::CF3DApp::~CF3DApp()
	{
	}


} // namespace
