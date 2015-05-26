#pragma once

using namespace Platform;
using namespace Windows::UI::Xaml::Data;

namespace Calyser
{

//		[Windows::UI::Xaml::Data::Bindable] // in c++, adding this attribute to ref classes enables data binding for more info search for 'Bindable' on the page http://go.microsoft.com/fwlink/?LinkId=254639
		public ref class FileItem sealed 
		{
		public:
			property Platform::String^ Title;
			property Platform::String^ Subtitle;

		private:
			Platform::String^ _title;
			Platform::String^ _subtitle;
		};

	}