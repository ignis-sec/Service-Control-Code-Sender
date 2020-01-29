
#include <iostream>
#include <windows.h>
#include <string>
#include <map>
#include <algorithm>

void initmaps();
std::map<std::string, DWORD> permMap;
std::map<DWORD, std::string> typeMap;
std::map<DWORD, std::string> statusMap;
std::map<std::string, DWORD> controlMap;
DWORD parsePerms(std::string perms);

int main(int argc, char** argv){

	initmaps();

	//Missing mandatory params
	if (argc < 3) {
		std::cout << "servReq.exe <serviceName> <code> <perm>(optional)";
		exit(1);
	}
	
	//Calculate permission schema if there is any specified
	DWORD perms = SERVICE_USER_DEFINED_CONTROL;
	if (argc >= 4) {
		std::string permstr(argv[3]);
		permstr.erase(remove(permstr.begin(), permstr.end(), ' '), permstr.end());
		perms = parsePerms(permstr);
	}

	//service name wchar buffer
	wchar_t servName[64];
	mbstowcs_s(nullptr, servName, 64, argv[1], _TRUNCATE);

	//request code to send to the service
	int reqCode = std::stoi(argv[2]);

	//NULL handles mean name or permission issue
	SC_HANDLE hSCManager = OpenSCManagerA(0, 0, SC_MANAGER_CONNECT);
	if (!hSCManager) {
		std::cout << "Can't access service manager." << std::endl;
		exit(2);
	}
	std::cout << "Service Manager Handle:" << hSCManager << std::endl;
	SC_HANDLE hService = OpenServiceW(hSCManager, servName, perms);
	if (!hService) {
		std::cout << "Can't access service, check privileges." << std::endl;
		exit(2);
	}
	std::cout << "Service Handle:" << hService << std::endl;
	
	//Query request
	LPSERVICE_STATUS servStat = new SERVICE_STATUS;
	ControlService(hService, reqCode, servStat);
	
	//current state of the service
	std::cout << "\tCurrent State: "<< servStat->dwCurrentState;
	auto t = statusMap.find(servStat->dwCurrentState);
	//check status name from the map
	if(t!=statusMap.end()) std::cout << " (" << t->second << ")" << std::endl;
	else std::cout << " (??????)" << std::endl;

	//service type
	std::cout << "\tService type: " << servStat->dwServiceType;
	auto tt = typeMap.find(servStat->dwServiceType);
	//check typename from the map
	if (tt != typeMap.end()) std::cout << " (" << tt->second << ")" << std::endl;
	else std::cout << " (??????)" << std::endl;

	//output all accepted controls
	std::cout << "\tControls Accepted: " << servStat->dwControlsAccepted;
	for (auto it = controlMap.begin(); it != controlMap.end(); ++it)
		if (it->second & servStat->dwControlsAccepted)
			std::cout << " (" << it->first << ")";
	std::cout << std::endl;

	//rest of the values
	std::cout << "\tCheckpoint: " << servStat->dwCheckPoint << std::endl;
	std::cout << "\tWait hint: " << servStat->dwWaitHint << std::endl;
	std::cout << "\tService Specific exit code: " << servStat->dwServiceSpecificExitCode << std::endl;
	std::cout << "\tWin32 Exit code: " << servStat->dwWin32ExitCode << std::endl;
}

//parse permissions, ex: GENERIC_READ|GENERIC_WRITE
DWORD parsePerms(std::string perms) {
	//recursion termination
	if (perms.empty()) return 0;
	
	DWORD ret = 0;
	//delimiter is |
	int dpos = perms.find("|");
	std::string perm;
	std::string rest;
	//No delimiter found, this is either the only permission, or the last one.
	if (dpos == std::string::npos) {
		perm = perms;
		rest = "";
	}
	//there is a delimiter, ignore rest for now
	else {
		perm = perms.substr(0, dpos++);
		rest = perms.substr(dpos);
	}
	ret = permMap.find(perm)->second;
	//std::cout << perm << ": " << ret << "\n";
	return ret | parsePerms(rest);
}


