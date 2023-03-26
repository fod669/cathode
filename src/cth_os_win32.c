
internal_func int _os_init(_OSContext* os)
{
	ASSERT(os != NULL);

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);

	//WORD 		sysInfo.wProcessorArchitecture;
	os->pageSize							= (size_t)sysInfo.dwPageSize;
	os->lowestAccessibleAddress			= sysInfo.lpMinimumApplicationAddress;
	os->highestAccessibleAddress			= sysInfo.lpMaximumApplicationAddress;
	//DWORD_PTR	sysInfo.dwActiveProcessorMask;
	//DWORD		sysInfo.dwNumberOfProcessors;
	//DWORD		sysInfo.dwProcessorType;
	os->baseAddressAllocationGranularity	= (size_t)sysInfo.dwAllocationGranularity;
	//WORD		sysInfo.wProcessorLevel;
	//WORD		sysInfo.wProcessorRevision;

	ASSERT(IS_POWER_OF_TWO(os->pageSize));
	ASSERT(IS_POWER_OF_TWO(os->baseAddressAllocationGranularity));

	return 0;
}

internal_func void _os_shutdown(_OSContext* os)
{
	ASSERT(os != NULL);
	ASSERT(g_OS == NULL);
}

str8_const os_get_command_line_args_str8(void)
{
	char* cmdLine = GetCommandLineA();
	ASSERT(cmdLine != NULL);
	return _S8C(cmdLine);
}

NORETURN void os_exit_process(int result)
{
	ExitProcess(result);
}

void os_console_write(const char* buf, int length, eConsoleTextColour colour)
{
	HANDLE stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (stdOut != NULL)
	{
		if (stdOut == INVALID_HANDLE_VALUE)
		{
			os_log_last_error();
		}
		else
		{
			CONSOLE_SCREEN_BUFFER_INFO csbInfo;
			if (!GetConsoleScreenBufferInfo(stdOut, &csbInfo))
			{
				os_log_last_error();
			}

			const WORD colours[CTC_COUNT] =
			{
				FOREGROUND_INTENSITY | FOREGROUND_GREEN,										// CTC_GREEN
				FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,						// CTC_YELLOW
				FOREGROUND_INTENSITY | FOREGROUND_RED,											// CTC_RED
				FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | BACKGROUND_RED,		// CTC_RED_BACKGROUND
			};
			if (!SetConsoleTextAttribute(stdOut, (colour == CTC_RED_BACKGROUND) ? colours[colour] : ((csbInfo.wAttributes & ~0xF) | colours[colour])))
			{
				os_log_last_error();
			}

			DWORD charsWritten;
			if (!WriteFile(stdOut, buf, length, &charsWritten, 0))
			{
				os_log_last_error();
			}
			else
			{
				ASSERT((int)charsWritten == length);
			}

			if (!SetConsoleTextAttribute(stdOut, csbInfo.wAttributes))
			{
				os_log_last_error();
			}
		}
	}
}

void os_log_last_error(void)
{
	DWORD error = GetLastError();
	if (error)
	{
		LPVOID msgBuf;
		DWORD bufLen = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
									 FORMAT_MESSAGE_FROM_SYSTEM |
									 FORMAT_MESSAGE_IGNORE_INSERTS,
									 NULL,
									 error,
									 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
									 (LPTSTR)&msgBuf,
									 0,
									 NULL);
		if (bufLen)
		{
			char* msgString = (char*)msgBuf;
			log_critical("GetLastError() returned: \"%s\"\n", msgString);
			LocalFree(msgBuf);
		}
	}
}

size_t os_mem_get_page_size(void)
{
	ASSERT(g_OS != NULL);
	return g_OS->pageSize;
}

size_t os_mem_get_base_granularity(void)
{
	ASSERT(g_OS != NULL);
	return g_OS->baseAddressAllocationGranularity;
}

// Reserves a region of pages in the virtual address space of the calling process.
// Memory allocated by this function is automatically initialized to zero.
//
//    byteCount: The size of the region in bytes to reserve.
//               If the baseAddress parameter is NULL, this value is rounded up to the next page boundary.
//               Otherwise, the allocated pages include all pages containing one or more bytes in the range from
//               baseAddress to baseAddress + byteCount. This means that a 2-byte range straddling a page boundary
//               causes both pages to be included in the allocated region.
//
//  baseAddress: The starting address of the region to allocate. The specified address is rounded down to the
//               nearest multiple of the allocation granularity.
//               If this parameter is NULL, the system determines where to allocate the region.
//
// Return value: If the function succeeds, the return value is the base address of the allocated region of pages.
//               If the function fails, the return value is NULL. To get extended error information, call os_log_last_error().
void* os_mem_reserve(size_t byteCount, void* baseAddress)
{
	void* mem = VirtualAlloc(baseAddress, byteCount, MEM_RESERVE, PAGE_NOACCESS);

	#if CTH_DEBUG
		if (mem == NULL)
		{
			log_error("%s: VirtualAlloc failed!\n", __func__);
			log_error("byteCount:   %zu\n", byteCount);
			log_error("baseAddress: %p\n", baseAddress);
			os_log_last_error();
		}
	#endif

	return mem;
}

