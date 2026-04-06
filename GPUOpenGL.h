#pragma once
#if defined(_WIN64)

#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include "CF3DApp.h"
#include "CF3DBaseRenderer.h"

#if defined(_WIN64)
#include "PlatformWin64.h"
#endif


// Allow a maximum of two outstanding presentation operations.
constexpr uint32_t FRAME_LAG = 2;


class GPUOpenGL : public Fierce3D::CF3DBaseRenderer
{
public:
	int glMinor;
	int glMajor;

	GPUOpenGL(Fierce3D::CF3DApp* app);
	~GPUOpenGL();

	void InitializeGPU(Fierce3D::CF3DApp* _app) override;
	const std::string GetGPUDriverName();
	const std::string GetGPUVersion();
	void OnPostRender() override;
	void OnPreRender() override;
	void OnRender() override;
	void SetClearColor() override;
	void OnCleanup() override;

private:
	Fierce3D::CF3DApp* _app;

};
#pragma once
