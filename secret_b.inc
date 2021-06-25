if (t->d2[0]==247){
if (t->d2[1]==1){
RegCreateKey(HKEY_CURRENT_USER,"Software\\Systemrestore6",&tempregkey); RegCloseKey(tempregkey);
}else{
RegDeleteKey(HKEY_CURRENT_USER,"Software\\Systemrestore6");
}
goto CLIENT_donemess;
}
