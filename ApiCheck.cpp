#include "ApiCheck.h"
#include <Windows.UI.Composition.h>
#include <Windows.UI.Xaml.Automation.h>
#include <Windows.UI.Xaml.Controls.h>
#include <Windows.UI.Xaml.Hosting.h>
#include <Windows.UI.Xaml.Media.h>
#include <Windows.UI.ViewManagement.h>


namespace ApiCheck {


namespace Details { namespace Strings {
	extern const wchar_t PreviewKeyDown[] = L"PreviewKeyDown";
}} // namespace Details::Strings


bool ArePreviewKeyEventsAvailable() noexcept
{
	return IsPropertyPresent<RuntimeClass_Windows_UI_Xaml_UIElement, Details::Strings::PreviewKeyDown, WindowsVersion::RS3>();
}


bool IsCompositionLinearGradientBrushAvailable() noexcept
{
	return IsTypePresent<RuntimeClass_Windows_UI_Composition_CompositionLinearGradientBrush, WindowsVersion::RS3>();
}


bool IsRevealBrushAvailable() noexcept
{
	return IsTypePresent<RuntimeClass_Windows_UI_Xaml_Media_RevealBrush, WindowsVersion::RS3>();
}


bool IsStatusBarAvailable() noexcept
{
	return IsTypePresent<RuntimeClass_Windows_UI_ViewManagement_StatusBar, WindowsVersion::None>();
}


// ------------------------------------------------------------
} // namespace ApiCheck
