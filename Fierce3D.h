// Fierce3D Engine main header file.
//
// Fierce3D Engine (C) 2024 Dave Smith, Otago, New Zealand
//
// May be used freelie, but not misrepresented. Any work or fork
// using the Fierce3D engine MUST (A) Give credit (B) Let me know! :)
//
// email fierce3d@outlook.com
//
#pragma once
#if defined(_WIN64)
#include <Windows.h>
#endif


#include <cstdint>

#ifndef F3DLIBRARY_EXPORTS
#pragma comment(lib,"Fierce3D.lib")
#ifndef _DEBUG
	#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") 
#endif
#endif

#ifndef NULL
#define NULL (void *)0
#endif

#define SAFE_RELEASE(ptr) { if ( (ptr) ) { (ptr)->Release(); (ptr) = 0; } }
#define SAFE_DELETE(ptr) { if( (ptr) != NULL ) delete (ptr); (ptr) = NULL; }

#ifdef F3DLIBRARY_EXPORTS
#define F3DLIBRARY_API __declspec(dllexport)
#else
#define F3DLIBRARY_API __declspec(dllimport)
#endif


namespace Fierce3D
{
	constexpr int F3D_OK = 0x00000000;
	constexpr int F3DERR_NONE = 0x00000000;
	constexpr int F3DERR_UNKNOWN = 0x00000001;
	constexpr int F3DERR_API_NOT_SUPPORTED = 0x00000002;
	constexpr int F3DERR_API_INVALID_ENUM = 0x00000003;
	constexpr int F3DERR_API_INVALID_VALUE = 0x00000004;
	constexpr int F3DERR_API_INVALID_OPERATION = 0x00000005;
	constexpr int F3DERR_API_STACK_OVERFLOW = 0x00000006;
	constexpr int F3DERR_API_STACK_UNDERFLOW = 0x00000007;
	constexpr int F3DERR_API_OUT_OF_MEMORY = 0x00000008;
	constexpr int F3DERR_API_INVALID_FRAMEBUFFER_OPERATION = 0x00000009;
	constexpr int F3DERR_API_CONTEXT_LOST = 0x00000010;
	constexpr int F3DERR_API_TABLE_TOO_LARGE = 0x00000011;
	constexpr int F3DERR_F3DOBJECT_CONSTRUCTOR_FAILED = 0x00000012;

	typedef uint64_t F3DEventType;
	typedef uint64_t F3Did;


	enum EF3DOBJECT_TYPE
	{
		F3DOBJECT_TYPE_EMPTY,

		F3DOBJECT_TYPE_CANVAS,
		F3DOBJECT_TYPE_CANVAS_BUTTON,
		F3DOBJECT_TYPE_CANVAS_TEXT,
		F3DOBJECT_TYPE_CANVAS_EDITBOX,
		F3DOBJECT_TYPE_CANVAS_IMAGE,

		F3DOBJECT_TYPE_MESH,
		F3DOBJECT_TYPE_CAMERA,
		F3DOBJECT_TYPE_LIGHT,
	};


	constexpr float F3D_PI = 3.1416f;

#define F3D_TO_RAD(deg) deg * F3D_PI/180
#define F3D_TO_DEG(rad) rad * 180 / F3D_PI

	constexpr int  F3D_EVT_FLAG_DEFAULT = 0x00000000;	// Default is to F3D_EVT_FLAG_POSTPROCESS
	constexpr int  F3D_EVT_FLAG_PREPROCESS = 0x00000001;	// Process the event BEFORE the builtin event handler for the object
	constexpr int  F3D_EVT_FLAG_POSTPROCESS = 0x00000002;	// Process event AFTER the builtin handler 
	constexpr int  F3D_EVT_FLAG_OVERRIDE = 0x00000003;	// Object to Process event by itself without using any builtin code


