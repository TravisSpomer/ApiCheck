#pragma once
#include "ApiCheckCore.h"


/**

	ApiCheck

	These functions help you efficiently determine if a particular WinRT API is present.
	They can be used from pure C++, C++/CX, or C++/WinRT, and work on any Windows 10 version or later (but not Windows 8).

	ApiCheck.h includes higher-level helper functions for common Windows versions and APIs.
	ApiCheckCore.h includes lower-level helper functions for checking for custom APIs that don't have a higher-level helper function.

	--------------------------------------------------

	Example usage:

	Is this running on Windows RS5 or later?
		bool isRS5 = ApiCheck::IsAtLeastRS5();
			-OR-
		bool isRS5 = ApiCheck::IsAtLeast<ApiCheck::WindowsVersion::RS5>();

	Does this version of Windows include RevealBrush, which was introduced in RS3?
		bool isRevealSupported = ApiCheck::IsRevealBrushAvailable();
			-OR-
		bool isRevealSupported = ApiCheck::IsTypePresent<RuntimeClass_Windows_UI_Xaml_Media_RevealBrush, ApiCheck::WindowsVersion::RS3>();

	Does this version of Windows include a version of the RevealBrush type with a property named "FakeProperty"?
		extern const wchar_t FakeProperty[] = L"FakeProperty";
		bool isFakePropertySupported = ApiCheck::IsPropertyPresent<RuntimeClass_Windows_UI_Xaml_Media_RevealBrush, FakeProperty, ApiCheck::WindowsVersion::None>();

	Examples of all of those are present below.

*/


namespace ApiCheck {


// ------------------------------------------------------------
// Direct Windows OS version checks
// ------------------------------------------------------------


/**
	Returns true if the app is running on Windows RS1 or a later version.
	The result is cached, so checking the version a second time is extremely fast.
*/
static inline bool IsAtLeastRS1() noexcept
{
	return ApiCheck::IsAtLeast<ApiCheck::WindowsVersion::RS1>();
}


/**
	Returns true if the app is running on Windows RS2 or a later version.
	The result is cached, so checking the version a second time is extremely fast.
*/
static inline bool IsAtLeastRS2() noexcept
{
	return ApiCheck::IsAtLeast<ApiCheck::WindowsVersion::RS2>();
}


/**
	Returns true if the app is running on Windows RS3 or a later version.
	The result is cached, so checking the version a second time is extremely fast.
*/
static inline bool IsAtLeastRS3() noexcept
{
	return ApiCheck::IsAtLeast<ApiCheck::WindowsVersion::RS3>();
}


/**
	Returns true if the app is running on Windows RS4 or a later version.
	The result is cached, so checking the version a second time is extremely fast.
*/
static inline bool IsAtLeastRS4() noexcept
{
	return ApiCheck::IsAtLeast<ApiCheck::WindowsVersion::RS4>();
}


/**
	Returns true if the app is running on Windows RS5 or a later version.
	The result is cached, so checking the version a second time is extremely fast.
*/
static inline bool IsAtLeastRS5() noexcept
{
	return ApiCheck::IsAtLeast<ApiCheck::WindowsVersion::RS5>();
}


/**
	Returns true if the app is running on Windows 19H1 or a later version.
	The result is cached, so checking the version a second time is extremely fast.
*/
static inline bool IsAtLeast19H1() noexcept
{
	return ApiCheck::IsAtLeast<ApiCheck::WindowsVersion::CY19H1>();
}


/**
	Returns true if the app is running on Windows 20H1 or a later version.
	The result is cached, so checking the version a second time is extremely fast.
*/
static inline bool IsAtLeast20H1() noexcept
{
	return ApiCheck::IsAtLeast<ApiCheck::WindowsVersion::CY20H1>();
}


// ------------------------------------------------------------
// Specific common APIs
// ------------------------------------------------------------


/**
	Returns true if the Windows.UI.Xaml.UIElement.PreviewKeyDown and Up events are available.
	The result is cached, so checking for the API a second time is extremely fast.
*/
bool ArePreviewKeyEventsAvailable() noexcept;


/**
	Returns true if the Windows.UI.Composition.CompositionLinearGradientBrush API is available.
	The result is cached, so checking for the API a second time is extremely fast.
*/
bool IsCompositionLinearGradientBrushAvailable() noexcept;


/**
	Returns true if the Windows.UI.Xaml.Media.RevealBrush API is available.
	The result is cached, so checking for the API a second time is extremely fast.
*/
bool IsRevealBrushAvailable() noexcept;


/**
	Returns true if the deprecated Windows.UI.ViewManagement.StatusBar API is available.
	The result is cached, so checking for the API a second time is extremely fast.
*/
bool IsStatusBarAvailable() noexcept;


} // namespace ApiCheck
