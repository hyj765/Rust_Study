#include"stdafx.h"
#include"regstrymgr.h"
#include"VmDetector.h"
using namespace std;

void main() {
	
	
	VMDetector st;

	if (st.IOPortBasedDetection()) {
		printf("is vm");
	}
	else {
		printf("is not vm");
	}



}



 