#include "Fierce3D.h"
#include "GPUOpenGL.h"
#include "CF3DApp.h"


GPUOpenGL::GPUOpenGL(Fierce3D::CF3DApp* app)
{
	if ( app == nullptr )
		throw std::invalid_argument("Invalid Fierce3D Application call to OpenGL");
	this->_app = app;
}

GPUOpenGL::~GPUOpenGL()
{
}

void GPUOpenGL::InitializeGPU(Fierce3D::CF3DApp* _app)
{
}

const std::string GPUOpenGL::GetGPUDriverName()
{
	return std::string("Fierce3D OpenGL Driver.  Version ");
}

const std::string GPUOpenGL::GetGPUVersion()
{
	return std::string("n/a");
}

void GPUOpenGL::OnPostRender() {}

void GPUOpenGL::OnPreRender() {}

void GPUOpenGL::OnRender() {}


void GPUOpenGL::SetClearColor() {}


void GPUOpenGL::OnCleanup() {}

