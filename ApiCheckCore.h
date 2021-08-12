#pragma once

#include <cstdint>
#include <Windows.h>
#include <Windows.Foundation.Metadata.h>
#include <WRL/Client.h>
#include <WRL/Wrappers/CoreWrappers.h>


/**

	ApiCheck

	Core functionality, nothing more

	--------------------------------------------------

	See ApiCheck.h for usage examples and higher-level wrapper functions.

*/


namespace ApiCheck {


// ------------------------------------------------------------
// Constants and enums
// ------------------------------------------------------------


/**
	Represents a Windows OS version.
*/
enum class WindowsVersion : std::uint16_t
{
	/** Threshold 1 = version 1507 = build 10240 */
	TH1 = 1,
	/** Threshold 2 = version 1511 = build 10586 */
	TH2 = 2,
	/** Redstone 1 = Anniversary Update = version 1607 = build 14393 */
	RS1 = 3,
	/** Redstone 2 = Creators Update = version 1703 = build 15063, or 15254 for final feature2 build of Windows 10 Mobile */
	RS2 = 4,
	/** Redstone 3 = Fall Creators Update = version 1709 = build 16299 */
	RS3 = 5,
	/** Redstone 4 = April 2018 Update = version 1803 = build 17134 */
	RS4 = 6,
	/** Redstone 5 = October 2018 Update = version 1809 = build 17763 */
	RS5 = 7,
	/** 19H1 = May 2019 Update = version 1903 = build 18362 */
	CY19H1 = 8,
	// There's no UAP contract 9.
	/** 20H1 = May 2020 Update = version 2004 = build 19041 */
	CY20H1 = 10,

