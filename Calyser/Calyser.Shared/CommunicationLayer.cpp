#include "pch.h"
#include "CommunicationLayer.h"
#include "pch.h"
#include <atomic>
#include <collection.h>
#include <ppltasks.h>
#include <concurrent_vector.h>
#include <sqlite3.h>

using namespace concurrency;
using namespace std;
using namespace CommunicationLayerN;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Networking::Sockets;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

StorageFolder^ localFolder = ApplicationData::Current->LocalFolder;

CommunicationLayer::CommunicationLayer()
{
}

//

void CommunicationLayer::ReadDB(){
	//
	concurrency::task<StorageFile^> getFileOperation(localFolder->GetFileAsync("Calyser.db"));
	getFileOperation.then([this](StorageFile^ file)
	{
		return FileIO::ReadTextAsync(file);
	}).then([this](concurrency::task<String^> previousOperation) {
		
	
	
		}
	);
//	
}

void CommunicationLayer::OnConnectionReceived(StreamSocketListener ^sender, StreamSocketListenerConnectionReceivedEventArgs ^args)
{
	//
	StreamSocket ^CalyserSocket = args->Socket;
	//
	std::string output = "Inside OnConectionReceived ";
	output.append("\n");
	OutputDebugStringA(output.c_str());
	/*
	concurrency::create_async([CalyserSocket]()
	{
		IInputStream  ^CalyserInputStream  = CalyserSocket->InputStream;
		IOutputStream ^CalyserOutputStream = CalyserSocket->OutputStream;
	}
		//
	);
	*/
};

IAsyncOperationWithProgress<IVector<int>^, double>^ CommunicationLayer::Runlistener (int port) 
{
	return create_async([this, port](progress_reporter<double> reporter) -> IVector<int>^
	{
		//
		std::string output = "Started Listener on port ";
		output.append(to_string(port));
		output.append("\n");
		OutputDebugStringA(output.c_str());
		//
		StreamSocketListener^ Listener = ref new StreamSocketListener;
		//
		Listener->ConnectionReceived += ref new Windows::Foundation::TypedEventHandler<Windows::Networking::Sockets::StreamSocketListener ^, Windows::Networking::Sockets::StreamSocketListenerConnectionReceivedEventArgs ^>(this, &CommunicationLayerN::CommunicationLayer::OnConnectionReceived);
		//
		
		auto m_BindService = Listener->BindServiceNameAsync("8080");
		//
//		Listener->ConnectionReceived += ref new Windows::Foundation::TypedEventHandler<Windows::Networking::Sockets::StreamSocketListener ^, Windows::Networking::Sockets::StreamSocketListenerConnectionReceivedEventArgs ^>(this, &CommunicationLayerN::CommunicationLayer::OnConnectionReceived);
		//
		output.clear();
		output = "Biding error code ";
		output.append(to_string(m_BindService->ErrorCode.Value));
	
		output.append("\n");
		OutputDebugStringW(((Listener->Information->LocalPort)->Data()));
		//
		//Listener->ConnectionReceived += ref new Windows::Foundation::TypedEventHandler<Windows::Networking::Sockets::StreamSocketListener ^, Windows::Networking::Sockets::StreamSocketListenerConnectionReceivedEventArgs ^>(this, &CommunicationLayerN::CommunicationLayer::OnConnectionReceived);
		//
		float m_calc = 0.;
		while (1>0)
		{
			m_calc = m_calc + 0.01;
			/*
				output.clear();
				output = "Stopped Listener on port ";
				output.append(to_string(m_calc));
				output.append("\n");
				OutputDebugStringA(output.c_str());
				*/
		}
		//
		output.clear();
		output = "Stopped Listener on port ";
		output.append(to_string(port));
		output.append("\n");
		OutputDebugStringA(output.c_str());
		//
		// Perform the computation in parallel.
		concurrent_vector<int> primes;
		//
		reporter.report(100.0);
		//
		// Copy the results to an IVector object. The IVector  
		// interface makes collections of data available to other  
		// Windows Runtime components.
		auto results = ref new Vector<int>();
		for (int prime : primes)
		{
			results->Append(prime);
		}
		return results;
	});

}


