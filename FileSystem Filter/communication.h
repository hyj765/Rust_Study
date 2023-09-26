#pragma once
#include<fltKernel.h>
#include<ntddk.h>


	NTSTATUS StartCommunicate(
		PFLT_FILTER filter,
		PFLT_PORT port,
		PFLT_CONNECT_NOTIFY notifyCallback,
		PFLT_DISCONNECT_NOTIFY disConnectCallback,
		PFLT_MESSAGE_NOTIFY messageCallback,
		LONG MaxConnection,
		UNICODE_STRING name);

	void CloseCommunication(PFLT_PORT port);

	NTSTATUS DataSend(PFLT_FILTER filterHandle,PFLT_PORT clientPort, PVOID buffer, ULONG bufferSize, PVOID response, ULONG responseSize, ULONG timeOut);