	// Window Events 60 - 70
	constexpr int  F3D_EVT_WIN_CREATE = 0x00000060;
	constexpr int  F3D_EVT_WIN_MOVE = 0x00000061;
	constexpr int  F3D_EVT_WIN_SIZE = 0x00000062;
	constexpr int  F3D_EVT_WIN_MINIMISE = 0x00000063;
	constexpr int  F3D_EVT_WIN_MAXIMISE = 0x00000064;
	constexpr int  F3D_EVT_WIN_RESTORE = 0x00000065;
	constexpr int  F3D_EVT_WIN_DESTROY = 0x00000066;

	// Create a Renderer Device link 35 - 59
	constexpr int  F3D_EVT_DEVICE3D_CREATE = 0x00000035;
	constexpr int  F3D_EVT_DEVICE3D_INITIALIZE = 0x00000036;
	constexpr int  F3D_EVT_DEVICE3D_SETTINGS_CHANGE = 0x00000037;	// Send a request to the Graphics device to change adapter settings, i.e screen res
	constexpr int  F3D_EVT_DEVICE3D_BUFFER_FLIP = 0x00000038;
	constexpr int  F3D_EVT_DEVICE3D_DESTROY = 0x00000059;

	// Timer Events 90 - 95
	constexpr int  F3D_EVT_TIMER_START = 0x00000090;
	constexpr int  F3D_EVT_TIMER_IDLE = 0x00000091;
	constexpr int  F3D_EVT_TIMER_LOOP = 0x00000092;
	constexpr int  F3D_EVT_TIMER_END = 0x00000093;
	constexpr int  F3D_EVT_TIMER_TRIGGERED = 0x00000094;
	constexpr int  F3D_EVT_TIMER_DESTROYED = 0x00000095;
	constexpr int  F3D_EVT_HEARTBEAT = 0x00000096;


	// Keyboard event
	constexpr int  F3D_EVT_KEYDOWN = 0x00000111;
	constexpr int  F3D_EVT_KEYUP = 0x00000112;

	// Mouse Event 140 - 144
	constexpr int  F3D_EVT_MOUSEMOVE = 0x00000140;
	constexpr int  F3D_EVT_MOUSEBUTTONDOWN = 0x00000141;
	constexpr int  F3D_EVT_MOUSEBUTTONUP = 0x00000142;
	constexpr int  F3D_EVT_MOUSEWHEEL = 0x00000143;
	constexpr int  F3D_EVT_MOUSESCROLLDOWN = 0x00000144;

	// Object Events 300 - 350
	constexpr int F3D_EVT_OBJECT_CREATE = 0x00000300;
	constexpr int F3D_EVT_OBJECT_MOVE = 0x00000301;		// Use if an object moves/rotates and you want to intercept it
	constexpr int F3D_EVT_OBJECT_HEARTBEAT = 0x00000302;			// a Heartbeat is called every frame on an object. Used for animation, collision detection etc
	constexpr int F3D_EVT_OBJECT_SLEEP = 0x00000303;		// If an object is put to sleep it's heartbeat function is not called
	constexpr int F3D_EVT_OBJECT_WAKE = 0x00000304;			// Wake an object up again to receive heartbeat calls
	constexpr int F3D_EVT_OBJECT_DESTROY = 0x00000350;

	// Rendering
	constexpr int F3D_EVT_RENDER_CREATE = 0x00000390;			// A render traget is being created
	constexpr int F3D_EVT_RENDER_BEGIN = 0x00000391;		// A Render target is about to begin rendering the frame
	constexpr int F3D_EVT_RENDER_END = 0x00000392;		// The Render target has finished rendering the frame
	constexpr int F3D_EVT_RENDER_PRESENT = 0x00000393;		// The Render target is now presenting the finished render
	constexpr int F3D_EVT_RENDER_SETTINGS_CHANGE = 0x00000394;			// The Render target is now presenting the finished render

