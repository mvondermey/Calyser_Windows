#pragma once

namespace CommLayer{

	enum LOGIN_RESULT
	{
		LOGIN_SUCCEED = 0,
		LOGIN_EXIST_PASSWORD_WRONG,
		LOGIN_NOT_EXIST,
		CANNOT_OPEN_DB,
		CANNOT_CREATE_TABLE,
		CANNOT_DO_LOGIN_SELECT
	};

	ref class CommunicationLayer sealed
	{
	public:


		CommunicationLayer();

		int CheckLogin( Platform::String^ login, Platform::String^ password);

		void ReadDB();

		Windows::Foundation::IAsyncOperationWithProgress<Windows::Foundation::Collections::IVector<int>^, double>^ Runlistener(int port);

	private:
			
		//Vector<StreamSocket> m_streamsockets;

		concurrency::task  <unsigned int> CommunicationLayer::WriteResponseAsync(Windows::Networking::Sockets::StreamSocket^ m_socket, Platform::String^ request);

		void OnConnectionReceived(Windows::Networking::Sockets::StreamSocketListener ^, Windows::Networking::Sockets::StreamSocketListenerConnectionReceivedEventArgs ^);

//		int callback(void *data, int argc, char **argv, char **azColName);


	};

}