	None = 0xffff,
};


/**
	The minimum required version of this app as specified in the manifest.
	Checks for APIs that were introduced in a version after this can be skipped since the API will always exist.
*/
static constexpr WindowsVersion RequiredVersion = WindowsVersion::TH1;


// ------------------------------------------------------------
// Internal helpers
// ------------------------------------------------------------


template<WindowsVersion Version> static inline bool IsAtLeast() noexcept;


namespace Details {


static constexpr inline bool IsBeforeAppRequiredVersion(WindowsVersion other) noexcept
{
	return other != WindowsVersion::None && static_cast<std::uint16_t>(other) < static_cast<std::uint16_t>(RequiredVersion);
}


static constexpr inline WindowsVersion VersionBefore(WindowsVersion current) noexcept
{
	if (current == WindowsVersion::CY20H1)
		return WindowsVersion::CY19H1;
	else
		return static_cast<WindowsVersion>(static_cast<std::uint16_t>(current) - 1);
}


static constexpr inline WindowsVersion VersionAfter(WindowsVersion current) noexcept
{
	if (current == WindowsVersion::CY19H1)
		return WindowsVersion::CY20H1;
	else
		return static_cast<WindowsVersion>(static_cast<std::uint16_t>(current) + 1);
}


template<typename T>
static inline ::Microsoft::WRL::ComPtr<T> InternalGetActivationFactory(const wchar_t runtimeClass[]) noexcept
{
	::Microsoft::WRL::ComPtr<T> spActivationFactory;
	HRESULT hr = ::Windows::Foundation::GetActivationFactory(::Microsoft::WRL::Wrappers::HStringReference(runtimeClass).Get(), &spActivationFactory);
	if (!SUCCEEDED(hr))
		__fastfail(hr);
	return spActivationFactory;
}


static ::ABI::Windows::Foundation::Metadata::IApiInformationStatics& ApiInformation() noexcept
{
	static const ::Microsoft::WRL::ComPtr<::ABI::Windows::Foundation::Metadata::IApiInformationStatics> s_spApiInformationStatics{ InternalGetActivationFactory<::ABI::Windows::Foundation::Metadata::IApiInformationStatics>(RuntimeClass_Windows_Foundation_Metadata_ApiInformation) };
	return *s_spApiInformationStatics.Get();
}


static inline bool InternalGetIsApiContractPresent(const wchar_t szApiContract[], std::uint16_t apiContractVersion) noexcept
{
	boolean bResult = false;
	HRESULT hr = ApiInformation().IsApiContractPresentByMajor(::Microsoft::WRL::Wrappers::HStringReference(szApiContract).Get(), apiContractVersion, &bResult);
	return SUCCEEDED(hr) && bResult;
}


template<WindowsVersion IntroducedIn>
static inline bool CanAssumeApiIsPresentIfIntroducedIn() noexcept
{
	// If the user is already running a Windows version *after* the one that introduced this API, we don't need to check
	// and can simply assume it's present.  But we do need to check existence if it's the same version that introduced the
	// API, because it could be an earlier Insider build.
	return IntroducedIn != WindowsVersion::None && IsAtLeast<VersionAfter(IntroducedIn)>();
}


template<WindowsVersion IntroducedIn>
static inline bool CanAssumeApiIsNotPresentIfIntroducedIn() noexcept
{
	return IntroducedIn != WindowsVersion::None && !IsAtLeast<IntroducedIn>();
}


static inline bool InternalGetIsTypePresent(const wchar_t szRuntimeClass[]) noexcept
{
	boolean bResult = false;
	HRESULT hr = ApiInformation().IsTypePresent(::Microsoft::WRL::Wrappers::HStringReference(szRuntimeClass).Get(), &bResult);
	return SUCCEEDED(hr) && bResult;
}


static inline bool InternalGetIsPropertyPresent(const wchar_t szRuntimeClass[], const wchar_t szProperty[]) noexcept
{
	boolean bResult = false;
	HRESULT hr = ApiInformation().IsPropertyPresent(::Microsoft::WRL::Wrappers::HStringReference(szRuntimeClass).Get(), ::Microsoft::WRL::Wrappers::HStringReference(szProperty).Get(), &bResult);
	return SUCCEEDED(hr) && bResult;
}


static inline bool InternalGetIsMethodPresent(const wchar_t szRuntimeClass[], const wchar_t szMethod[]) noexcept
{
	boolean bResult = false;
	HRESULT hr = ApiInformation().IsMethodPresent(::Microsoft::WRL::Wrappers::HStringReference(szRuntimeClass).Get(), ::Microsoft::WRL::Wrappers::HStringReference(szMethod).Get(), &bResult);
	return SUCCEEDED(hr) && bResult;
}


static inline bool InternalGetIsEventPresent(const wchar_t szRuntimeClass[], const wchar_t szEvent[]) noexcept
{
	boolean bResult = false;
	HRESULT hr = ApiInformation().IsEventPresent(::Microsoft::WRL::Wrappers::HStringReference(szRuntimeClass).Get(), ::Microsoft::WRL::Wrappers::HStringReference(szEvent).Get(), &bResult);
	return SUCCEEDED(hr) && bResult;
}


static inline bool InternalGetIsEnumNamedValuePresent(const wchar_t szEnum[], const wchar_t szNamedValue[]) noexcept
{
	boolean bResult = false;
	HRESULT hr = ApiInformation().IsEnumNamedValuePresent(::Microsoft::WRL::Wrappers::HStringReference(szEnum).Get(), ::Microsoft::WRL::Wrappers::HStringReference(szNamedValue).Get(), &bResult);
	return SUCCEEDED(hr) && bResult;
}


}  // namespace Details


// ------------------------------------------------------------
// Direct Windows OS version checks
// ------------------------------------------------------------


/**
	Returns true if the app is running on the specified version of Windows or a later one.
	The result is cached, so checking the version a second time is extremely fast.
*/
template<WindowsVersion Version>
static inline bool IsAtLeast() noexcept
{
	static const bool s_isPresent{ Details::IsBeforeAppRequiredVersion(Version) || Details::InternalGetIsApiContractPresent(L"Windows.Foundation.UniversalApiContract", static_cast<std::uint16_t>(Version)) };
	return s_isPresent;
}


// ------------------------------------------------------------
// API existence checks
// ------------------------------------------------------------


/**
	Returns true if the specified class or other type is present in the current version of Windows.
	The result is cached, so checking the existence of the same API a second time is extremely fast.
	Supply the version of Windows that the API was introduced in, or WindowsVersion::None if it's not in the universal contract (extension APIs, private APIs, etc.).
*/
template<const wchar_t RuntimeClass[], WindowsVersion IntroducedIn>
static bool IsTypePresent() noexcept
{
	static const bool s_isPresent{ !Details::CanAssumeApiIsNotPresentIfIntroducedIn<IntroducedIn>() && (Details::CanAssumeApiIsPresentIfIntroducedIn<IntroducedIn>() || Details::InternalGetIsTypePresent(RuntimeClass)) };
	return s_isPresent;
}


/**
	Returns true if the specified class has a specified property in the current version of Windows.
	The result is cached, so checking the existence of the same API a second time is extremely fast.
	Supply the version of Windows that the API was introduced in, or WindowsVersion::None if it's not in the universal contract (extension APIs, private APIs, etc.).
*/
template<const wchar_t RuntimeClass[], const wchar_t Property[], WindowsVersion IntroducedIn>
static bool IsPropertyPresent() noexcept
{
	static const bool s_isPresent{ !Details::CanAssumeApiIsNotPresentIfIntroducedIn<IntroducedIn>() && (Details::CanAssumeApiIsPresentIfIntroducedIn<IntroducedIn>() || Details::InternalGetIsPropertyPresent(RuntimeClass, Property)) };
	return s_isPresent;
}


/**
	Returns true if the specified class has a specified method in the current version of Windows.
	The result is cached, so checking the existence of the same API a second time is extremely fast.
	Supply the version of Windows that the API was introduced in, or WindowsVersion::None if it's not in the universal contract (extension APIs, private APIs, etc.).
*/
template<const wchar_t RuntimeClass[], const wchar_t Method[], WindowsVersion IntroducedIn>
static bool IsMethodPresent() noexcept
{
	static const bool s_isPresent{ !Details::CanAssumeApiIsNotPresentIfIntroducedIn<IntroducedIn>() && (Details::CanAssumeApiIsPresentIfIntroducedIn<IntroducedIn>() || Details::InternalGetIsMethodPresent(RuntimeClass, Method)) };
	return s_isPresent;
}


/**
	Returns true if the specified class has a specified event in the current version of Windows.
	The result is cached, so checking the existence of the same API a second time is extremely fast.
	Supply the version of Windows that the API was introduced in, or WindowsVersion::None if it's not in the universal contract (extension APIs, private APIs, etc.).
*/
template<const wchar_t RuntimeClass[], const wchar_t Event[], WindowsVersion IntroducedIn>
static bool IsEventPresent() noexcept
{
	static const bool s_isPresent{ !Details::CanAssumeApiIsNotPresentIfIntroducedIn<IntroducedIn>() && (Details::CanAssumeApiIsPresentIfIntroducedIn<IntroducedIn>() || Details::InternalGetIsEventPresent(RuntimeClass, Event)) };
	return s_isPresent;
}


/**
	Returns true if the specified enum has a specified named value in the current version of Windows.
	The result is cached, so checking the existence of the same API a second time is extremely fast.
	Supply the version of Windows that the API was introduced in, or WindowsVersion::None if it's not in the universal contract (extension APIs, private APIs, etc.).
*/
template<const wchar_t Enum[], const wchar_t NamedValue[], WindowsVersion IntroducedIn>
static bool IsEnumNamedValuePresent() noexcept
{
	static const bool s_isPresent{ !Details::CanAssumeApiIsNotPresentIfIntroducedIn<IntroducedIn>() && (Details::CanAssumeApiIsPresentIfIntroducedIn<IntroducedIn>() || Details::InternalGetIsEnumNamedValuePresent(Enum, NamedValue)) };
	return s_isPresent;
}


} // namespace ApiCheck