//initialize maps
void initmaps() {
	permMap["SERVICE_CONTROL_STOP"] = 0x1;
	permMap["SERVICE_CONTROL_PAUSE"] = 0x2;
	permMap["SERVICE_CONTROL_CONTINUE"] = 0x3;
	permMap["SERVICE_CONTROL_INTERROGATE"] = 0x4;
	permMap["SERVICE_CONTROL_PARAMCHANGE"] = 0x6;
	permMap["SERVICE_CONTROL_NETBINDADD"] = 0x7;
	permMap["SERVICE_CONTROL_NETBINDREMOVE"] = 0x8;
	permMap["SERVICE_CONTROL_NETBINDENABLE"] = 0x9;
	permMap["SERVICE_CONTROL_NETBINDDISABLE"] = 0xa;
	permMap["SERVICE_ALL_ACCESS"] = 0xF01FF;
	permMap["SERVICE_CHANGE_CONFIG"] = 0x0002;
	permMap["SERVICE_ENUMERATE_DEPENDENTS"] = 0x0008;
	permMap["SERVICE_INTERROGATE"] = 0x0080;
	permMap["SERVICE_PAUSE_CONTINUE"] = 0x0040;
	permMap["SERVICE_QUERY_CONFIG"] = 0x0001;
	permMap["SERVICE_QUERY_STATUS"] = 0x0004;
	permMap["SERVICE_START"] = 0x0010;
	permMap["SERVICE_STOP"] = 0x0020;
	permMap["SERVICE_USER_DEFINED_CONTROL"] = 0x0100;
	permMap["ACCESS_SYSTEM_SECURITY"] = ACCESS_SYSTEM_SECURITY;
	permMap["DELETE"] = 0x10000;
	permMap["READ_CONTROL"] = 0x20000;
	permMap["WRITE_DAC"] = 0x40000;
	permMap["WRITE_OWNER"] = 0x80000;

	permMap["GENERIC_READ"] = GENERIC_READ;
	permMap["GENERIC_WRITE"] = GENERIC_WRITE;
	permMap["GENERIC_EXECUTE"] = GENERIC_EXECUTE;
	permMap["GENERIC_ALL"] = GENERIC_ALL;


	typeMap[0x1] = "SERVICE_KERNEL_DRIVER";
	typeMap[0x2] = "SERVICE_FILE_SYSTEM_DRIVER";
	typeMap[0x10] = "SERVICE_WIN32_OWN_PROCESS";
	typeMap[0x20] = "SERVICE_WIN32_SHARE_PROCESS";
	typeMap[0x50] = "SERVICE_USER_OWN_PROCESS";
	typeMap[0x60] = "SERVICE_USER_SHARE_PROCESS";
	typeMap[0x100] = "SERVICE_INTERACTIVE_PROCESS";

	statusMap[0x1] = "SERVICE_STOPPED";
	statusMap[0x2] = "SERVICE_START_PENDING";
	statusMap[0x3] = "SERVICE_STOP_PENDING";
	statusMap[0x4] = "SERVICE_RUNNING";
	statusMap[0x5] = "SERVICE_CONTINUE_PENDING";
	statusMap[0x6] = "SERVICE_PAUSE_PENDING";
	statusMap[0x7] = "SERVICE_PAUSED";

	controlMap["SERVICE_ACCEPT_STOP"] = 0x1;
	controlMap["SERVICE_ACCEPT_PAUSE_CONTINUE"] = 0x2;
	controlMap["SERVICE_ACCEPT_SHUTDOWN"] = 0x4;
	controlMap["SERVICE_ACCEPT_PARAMCHANGE"] = 0x8;
	controlMap["SERVICE_ACCEPT_NETBINDCHANGE"] = 0x10;
	controlMap["SERVICE_ACCEPT_HARDWAREPROFILECHANGE"] = 0x20;
	controlMap["SERVICE_ACCEPT_POWEREVENT"] = 0x40;
	controlMap["SERVICE_ACCEPT_SESSIONCHANGE"] = 0x80;
	controlMap["SERVICE_ACCEPT_PRESHUTDOWN"] = 0x100;
	controlMap["SERVICE_ACCEPT_TIMECHANGE"] = 0x200;
	controlMap["SERVICE_ACCEPT_TRIGGEREVENT"] = 0x400;
	controlMap["SERVICE_ACCEPT_USERMODEREBOOT"] = 0x800;
}