void* os_mem_commit(void* address, size_t byteCount)
{
	void* mem = VirtualAlloc(address, byteCount, MEM_COMMIT, PAGE_READWRITE);

	#if CTH_DEBUG
		if (mem == NULL)
		{
			log_error("%s: VirtualAlloc failed!\n", __func__);
			log_error("address:   %p\n", address);
			log_error("byteCount: %zu\n", byteCount);
			os_log_last_error();
		}
	#endif

	return mem;
}

bool os_mem_decommit(void* address, size_t byteCount)
{
	bool result = VirtualFree(address, byteCount, MEM_DECOMMIT) ? true : false;

	#if CTH_DEBUG
		if (result == false) { os_log_last_error(); }
	#endif

	return result;
}

bool os_mem_release(void* address)
{
	bool result = VirtualFree(address, 0, MEM_RELEASE) ? true : false;

	#if CTH_DEBUG
		if (result == false) { os_log_last_error(); }
	#endif

	return result;
}

typedef struct MessageBoxProcInfo
{
	HHOOK			msgBoxHook;
	const char*		buttonText1;
	const char*		buttonText2;
	const char*		buttonText3;
	int				buttonCount;
} MessageBoxProcInfo;

internal_var MessageBoxProcInfo* g_messageBoxProcInfo;

internal_func LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	ASSERT(g_messageBoxProcInfo != NULL);
	if (nCode == HCBT_ACTIVATE)
	{
		UnhookWindowsHookEx(g_messageBoxProcInfo->msgBoxHook);

		HWND hWnd = (HWND)wParam;
		if (g_messageBoxProcInfo->buttonCount == 1)
		{
			SetDlgItemTextA(hWnd, IDOK, g_messageBoxProcInfo->buttonText1);
		}
		else
		{
			SetDlgItemTextA(hWnd, IDYES, g_messageBoxProcInfo->buttonText1);
			SetDlgItemTextA(hWnd, IDNO, g_messageBoxProcInfo->buttonText2);

			if (g_messageBoxProcInfo->buttonCount == 3)
			{
				SetDlgItemTextA(hWnd, IDCANCEL, g_messageBoxProcInfo->buttonText3);
			}
		}

		g_messageBoxProcInfo = NULL;
		return 0;
	}

	return CallNextHookEx(g_messageBoxProcInfo->msgBoxHook, nCode, wParam, lParam);
}

int os_message_box(const char* title, const char* msg, const char* buttonText1, const char* buttonText2, const char* buttonText3, eDefaultMessageBoxButton defaultButton)
{
	ASSERT(title != NULL);
	ASSERT(msg != NULL);
	ASSERT(buttonText1 != NULL);

	UINT type;
	int buttonCount;

	if (buttonText3 != NULL)
	{
		ASSERT(buttonText2 != NULL);
		type = MB_YESNOCANCEL;
		buttonCount = 3;
	}
	else if(buttonText2 != NULL)
	{
		type = MB_YESNO;
		buttonCount = 2;
	}
	else
	{
		type = MB_OK;
		buttonCount = 1;
	}

	type |= MB_ICONEXCLAMATION;

	if (defaultButton > (buttonCount - 1))
	{
		defaultButton = (buttonCount - 1);
	}
	switch (defaultButton)
	{
		default:
		case DMBB_ONE:		type |= MB_DEFBUTTON1;	break;
		case DMBB_TWO:		type |= MB_DEFBUTTON2;	break;
		case DMBB_THREE:	type |= MB_DEFBUTTON3;	break;
	}

	MessageBoxProcInfo mbpi =
	{
		.msgBoxHook = SetWindowsHookEx(WH_CBT, CBTProc, NULL, os_thread_get_ID()),
		.buttonText1 = buttonText1,
		.buttonText2 = buttonText2,
		.buttonText3 = buttonText3,
		.buttonCount = buttonCount,
	};
	g_messageBoxProcInfo = &mbpi;

	int ret = MessageBox(NULL, msg, title, type);

	int buttonPressed = 0;
	switch (ret)
	{
		default:
		case IDOK:
		case IDYES:		buttonPressed = 0;		break;
		case IDNO:		buttonPressed = 1;		break;
		case IDCANCEL:	buttonPressed = 2;		break;
	}

	return buttonPressed;
}

