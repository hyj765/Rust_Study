#include"communication.h"


NTSTATUS StartCommunicate(
	PFLT_FILTER filter,
	PFLT_PORT port,
	PFLT_CONNECT_NOTIFY notifyCallback,
	PFLT_DISCONNECT_NOTIFY disConnectCallback,
	PFLT_MESSAGE_NOTIFY messageCallback,
	LONG MaxConnection,
	UNICODE_STRING name) {

	NTSTATUS status;
	OBJECT_ATTRIBUTES objattr;
	PSECURITY_DESCRIPTOR sd;

	status = FltBuildDefaultSecurityDescriptor(&sd, FLT_PORT_ALL_ACCESS);
	if (!NT_SUCCESS(status)) {
		return STATUS_UNSUCCESSFUL;
	}

	InitializeObjectAttributes(&objattr, &name, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, sd);

	status =FltCreateCommunicationPort(filter,&port,&objattr,NULL,notifyCallback,disConnectCallback,messageCallback,MaxConnection);
	FltFreeSecurityDescriptor(sd);

	if (NT_SUCCESS(status)) {
		KdPrint(("Communication Port Successfully Created PortName: %ws \r\n", name.Buffer));
	}
	else {
		KdPrint(("Communication Port Create Fail..\r\n"));
	}

	return status;
}

void CloseCommunication(PFLT_PORT port) {

	FltCloseCommunicationPort(port);

}

NTSTATUS DataSend(PFLT_FILTER filterHandle,PFLT_PORT clientPort, PVOID buffer, ULONG bufferSize, PVOID response, ULONG responseSize, ULONG timeOut) {

	LARGE_INTEGER convertedTimeOut;
	convertedTimeOut.QuadPart = -(INT64)timeOut * 10 * 1000;
	return FltSendMessage(filterHandle, &clientPort, buffer, bufferSize, response, &responseSize, &convertedTimeOut);
}

