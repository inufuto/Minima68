#pragma once
#include "StatusWindow.h"
#include "../../../core/Cpu.h"

class AssemblyWindow : public StatusWindow
{
private:
	const Cpu* pCpu;
	const MemorySpace* pMemorySpace;
	int maxBytesPerLine;
	float lineHeight;
	float addressWidth;
	float valueWidth;
	float mnemonicWidth;
	float operandWidth;
	float spaceWidth;
	float marginX;
	int minWindowWidth;
	uint32_t topAddress;
	std::vector<AssemblyElement> elements;
protected:
	void OnCreate(CREATESTRUCT* pCreateStruct) override;
	void OnSize(UINT width, UINT height) override;
	void OnRender(class RenderTarget& renderTarget) override;
private:
	void BuildElements();
public:
	AssemblyWindow(const Cpu* pCpu, const MemorySpace* pMemorySpace, int maxBytesPerLine = 3) : pCpu(pCpu), pMemorySpace(pMemorySpace), maxBytesPerLine(maxBytesPerLine) {}
	auto MinWindowWidth() const { return minWindowWidth; }
	void UpdateList();
};
