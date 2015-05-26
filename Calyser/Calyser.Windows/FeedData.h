#pragma once
#include "pch.h"
#include "FileItem.h"

using namespace Platform;
using namespace Windows::UI::Xaml::Data;

namespace Calyser {

	/// <summary>
	/// A FeedData object represents a feed that contains 
	/// one or more FeedItems. 
	/// </summary>
	//		[Windows::UI::Xaml::Data::Bindable]
	public ref class FeedData sealed
	{
	public:
		FeedData(void)
		{
			m_items = ref new Platform::Collections::Vector<FileItem^>();
		}

		// The public members must be Windows Runtime types so that
		// the XAML controls can bind to them from a separate .winmd.
		property Platform::String^ Title;

		property Platform::String^ Description;
		property Windows::Foundation::DateTime PubDate;
		property Platform::String^ Uri;


		Windows::Foundation::Collections::IVector<FileItem^>^ get() {
			return m_items;
		};


	private:
		~FeedData(void){}
		
		Platform::Collections::Vector<FileItem^>^ m_items;
	};

}