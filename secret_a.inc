if (ERROR_SUCCESS==RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\Systemrestore6",0,KEY_READ,&tempregkey)){
RegCloseKey(tempregkey);
sysban=0.001f;
}
