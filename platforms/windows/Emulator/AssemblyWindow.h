#pragma once
#include "StatusWindow.h"
#include "../../../core/Cpu.h"

class AssemblyWindow : public StatusWindow
{
private:
	const Cpu* pCpu;
	const MemorySpace* pMemorySpace;
	float lineHeight;
	float addressWidth;
	float valueWidth;
	float mnemonicWidth;
	float operandWidth;
	float spaceWidth;
	float marginX;
	int minWindowWidth;
	uint32_t topAddress;
	std::vector<std::unique_ptr<AssemblyElement>> elements;
protected:
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnSize(UINT width, UINT height) override;
	void OnRender(class RenderTarget& renderTarget) override;
private:
	void BuildElements();
public:
	AssemblyWindow(const Cpu* pCpu, const MemorySpace* pMemorySpace) : pCpu(pCpu), pMemorySpace(pMemorySpace) {}
	auto MinWindowWidth() const { return minWindowWidth; }
	void UpdateList();
};