	// Light Events
	constexpr int F3D_EVT_LIGHTON = 0x00000200;		// Light turning on
	constexpr int F3D_EVT_LIGHTOFF = 0x00000201;		// Light turning off
	constexpr int F3D_EVT_LIGHTMOVE = 0x00000202;		// Trigger that a light has moved or rotated
	constexpr int F3D_EVT_LIGHT_PARAM_UPDATE = 0x00000203;		// The lights paramaters has changed, i.e colour change
	constexpr int F3D_EVT_LIGHT_DESTROY = 0x00000204;		// A light is being destroyed

	// Object Events
	constexpr int F3D_OBJECT_CREATE = 0x00000700;
	constexpr int F3D_OBJECT_MOVE = 0x00000701;
	constexpr int F3D_OBJECT_SLEEP = 0x00000702;
	constexpr int F3D_OBJECT_WAKE = 0x00000703;
	constexpr int F3D_OBJECT_COLLISION = 0x00000704;
	constexpr int F3D_OBJECT_HEARTBEAT = 0x00000705;
	constexpr int F3D_OBJECT_DESTROY = 0x00000706;


	typedef int F3D_EVENTTYPE;
	typedef unsigned long long F3D_ID;	// an Object/Event ID
	typedef void* voidptr;
	typedef int* (*F3DFunctionPointer)(const int F3DEVT_TYPE, const void* Params, const int paramCnt);
	typedef int F3DError;

	class CF3DEvent; typedef F3DError(*EvtFunctionPtr)(CF3DEvent*);

	// define simple colors rgb, rgba
	class CF3DColour3b
	{
	public:
		char r, g, b;
		CF3DColour3b() { r = g = b = 0; }
		~CF3DColour3b() {};
	};

	class CF3DColour3f
	{
	public:
		float r, g, b;
		CF3DColour3f() { r = g = b = 0.0f; }
		~CF3DColour3f() {};
	};

	class CF3DColour4b
	{
	public:
		char r, g, b, a;
		CF3DColour4b() { r = g = b = a = 0; }
		~CF3DColour4b() {};
	};

	class CF3DColour4f
	{
	public:
		float r, g, b, a;
		CF3DColour4f() { r = g = b = a = 0.0f; }
		~CF3DColour4f() {};
	};

	class CF3DApp;
	class CF3DEventManager;
	class CF3DObject;
	class CF3DVector3f;
	class CF3DVector3d;
	class CF3DQuaternionf;
	//	class CF3DQuaterniond;


	enum EplatformOS
	{
		UnknownOS,
		Windows,
		Linux,
		Android
	};


	enum EplatformEnv
	{
		UnknownEnv,
		WinAPI,
		Wayland,
		X11
	};


	enum EplatformCPU
	{
		UnknownCPU,
		X64,
		ARM
	};

	
	enum EF3DRenderer {
		Software,
		OpenGL,
		OpenGLES,
		Vulkan,
		DirectX11,
		DirectX12
	};


	struct sPlatformInfo
	{
		EplatformOS		PlatformOS;
		EplatformCPU	PlatformCPU;
		EplatformEnv	PlatformEnv;
		unsigned long long	PlatformMemory;
		int PlatformMaxThreads;
	};

	struct SVideoCreateStruct 
	{
#if defined (_WIN64)	
		// We can allow the user to pass an existing HWND/HInstance to the Engine to use.
		// If these are nullptr then the Engine will make a default window.
		HWND hWnd;
		HINSTANCE hInstance;
#endif
#if defined (__linux__)
		// Pass an existing X11 Window
#endif
		uint32_t width;
		uint32_t height;
		uint32_t bits;
		EF3DRenderer renderer;
	};
}; // end namespace Fierce3D


#include <iostream>
#include <list>
#include <functional>
#include "CF3DBaseRenderer.h"
#include "CF3DEventManager.h"
#include "CF3DEventsServer.h"
#include "CF3DQuaternion.h"
#include "CF3DObject.h"
#include "CF3DVector3.h"
#include "CF3DApp.h"

