//#pragma comment(linker, "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") 
#include "Fierce3D.h"
#include <iostream>
#include <cstring>

Fierce3D::CF3DApp* app;

std::string* name = new std::string("F3D App Test 1");

Fierce3D::CF3DObject* objs[10];
Fierce3D::CF3DObject* children[10];
Fierce3D::CF3DVector3d v1;// , v2, v3;
Fierce3D::CF3DVector3d* v2;

//Fierce3D::CF3DVector3d v2;


template <typename T> void PrintVector(const T& v)
{
	std::cout << "  X: " << v._x << " Y: " << v._y << "    Z: " << v._z << std::endl;
}


int main()
{
	// Create a Simple Window with Vulkan Renderer
	Fierce3D::SVideoCreateStruct vcs;
	// setting hWnd to nullptr tells Fierce3D to create the main App Window itself.
	// We can give it an hWnd if we've created one already and want Fierce3D to render
	// to it instead.
	vcs.hWnd = nullptr;
	vcs.hInstance = nullptr;
	// Render window dimensions
	vcs.height = 768;
	vcs.width = 1024;
	// You can switch to OpenGL by changing vcs.renderer to Fierce3D::EF3DRenderer::OpenGL;
	vcs.renderer = Fierce3D::EF3DRenderer::Vulkan;

	// Create Fierce3D Instance
	app = new Fierce3D::CF3DApp(name->c_str(), name->c_str());

	// Have Fierce3D create a Render context using the above params.
	app->CreateRenderContext(vcs, true);

/*
	for (auto i = 0; i < 10; i++)
	{
		objs[i] = new Fierce3D::CF3DObject(app->getRootObject());
	}

	Fierce3D::CF3DObject* p = objs[3];
	for (auto i = 0; i < 10; i++)
	{
		children[i] = new Fierce3D::CF3DObject(p);
	}
	*/
	// print out each Objects ID
	for (auto i = 0; i < 10; i++)
		std::cout << "Object: " << i << " has " << objs[i]->GetChildCount() << " children." << std::endl;
	std::cout << "Vector Set" << std::endl << std::endl;
/*
	v1.Set(1.0f, 0.2f, 0.80f);
	auto dp = v1.GetDotProduct();// v2);
	std::cout << "V1 DotProduct: " << dp << std::endl;
	PrintVector(v1);

	v2 = new Fierce3D::CF3DVector3d(1.0f, 0.2f, 0.80f);
	auto dpPtr = v2->GetDotProduct();// v2);
	std::cout << "V2 DotProduct: " << dpPtr << std::endl;
	char c;
	std::cin >> c;
	*/
	return 0;
}
