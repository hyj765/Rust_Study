#include"stdafx.h";
#include"regstrymgr.h";
using namespace std;

void main() {
        

    RegMgr sts;
    sts.RegstrySelectALL(HKEY_LOCAL_MACHINE,"SOFTWARE\\Microsoft\\Windows\\CurrentVersion");
}



 