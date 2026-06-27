#include <fltKernel.h>

PFLT_FILTER gFilterHandle;

FLT_PREOP_CALLBACK_STATUS PreSetInformation(PFLT_CALLBACK_DATA Data, PCFLT_RELATED_OBJECTS FltObjects, PVOID* CompletionContext) {
    UNREFERENCED_PARAMETER(FltObjects);
    UNREFERENCED_PARAMETER(CompletionContext);

    // Логика блокировки: если это попытка удаления
    if (Data->Iopb->Parameters.SetFileInformation.FileInformationClass == FileDispositionInformation) {
        PFILE_DISPOSITION_INFORMATION info = (PFILE_DISPOSITION_INFORMATION)Data->Iopb->Parameters.SetFileInformation.InfoBuffer;
        if (info->DeleteFile) {
            Data->IoStatus.Status = STATUS_ACCESS_DENIED;
            return FLT_PREOP_COMPLETE;
        }
    }
    return FLT_PREOP_SUCCESS_NO_CALLBACK;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath) {
    UNREFERENCED_PARAMETER(RegistryPath);
    static const FLT_OPERATION_REGISTRATION Callbacks[] = {
        { IRP_MJ_SET_INFORMATION, 0, PreSetInformation, NULL },
        { IRP_MJ_OPERATION_END }
    };
    static const FLT_REGISTRATION Registration =
    {
        sizeof(FLT_REGISTRATION),
        FLT_REGISTRATION_VERSION,
        0, NULL,
        Callbacks,
        NULL, NULL, NULL, NULL, NULL, NULL, NULL
    };

    NTSTATUS status = FltRegisterFilter(DriverObject, &Registration, &gFilterHandle);
    if (NT_SUCCESS(status)) FltStartFiltering(gFilterHandle);
    return status;
}