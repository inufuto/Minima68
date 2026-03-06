#include "AssemblyWindow.h"

void AssemblyWindow::OnCreate(CREATESTRUCT* pCreateStruct)
{
	StatusWindow::OnCreate(pCreateStruct);
	auto charSize = CharSize();
	lineHeight = charSize.height;
	marginX = 4.0f;
	addressWidth = 4 * charSize.width;
	valueWidth = 2 * charSize.width;
	mnemonicWidth = 4 * charSize.width;
	operandWidth = 8 * charSize.width;
	spaceWidth = charSize.width;
	minWindowWidth = DipToPixel(
		marginX * 2 + addressWidth + spaceWidth + (valueWidth + spaceWidth) * maxBytesPerLine + mnemonicWidth + spaceWidth +
		operandWidth);
	topAddress = 0;
}

void AssemblyWindow::OnSize(UINT width, UINT height)
{
	StatusWindow::OnSize(width, height);
	BuildElements();
}

void AssemblyWindow::OnRender(class ::RenderTarget& renderTarget)
{
	StatusWindow::OnRender(renderTarget);
	auto address = topAddress;
	auto size = renderTarget.GetSize();
	D2D1_RECT_F rect;
	rect.top = 0.0f;
	for (const auto& pElement : elements) {
		rect.bottom = rect.top + lineHeight;
		if (rect.top >= size.height) {
			break;
		}
		rect.left = marginX;
		rect.right = rect.left + addressWidth;
		char addressText[4 + 1];
		sprintf_s(addressText, sizeof(addressText), "%04X", address);
		renderTarget.DrawText(addressText, TextFormat(), rect, TextBrush());
		rect.left = rect.right + spaceWidth;
		for (int i = 0; i < maxBytesPerLine; ++i) {
			rect.right = rect.left + valueWidth;
			if (i < pElement->size) {
				auto value = pMemorySpace->Read(address++);
				char valueText[2 + 1];
				sprintf_s(valueText, sizeof(valueText), "%02X", value);
				renderTarget.DrawText(valueText, TextFormat(), rect, TextBrush());
			}
			rect.left = rect.right + spaceWidth;
		}
		rect.right = rect.left + mnemonicWidth;
		renderTarget.DrawText(pElement->mnemonic.c_str(), TextFormat(), rect, TextBrush());
		rect.left = rect.right + spaceWidth;
		rect.right = rect.left + operandWidth;
		renderTarget.DrawText(pElement->operand.c_str(), TextFormat(), rect, TextBrush());
		rect.top = rect.bottom;
	}
}

void AssemblyWindow::BuildElements()
{
	auto size = RenderTarget()->GetSize();
	auto count = static_cast<int>(size.height / lineHeight);
	elements.clear();
	topAddress = pCpu->LastInstructionAddress();
	auto address = topAddress;
	for (int i = 0; i < count; ++i) {
		auto pElement = std::make_unique<AssemblyElement>();
		pCpu->BuildAssemblyElement(address, pElement.get());
		elements.push_back(std::move(pElement));
		address += elements.back()->size;
	}
}

void AssemblyWindow::UpdateList()
{
	auto size = RenderTarget()->GetSize();
	auto count = static_cast<int>(size.height / lineHeight);
	if (pCpu->LastInstructionAddress() != topAddress || count != static_cast<int>(elements.size())) {
		BuildElements();
		Invalidate();
	}
}
