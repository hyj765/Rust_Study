
#include <fltKernel.h>
#include <dontuse.h>
#include"communication.h"

PFLT_FILTER gFilterHandle = NULL;
PFLT_PORT port = NULL;
PFLT_PORT clientPort = NULL;
const FLT_OPERATION_REGISTRATION callbacks[] = {
	{IRP_MJ_CREATE,0},
	{IRP_MJ_OPERATION_END}

};

NTSTATUS FilterUnload(FLT_FILTER_UNLOAD_FLAGS Flags) {
	UNREFERENCED_PARAMETER(Flags);
	KdPrint(("Filter Unloaded \r\n"));
	FltCloseCommunicationPort(port);
	FltUnregisterFilter(gFilterHandle);


	return STATUS_SUCCESS;
}

NTSTATUS ConnectedCallback(PFLT_PORT ClientPort, PVOID ServerPortCookie, PVOID ConnectionContext, ULONG SizeOfContext, PVOID* ConnectionPortCookie) {

	KdPrint(("connected \r\n"));
	clientPort = ClientPort;


	return STATUS_SUCCESS;
}

void DisConnectCallback(PVOID ConnectionCookie) {
	
	KdPrint(("DisCoonected \r\n"));
	FltCloseClientPort(gFilterHandle,clientPort);

}

NTSTATUS MessageNotifyCallback(PVOID portCookie, PVOID inputBuffer, ULONG inputBufferLength, PVOID OutputBuffer, ULONG OutputBUfferLength, PULONG ReturnOutputbufferLength) {



	return STATUS_SUCCESS;
}

const FLT_REGISTRATION FilterRegistration = {
	sizeof(FLT_REGISTRATION),
	FLT_REGISTRATION_VERSION,
	0,
	NULL,
	callbacks, // CALLBACKS
	FilterUnload, // UNLOAD
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL
};

NTSTATUS DriverEntry(PDRIVER_OBJECT pDrvObj, PUNICODE_STRING pRegPath) {

	NTSTATUS status;
	UNREFERENCED_PARAMETER(pRegPath);
	UNICODE_STRING name = RTL_CONSTANT_STRING(L"\\myFilter");
	status = FltRegisterFilter(pDrvObj, &FilterRegistration, &gFilterHandle);
	StartCommunicate(gFilterHandle, port, ConnectedCallback, DisConnectCallback, MessageNotifyCallback, 1, name);

	if (NT_SUCCESS(status)) {
		status = FltStartFiltering(gFilterHandle);

		
		CloseCommunication(port);
	}
	FltUnregisterFilter(gFilterHandle);

	return status;
}