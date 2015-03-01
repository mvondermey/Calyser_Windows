#pragma once


 
namespace CommLayer{

	ref class CommunicationLayer sealed
	{
	public:

		int LOGIN_SUCCEED = 1;
		int LOGIN_EXIST_PASSWORD_WRONG = 2;
		int LOGIN_NOT_EXIST = 3;

		CommunicationLayer();

		int CheckLogin( Platform::String^ login, Platform::String^password);

		void ReadDB();

		Windows::Foundation::IAsyncOperationWithProgress<Windows::Foundation::Collections::IVector<int>^, double>^ Runlistener(int port);

	private:
			
		void OnConnectionReceived(Windows::Networking::Sockets::StreamSocketListener ^, Windows::Networking::Sockets::StreamSocketListenerConnectionReceivedEventArgs ^);

	};

}

