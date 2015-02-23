#pragma once



namespace CommunicationLayerN{

	ref class CommunicationLayer sealed
	{
	public:
		CommunicationLayer();

		void ReadDB();

		Windows::Foundation::IAsyncOperationWithProgress<Windows::Foundation::Collections::IVector<int>^, double>^ Runlistener(int port);


	 
			
		void OnConnectionReceived(Windows::Networking::Sockets::StreamSocketListener ^, Windows::Networking::Sockets::StreamSocketListenerConnectionReceivedEventArgs ^);

	};

}

