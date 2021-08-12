# ApiCheck

Writing Windows Runtime (WinRT) code in C++ that needs to run on older, downlevel versions of Windows, while still taking advantage of new system APIs if present, is a pain. This library makes it easy to check for APIs in the supported fashion with high performance. I've used code in production apps that have shipped to millions of users that's based on this library.

## Building and integrating

1. Copy `ApiCheck.cpp`, `ApiCheck.h`, and `ApiCheckCore.h` into your project.
2. (Optional) In `ApiCheckCore.h`, set `RequiredVersion` to the minimum version of Windows that your app supports.
2. Use the `ApiCheck::` functions accordingly.

**This library is in standard C++ and requires no additional frameworks beyond the Windows SDK.** Your project can use standard C++ (with WRL, for example), C++/CX, or C++/WinRT. As long as it links to the Windows Runtime and calls `RoInitialize`, it should work.

## Usage

Add the following to each of your source files that need it, or to your precompiled header:

```cpp
#include "ApiCheck.h"
```

### Checking for a Windows version

There are convenient helper functions for many versions of Windows:

```cpp
if (ApiCheck::IsAtLeastRS3()) { /* ... */ }
```

Alternately, you can use this more general form:

```cpp
if (ApiCheck::IsAtLeast<ApiCheck::WindowsVersion::RS3>()) { /* ... */ }
```

The supported values for `WindowsVersion` are in `ApiCheckCore.h`:

| `WindowsVersion` | means... |
|---|---|
| `TH1` | Threshold 1 = version 1507 = build 10240 |
| `TH2` | Threshold 2 = version 1511 = build 10586 |
| `RS1` | Redstone 1 = Anniversary Update = version 1607 = build 14393 |
| `RS2` | Redstone 2 = Creators Update = version 1703 = build 15063 (or 15254 for final feature2 build of Windows 10 Mobile) |
| `RS3` | Redstone 3 = Fall Creators Update = version 1709 = build 16299 |
| `RS4` | Redstone 4 = April 2018 Update = version 1803 = build 17134 |
| `RS5` | Redstone 5 = October 2018 Update = version 1809 = build 17763 |
| `CY19H1` | May 2019 Update = version 1903 = build 18362 |
| `CY20H1` | May 2020 Update = version 2004 = build 19041 |
| `None` | *default value* |

Hey, don't blame me for the absolute insanity of Windows version naming.

### Checking for a specific type

Instead of calling [`Windows.Foundation.Metadata.ApiInformation.IsTypePresent`](https://docs.microsoft.com/en-us/uwp/api/Windows.Foundation.Metadata.ApiInformation) directly, call `ApiCheck::IsTypePresent`. See `ApiCheck.cpp` for a few examples of this.

```cpp
if (ApiCheck::IsTypePresent<RuntimeClass_Windows_UI_Xaml_Media_RevealBrush,
    ApiCheck::WindowsVersion::RS3>())
{
    /* ... */
}
```

`IsTypePresent` takes two type parameters:

1. `RuntimeClass`: The RuntimeClass (full API name) of the API.
2. `IntroducedIn`: Optionally, the version of Windows that the API was introduced in, if it's in the universal contract.

The example above checks to see if the `RevealBrush` type is available. But, if the user is running a version of Windows after RS3 (aka Fall Creators Update, aka Version 1709, aka build 16299), then it knows that that check can be skipped. (If the type is part of an extension SDK and not the universal contract, don't include `IntroducedIn`. That's the case for APIs that are available only on, say, Windows Phone or HoloLens, such as `Windows.UI.ViewManagement.StatusBar`, which is not present on any desktop version of Windows.)

### Checking for a property, method, event, or enum value

ApiCheck also provides wrappers for `IsPropertyPresent`, `IsMethodPresent`, `IsEventPresent`, and `IsEnumNamedValuePresent`. These work the same as `IsTypePresent` except they take an additional type parameter.

```cpp
extern const wchar_t PreviewKeyDown[] = L"PreviewKeyDown";

if (ApiCheck::IsPropertyPresent<RuntimeClass_Windows_UI_Xaml_UIElement,
    PreviewKeyDown,
    ApiCheck::WindowsVersion::RS3>())
{
    /* ... */
}
```

These wrappers take three type parameters:

1. `RuntimeClass`: The RuntimeClass (full API name) of the type.
2. `Property` / `Method` / `Event` / `NamedValue` : The member of the type to look for.
3. `IntroducedIn`: Optionally, the version of Windows that the member was introduced in, if it's in the universal contract.

## Why is it fast? (Why is it normally slow?)

The supported way to check for the existence of an API is to use the [`Windows.Foundation.Metadata.ApiInformation`](https://docs.microsoft.com/en-us/uwp/api/Windows.Foundation.Metadata.ApiInformation) API. Under the hood, that requires looking up and instantiating the activation factory for `IApiInformationStatics` by name, and then calling the API, which does a string lookup of the API which requires loading metadata (.winmd) files.

This library:

* Is faster to code for, since it's just simple standard C++ function calls that just "do the right thing"
* Caches the activation factory for you
* Caches API existence checks for you so that they're only performed once
* Can often avoid checking for the API entirely, if:
    * The user is running a version of Windows that is guaranteed to always have the API
    * Your app's manifest dictates that the minimum version of Windows required to install the app already has the API

Under the hood, it uses [magic statics](https://docs.microsoft.com/en-us/cpp/cpp/storage-classes-cpp) and function templates. Each unique API checked adds about 83 bytes to your executable, and the cached checks only take about 1 μs on my machine. (In comparison, the small metadata file that Windows loads for an API check would easily be 500× that size or more.)

---
© 2021 Travis Spomer. [MIT license](License.txt).
