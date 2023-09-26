#include"callbacks.h"



FLT_PREOP_CALLBACK_STATUS PreCreateCallback(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompleteContext) {

	PFLT_FILE_NAME_INFORMATION fileNameInfo;
	NTSTATUS status;
	WCHAR name[200] = { 0 };

	status = FltGetFileNameInformation(data, FLT_FILE_NAME_NORMALIZED | FLT_FILE_NAME_QUERY_DEFAULT, &fileNameInfo);

	if (NT_SUCCESS(status)) {

		status = FltParseFileNameInformation(fileNameInfo);

		if (NT_SUCCESS(status)) {

			if(fileNameInfo->Name.MaximumLength < 260) {

				RtlCopyMemory(name, fileNameInfo->Name.Buffer, fileNameInfo->Name.MaximumLength);
				KdPrint(("Create File: %ws \r\n", name));
			}

		}

		FltReleaseFileNameInformation(fileNameInfo);
	}
	

	return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

FLT_POSTOP_CALLBACK_STATUS PostCreateCallback(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext, FLT_POST_OPERATION_FLAGS Flags) {


	return FLT_POSTOP_FINISHED_PROCESSING;
}

FLT_PREOP_CALLBACK_STATUS PreReadCallback(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompleteContext);
FLT_POSTOP_CALLBACK_STATUS PostReadCallback(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext, FLT_POST_OPERATION_FLAGS Flags);

FLT_PREOP_CALLBACK_STATUS PreWriteCallback(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompleteContext);
FLT_POSTOP_CALLBACK_STATUS PostWriteCallback(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext, FLT_POST_OPERATION_FLAGS Flags);

FLT_PREOP_CALLBACK_STATUS PreCloseCallback(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompleteContext);
FLT_POSTOP_CALLBACK_STATUS PostCloseCallback(PFLT_CALLBACK_DATA data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext, FLT_POST_OPERATION_FLAGS Flags);
