#include "pch.h"
#include "CommunicationLayer.h"
#include <atomic>
#include <collection.h>
#include <ppltasks.h>
#include <concurrent_vector.h>
#include <string.h>
#include <sqlite3.h>
#include <string>

//
using namespace concurrency;
using namespace std;
using namespace CommLayer;
//
using namespace Windows::UI::Xaml::Media;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
//using namespace Windows::Networking::Sockets;
using namespace Windows::Networking;
using namespace Windows::Storage;
using namespace Windows::Storage::Streams;

StorageFolder^ localFolder = ApplicationData::Current->LocalFolder;

CommunicationLayer::CommunicationLayer()
{
	//
	my_streamsockets = ref new StreamSocket();
	//
//	Vector<StreamSocket^> my_streamsockets;
};

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

static int callback(void *obj, int argc, char **argv, char **azColName){
	//
	vector<std::pair<string,string>> *m_result = (vector<std::pair<string,string>>*) (obj);
	m_result->clear();
	//
	for (int i = 0; i<argc; i++){
		string colname(azColName[i]);
		string colentry(argv[i]);
		m_result->push_back(std::make_pair(colname,colentry));
		OutputDebugStringA("\n");
		OutputDebugStringA(azColName[i]);
		OutputDebugStringA(" = ");
		OutputDebugStringA(argv[i] ? argv[i] : "NULL");
		OutputDebugStringA("\n");
	}
	printf("\n");
	return 0;
}


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
		string sqlsh = "create table if not exists LOGIN(LOGIN VAR, PASSWORD VAR)";
		//
		OutputDebugStringA(sqlsh.c_str());
		//
		char *zErrMsg = 0;
		const char* data = "Callback function called";
		//
		vector<string>result;
		//
		result_conn = sqlite3_exec(db, sqlsh.c_str(), callback, (void*)&result, &zErrMsg);
		if (result_conn != SQLITE_OK){
			OutputDebugStringA("\n SQL error: ");
			OutputDebugStringA(to_string(result_conn).c_str());
			OutputDebugStringA("\n SQL error: ");
			OutputDebugStringA(zErrMsg);
			OutputDebugStringA("\n");
			sqlite3_free(zErrMsg);
			return CANNOT_CREATE_TABLE;
		}
		else{
			OutputDebugStringA("Operation done successfully\n");
		}
		//
		wstring Login = login->Data();
		wstring sql = L"SELECT PASSWORD FROM LOGIN WHERE LOGIN=\"";
		sql += Login;
		sql += L"\"";
		string sqlshort(sql.begin(),sql.end());
		OutputDebugStringA(sqlshort.c_str());
		//
		zErrMsg = 0;
		data = "Callback function called";
		//
		result_conn = sqlite3_exec(db, sqlshort.c_str(), callback, (void*) this,&zErrMsg);
		if (result_conn != SQLITE_OK){
			OutputDebugStringA("\n SQL error: ");
			OutputDebugStringA(to_string(result_conn).c_str());
			OutputDebugStringA("\n SQL error: ");
			OutputDebugStringA(zErrMsg);
			OutputDebugStringA("\n");
			sqlite3_free(zErrMsg);
			return CANNOT_DO_LOGIN_SELECT;
		}
		else{
			OutputDebugStringA("Operation done successfully\n");

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
	//m_streamsockets.Append(CalyserSocket);
	//
	std::string output = "Inside OnConectionReceived ";
	output.append("\n");
	OutputDebugStringA(output.c_str());
	//
	DataReader^ reader = ref new DataReader(CalyserSocket->InputStream);
	reader->InputStreamOptions = InputStreamOptions::Partial;
	//
	StreamSocketControl^ m_control = CalyserSocket->Control;
	StreamSocketInformation^ m_information = CalyserSocket->Information;
	HostName^ m_hostname = m_information->LocalAddress;
	String^ m_port = m_information->LocalPort;
	//
	String^ request;
	//
	OutputDebugStringA("Loop \n");
	//
	int bufferSize = 4096;
	//
	while (true) {
		//
		task <unsigned int> readerload(reader->LoadAsync(bufferSize));
		//
		OutputDebugStringA("Before wait\n");
		//
		readerload.wait();
		//
		OutputDebugStringA("Before get\n");
		//
		uint32 sizeFieldCount = readerload.get();
		//
		while (reader->UnconsumedBufferLength > 0){
			//
			OutputDebugStringA(" Open buffer = ");
			OutputDebugStringA(to_string(reader->UnconsumedBufferLength).c_str());
			OutputDebugStringA(" \n");
			//
			OutputDebugStringA("After get \n");
			//
			OutputDebugStringA(" = ");
			OutputDebugStringA(to_string(sizeFieldCount).c_str());
			OutputDebugStringA("\n");
			//
			//unsigned int bytesRead = reader->ReadUInt32();
			String^ m_string = reader->ReadString(sizeFieldCount);
			//string m_string_short(m_string.begin(), m_string.end());
			//string m_string_short(to_string(bytesRead));
			//
			OutputDebugStringW(m_string->Data());
			//
			request += m_string;
			//
		}
		//
		//string m_request_short(request.begin(), request.end());
		OutputDebugStringW(request->Data());
		//
		OutputDebugStringA("Done \n");
		//
		if ( sizeFieldCount < bufferSize) break;
		//
	};
	//
	// Analyse request / Write response
	//
	//String^ s_request = ref new(m_request.c_);
	//
	WriteResponseAsync(CalyserSocket,request);
	//
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



task  <unsigned int> CommunicationLayer::WriteResponseAsync(StreamSocket^ m_socket, String^ request){
	//
	
	//
	return create_task([this, m_socket, request ]()
	{
		//
		DataWriter^ writer = ref new DataWriter(m_socket->OutputStream);
		//writer.UnicodeEncoding = Windows.Storage.Streams.UnicodeEncoding.Utf8;
		writer->UnicodeEncoding = Windows::Storage::Streams::UnicodeEncoding::Utf8;
		//
		OutputDebugStringA("Writing \n");
		//
		//String^ response = ref new String(L"HTTP/1.1 404 Not Found\r\n");
		//response += "Content-Length:0\r\n";
		//response +=	"Connection: close\r\n\r\n";
		/*
		writer->WriteString("HTTP/1.0 500 Internal server error\r\n");
		writer->WriteString("Connection: close\r\n");
		writer->WriteString("\r\n");
		writer->WriteString(" Error \r\n");
		*/
		writer->WriteString("HTTP/1.1 200 OK \r\n");
		writer->WriteString("Transfer-Encoding:chunked \r\n");
		writer->WriteString("\r\n");
		writer->WriteString("25 \r\n");
		writer->WriteString("This is the data in the first chunk \r\n");
		writer->WriteString("1A \r\n");
		writer->WriteString("and this is the second one \r\n");
		writer->WriteString("0 \r\n");
		//
		//OutputDebugStringW(response->Data());
		/*
		while(true) {
			auto result = writer->StoreAsync();
			OutputDebugStringA(to_string(result->ErrorCode.Value).c_str());
			if (result->Status.ToString()->Data() == L"Completed") break;
		}
		*/
		//
		task <unsigned int> m_store(writer->StoreAsync());
		m_store.wait();
		task <bool> m_flush(writer->FlushAsync());
		m_flush.wait();
		writer->DetachStream();
		//
		//m_socket->OutputStream->FlushAsync();
		//
		//writer->WriteString(response);
		//
		OutputDebugStringA("Done Writing \n");
		//
		unsigned int value = 0;
		return value;
	});
	//
};

IAsyncOperationWithProgress<IVector<int>^, double>^ CommunicationLayer::RunTalker()
{
	return create_async([this](progress_reporter<double> reporter) -> IVector<int>^
	{
		//
		std::string output = "Started Talker ";
		output.append("\n");
		OutputDebugStringA(output.c_str());
		//
		//auto socket = this->GetStreamSocket();
		auto socket = ref new StreamSocket;
		//
		if (socket->Information->RemoteAddress){
			OutputDebugStringA("SocketInformation\n");
			OutputDebugStringW(socket->Information->RemoteAddress->ToString()->Data());
			OutputDebugStringA("\n");
		}
		//
		HostName^ hostname   = ref new HostName("192.168.137.225");
		//
		socket->Control->KeepAlive;
		//

		//
		//auto getFileTask = create_task(localFolder->GetFileAsync(fileName))
		//
		wstring FilePath = (localFolder->Path->Data());
		string FilePathShort(FilePath.begin(), FilePath.end());
		//
		//FilePathShort += "\\Caliser.db";
		//
		OutputDebugStringW(L" File location \n");
		OutputDebugStringW(localFolder->Path->Data());
		OutputDebugStringW(L"\n");
		//
		OutputDebugStringW(L" File listing\n");
		OutputDebugStringW(localFolder->Path->Data());
		OutputDebugStringW(L"\n");
		//
		create_task(localFolder->GetFilesAsync()).then([=](IVectorView<StorageFile^>^ FilesInFolder){
			//Iterate over the results and print the list of files
			// to the visual studio output window
			for (auto it = FilesInFolder->First(); it->HasCurrent; it->MoveNext())
			{
				StorageFile^ file = it->Current;
				String^ output = "file " + file->Name + "\n";
				OutputDebugStringW(output->Begin());
			}
		});
		//
		OutputDebugStringW(L" Open Now Connection\n");
		//
		auto openconnection = create_task(socket->ConnectAsync(hostname, "80"));
		/*
		openconnection.then([this,socket](void)
		{
			//
			//Now connect
			//
			DataReader^ reader = ref new DataReader(socket->InputStream);
			DataWriter^ writer = ref new DataWriter(socket->OutputStream);
			//
			//write a string to the OutputStream
			//
			writer->WriteString("some data to send to host");
			//
			// commit and send the data in the OutputStream
			writer->StoreAsync();
			//
		})
			*/
			openconnection.then([](task<void> t)
		{
			try
			{
				t.get();
				// .get() didn't throw, so we succeeded.
				OutputDebugStringA("Connected \n");
			}
			catch (Exception^ exception)
			{
				//Example output: The system cannot find the specified file.
				//exception->Message->Data
				OutputDebugStringW(exception->Message->Data());
				OutputDebugStringA("Not Connected \n");
			}

		});
		//
		//
		auto results = ref new Vector<int>();
		//
		output = "Stopped Talker ";
		output.append("\n");
		OutputDebugStringA(output.c_str());
		//
		return results;
	});
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
		StreamSocketListenerControl^ m_socketlistenercontrol = Listener->Control;
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


