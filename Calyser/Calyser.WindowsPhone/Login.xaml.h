//
// Login.xaml.h
// Declaration of the Login class
//

#pragma once

#include "Login.g.h"

namespace Calyser
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class Login sealed
	{
	public:
		Login();
	private:
		void ContentDialog_PrimaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
		void ContentDialog_SecondaryButtonClick(Windows::UI::Xaml::Controls::ContentDialog^ sender, Windows::UI::Xaml::Controls::ContentDialogButtonClickEventArgs^ args);
	};
}
