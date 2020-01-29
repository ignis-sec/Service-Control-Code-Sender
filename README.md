# ServReqHandle
 
### Usage:
 `servRequest.exe \<serviceName\> \<code\> \<perm\>(optional)`
 
 ## Parameters
 - ServiceName: Name of the service, wchar_t
 - code: control code to send
 - perm: Permission schema, for multiple permissions use or operator
 # Example Run:
 
- `ServReqHandle.exe AppInfo 4 "GENERIC_ALL"` (elevation required)
- `ServReqHandle.exe AppInfo 128`
 
  
# Permissions
If not specified, will be set to `SERVICE_USER_DEFINED_CONTROL`

If specified, use the or operator.

Example: `"GENERIC_READ|GENERIC_WRITE"`

Complete list of permissions can be found from [microsoft docs](https://docs.microsoft.com/en-us/windows/win32/services/service-security-and-access-rights)

# Control code

For all services, codes 0-127 is reserved by microsoft, and codes 128-255 are service-defined, so they can define their own action.

Full documentation can be found from the [microsoft docs](https://docs.microsoft.com/en-us/windows/win32/api/winsvc/nf-winsvc-controlservice).

## SERVICE_CONTROL_CONTINUE
### 0x00000003
Notifies a paused service that it should resume. The hService handle must have the SERVICE_PAUSE_CONTINUE access right.

## SERVICE_CONTROL_INTERROGATE
### 0x00000004
Notifies a service that it should report its current status information to the service control manager. The hService handle must have the SERVICE_INTERROGATE access right.
Note that this control is not generally useful as the SCM is aware of the current state of the service.

## SERVICE_CONTROL_NETBINDADD
### 0x00000007
Notifies a network service that there is a new component for binding. The hService handle must have the SERVICE_PAUSE_CONTINUE access right. However, this control code has been deprecated; use Plug and Play functionality instead.

## SERVICE_CONTROL_NETBINDDISABLE
### 0x0000000A
Notifies a network service that one of its bindings has been disabled. The hService handle must have the SERVICE_PAUSE_CONTINUE access right. However, this control code has been deprecated; use Plug and Play functionality instead.

## SERVICE_CONTROL_NETBINDENABLE
### 0x00000009
Notifies a network service that a disabled binding has been enabled. The hService handle must have the SERVICE_PAUSE_CONTINUE access right. However, this control code has been deprecated; use Plug and Play functionality instead.

## SERVICE_CONTROL_NETBINDREMOVE
### 0x00000008
Notifies a network service that a component for binding has been removed. The hService handle must have the SERVICE_PAUSE_CONTINUE access right. However, this control code has been deprecated; use Plug and Play functionality instead.

## SERVICE_CONTROL_PARAMCHANGE
### 0x00000006
Notifies a service that its startup parameters have changed. The hService handle must have the SERVICE_PAUSE_CONTINUE access right.

## SERVICE_CONTROL_PAUSE
### 0x00000002
Notifies a service that it should pause. The hService handle must have the SERVICE_PAUSE_CONTINUE access right.

## SERVICE_CONTROL_STOP
### 0x00000001
Notifies a service that it should stop. The hService handle must have the SERVICE_STOP access right.
After sending the stop request to a service, you should not send other controls to the service.

 
