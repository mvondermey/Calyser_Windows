#include "pch.h"
#include "CommunicationLayer.h"
#include "pch.h"
#include <atomic>
#include <collection.h>
#include <ppltasks.h>
#include <concurrent_vector.h>
#include <string.h>
#include <sqlite3.h>
#include <string>


using namespace concurrency;
using namespace std;
using namespace CommLayer;

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
};

int CommunicationLayer::CheckLogin(String^ login, String^ password) {

	OutputDebugStringW(L" Email ");
	OutputDebugStringW((login)->ToString()->Data());
	OutputDebugStringW(L"\n");
	OutputDebugStringW(L" Pasword ");
	OutputDebugStringW((password)->ToString()->Data());
	OutputDebugStringW(L"\n");
	//
	sqlite3 *db = NULL;
	//
	wstring FilePath = (localFolder->Path->Data());
	string FilePathShort(FilePath.begin(), FilePath.end());
	FilePathShort += "\\Caliser.db";
	//
	OutputDebugStringW(L" File location 1\n");
	OutputDebugStringW(localFolder->Path->Data());
	OutputDebugStringW(L"\n");
	//
	//char * FilePath = ((localFolder->Path) + "Calyser.db")->Data();
	//
	int result_conn = sqlite3_open(FilePathShort.c_str(), &db);
	//
	OutputDebugStringW(L" File location 2\n");
	OutputDebugStringA(FilePathShort.c_str());
	OutputDebugStringW(L"\n");
	//
	if (result_conn){
		OutputDebugStringA("Can't open database: \n");
		OutputDebugStringA(sqlite3_errmsg(db));
		OutputDebugStringA("\n");
		return CANNOT_OPEN_DB;
	}
	else{
		//
		OutputDebugStringA("Opened database successfully\n");
		//
		wstring Login = login->Data();
		wstring sql = L"SELECT PASSWORD FROM LOGIN WHERE LOGIN=";
		sql += Login;
		//
		char *zErrMsg = 0;
		const char* data = "Callback function called";
		//
		result_conn = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
		if (result_conn != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else{
			fprintf(stdout, "Operation done successfully\n");
		}
		//
	}
	//
	return LOGIN_SUCCEED;
	//
};

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
		
		//
		std::string output = "Started Listener on port ";
		output.append(to_string(port));
		output.append("\n");
		OutputDebugStringA(output.c_str());
		//
		StreamSocketListener^ Listener = ref new StreamSocketListener;
		//
		Listener->ConnectionReceived += ref new Windows::Foundation::TypedEventHandler<Windows::Networking::Sockets::StreamSocketListener ^, Windows::Networking::Sockets::StreamSocketListenerConnectionReceivedEventArgs ^>(this, &CommLayer::CommunicationLayer::OnConnectionReceived);
		//	
		auto m_BindService = Listener->BindServiceNameAsync((port).ToString());
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
			//
				output.clear();
				output = "Stopped Listener on port ";
				output.append(to_string(m_calc));
				output.append("\n");
				//OutputDebugStringA(output.c_str());
				//
		}
		//
		output.clear();
		output = "Stopped Listener on port ";
		output.append(to_string(port));
		output.append("\n");
		//OutputDebugStringA(output.c_str());
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


