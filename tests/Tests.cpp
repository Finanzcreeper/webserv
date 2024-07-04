
#include "Tests.hpp"


void Tests::testHttpInterpreter() {
	std::cout << "\033[1;95mTesting the httpInterpreter:\033[0m " << std::endl;
	std::cout <<"[1;34m--------------findRoute---------------[0m" << std::endl;

	location empty;
	empty.cgi.clear();
	empty.dirListing = "";
	empty.httpMethods = 0;
	empty.index = "";
	empty.locationName = "";
	empty.redirect = "";
	empty.root = "";
//==========================================================//
//-------------------Preparing for Test 1-------------------//
//==========================================================//
	this->testRequest.UsedRoute = empty;
	testRequest.RequestedPath = "testing";
	testRequest.RequestIntegrity = OK_HTTP;

	this->testLocation.locationName = "testing";
	this->testLocation.root = "folder";
	testSettings.locations.insert(std::make_pair("testing",testLocation));
//----------------------------------------------------------//
//======================Running Test 1======================//
//----------------------------------------------------------//
	std::cout << "Correct full Path: ";
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != OK_HTTP || testRequest.RoutedPath != "folder") {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 2-------------------//
//==========================================================//
	this->testRequest.UsedRoute = empty;
	testRequest.RequestedPath = "testing/everything";
	testRequest.RoutedPath = "";
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 2======================//
//----------------------------------------------------------//
	std::cout << "Correct Partial Path: ";
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != OK_HTTP || testRequest.RoutedPath != "folder/everything") {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 3-------------------//
//==========================================================//
	this->testRequest.UsedRoute = empty;
	testRequest.RequestedPath = "";
	testRequest.RoutedPath = "";
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 3======================//
//----------------------------------------------------------//
	std::cout << "Empty Path: ";
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != NOT_FOUND || testRequest.RoutedPath != "") {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 4-------------------//
//==========================================================//
	testRequest.RequestedPath = "hubkhbkf";
	testRequest.RequestIntegrity = OK_HTTP;
	this->testRequest.UsedRoute = empty;
	testRequest.RoutedPath = "";
//----------------------------------------------------------//
//======================Running Test 4======================//
//----------------------------------------------------------//
	std::cout << "Incorrect Path: ";
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != NOT_FOUND || testRequest.RoutedPath != "") {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 5-------------------//
//==========================================================//
	testRequest.RequestedPath = "testing.html";
	testRequest.RequestIntegrity = OK_HTTP;
	this->testRequest.UsedRoute = empty;
	testRequest.RoutedPath = "";
//----------------------------------------------------------//
//======================Running Test 4======================//
//----------------------------------------------------------//
	std::cout << "requested file with same name as location: ";
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != NOT_FOUND || testRequest.RoutedPath != "") {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}


	std::cout <<"[1;34m-----checkIfMethodIsAllowedOnRoute----[0m" << std::endl;
//==========================================================//
//-------------------Preparing for Test 1-------------------//
//==========================================================//
	testRequest.ReqType = GET;
	testRequest.RequestIntegrity = OK_HTTP;
	testRequest.UsedRoute.httpMethods = GET;
//----------------------------------------------------------//
//======================Running Test 1======================//
//----------------------------------------------------------//
	std::cout << "Allowed Method (one is allowed): ";
	checkIfMethodIsAllowedOnRoute(testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 2-------------------//
//==========================================================//
	testRequest.ReqType = GET;
	testRequest.RequestIntegrity = OK_HTTP;
	testRequest.UsedRoute.httpMethods = GET | POST | PATCH;
//----------------------------------------------------------//
//======================Running Test 2======================//
//----------------------------------------------------------//
	std::cout << "Allowed Method (multiple are allowed): ";
	checkIfMethodIsAllowedOnRoute(testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 3-------------------//
//==========================================================//
	testRequest.ReqType = GET;
	testRequest.RequestIntegrity = OK_HTTP;
	testRequest.UsedRoute.httpMethods = POST;
//----------------------------------------------------------//
//======================Running Test 3======================//
//----------------------------------------------------------//
	std::cout << "Disallowed Method (one is allowed): ";
	checkIfMethodIsAllowedOnRoute(testRequest);
	if (testRequest.RequestIntegrity != METHOD_NOT_ALLOWED) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 4-------------------//
//==========================================================//
	testRequest.ReqType = PUT;
	testRequest.RequestIntegrity = OK_HTTP;
	testRequest.UsedRoute.httpMethods = GET | POST | PATCH;
//----------------------------------------------------------//
//======================Running Test 4======================//
//----------------------------------------------------------//
	std::cout << "Disallowed Method (multiple are allowed): ";
	checkIfMethodIsAllowedOnRoute(testRequest);
	if (testRequest.RequestIntegrity != METHOD_NOT_ALLOWED) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}

	std::cout <<"[1;34m-------------checkBodySize------------[0m" << std::endl;
//==========================================================//
//-------------------Preparing for Test 1-------------------//
//==========================================================//
	testRequest.Body = "FOO";
	testSettings.clientMaxBodySize = 10;
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 1======================//
//----------------------------------------------------------//
	std::cout << "Body size ok:  ";
	checkBodySize(testRequest,testSettings);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 2-------------------//
//==========================================================//
	testRequest.Body = "0123456789L";
	testSettings.clientMaxBodySize = 10;
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 2======================//
//----------------------------------------------------------//
	std::cout << "Body size to big:  ";
	checkBodySize(testRequest,testSettings);
	if (testRequest.RequestIntegrity != PAYLOAD_TO_LARGE) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 3-------------------//
//==========================================================//
	testRequest.Body = "0123456789";
	testSettings.clientMaxBodySize = 10;
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 3======================//
//----------------------------------------------------------//
	std::cout << "Body size same as clientMaxBodySize: ";
	checkBodySize(testRequest,testSettings);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}

	std::cout <<"[1;34m-----------redirectionChecker----------[0m" << std::endl;
//==========================================================//
//-------------------Preparing for Test 1-------------------//
//==========================================================//
	testRequest.UsedRoute.redirect = "";
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 1======================//
//----------------------------------------------------------//
	std::cout << "Redirect empty: ";
	redirectionChecker(testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 2-------------------//
//==========================================================//
	testRequest.UsedRoute.redirect = "eeEEeeEeE";
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 2======================//
//----------------------------------------------------------//
	std::cout << "Redirect not empty: ";
	redirectionChecker(testRequest);
	if (testRequest.RequestIntegrity != MOVED_PERMANENTLY) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}

	std::cout <<"[1;34m------------checkContentType-----------[0m" << std::endl;
//==========================================================//
//-------------------Preparing for Test 1-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 1======================//
//----------------------------------------------------------//
	std::cout << "No content type: ";
	checkContentType(testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 2-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "text/plain"));
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 2======================//
//----------------------------------------------------------//
	std::cout << "accepted content & subcontent type: ";
	checkContentType(testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 3-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "ext/plain"));
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 3======================//
//----------------------------------------------------------//
	std::cout << "not accepted content type: ";
	checkContentType(testRequest);
	if (testRequest.RequestIntegrity != UNSUPPORTED_MEDIA_TYPE) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 4-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "text/rain"));
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 5======================//
//----------------------------------------------------------//
	std::cout << "not accepted subContent type: ";
	checkContentType(testRequest);
	if (testRequest.RequestIntegrity != UNSUPPORTED_MEDIA_TYPE) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
}

void Tests::testConfigParser() {

}

void Tests::testConfigLocationParser() {

}

void Tests::testHttpParser() {
	std::cout << std::endl << "\033[1;95mTesting the httpParser:\033[0m " << std::endl;
	std::cout <<"[1;34m----Complete Header on empty struct----[0m" << std::endl;
	this->testConnection.r.RequestBuffer = "GET /index.html HTTP/1.1\r\nHost: www.example.re\r\nUser-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.1)\r\nAccept: text/html\r\nAccept-Language: en-US, en; q=0.5\r\nAccept-Encoding: gzip, deflate\r\n\r\n";
	this->testConnectionMap.insert(std::make_pair(3, this->testConnection));
	
	this->testPair = this->testConnectionMap.find(3);
	httpParser test(this->testPair);
	std::map<std::string, std::string> expectedHeaderFields;

	this->testConnection.r.RequestBuffer = "GET /index.html HTTP/1.1\r\nHost: www.example.re\r\nUser-Agent: Mozi";
	this->testConnectionMap.clear();
	this->testConnectionMap.insert(std::make_pair(4, this->testConnection));

	this->testPair = this->testConnectionMap.find(3);
	httpParser test1(this->testPair);
	expectedHeaderFields.clear();
	
//==========================================================//
//-------------------Preparing for Test 1-------------------//
//==========================================================//

	expectedHeaderFields.insert(std::make_pair("user-agent", "Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.1)"));
//----------------------------------------------------------//
//======================Running Test 1======================//
//----------------------------------------------------------//
//----------------------------------------------------------//
//=========================Check 1==========================//
//----------------------------------------------------------//
	std::cout << "Request Buffer consumed: ";
	if (this->testConnectionMap.find(3)->second.r.RequestBuffer.empty() == false) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//----------------------------------------------------------//
//=========================Check 2==========================//
//----------------------------------------------------------//
	std::cout << "Header Buffer filled: ";
	if (this->testConnectionMap.find(3)->second.r.HeaderBuffer.empty() == true) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//----------------------------------------------------------//
//=========================Check 3==========================//
//----------------------------------------------------------//
	std::cout << "Request Type found: ";
	if (this->testConnectionMap.find(3)->second.r.ReqType != GET) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//----------------------------------------------------------//
//=========================Check 4==========================//
//----------------------------------------------------------//
	std::cout << "Request Path found: ";
	if (this->testConnectionMap.find(3)->second.r.RequestedPath != "/index.html") {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//----------------------------------------------------------//
//=========================Check 5==========================//
//----------------------------------------------------------//
	std::cout << "Header fields decapitalized correctly: ";
	if (this->testConnectionMap.find(3)->second.r.HeaderBuffer != "GET /index.html HTTP/1.1\r\nhost: www.example.re\r\nuser-agent: Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.1)\r\naccept: text/html\r\naccept-language: en-US, en; q=0.5\r\naccept-encoding: gzip, deflate\r\n\r\n") {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//----------------------------------------------------------//
//=========================Check 6==========================//
//----------------------------------------------------------//
	std::cout << "Header fields extracted correctly: ";
	if(this->testConnectionMap.find(3)->second.r.HeaderFields != expectedHeaderFields) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//----------------------------------------------------------//
//=========================Check 7==========================//
//----------------------------------------------------------//
	std::cout << "Body empty: ";
	if(this->testConnectionMap.find(3)->second.r.Body.empty() == false) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}



	std::cout <<"[1;34m----Incomplete Header on empty struct----[0m" << std::endl;
//==========================================================//
//-------------------Preparing for Test 1-------------------//
//==========================================================//














//----------------------------------------------------------//
//======================Running Test 1======================//
//----------------------------------------------------------//
//----------------------------------------------------------//
//=========================Check 1==========================//
//----------------------------------------------------------//
	std::cout << "Request Buffer consumed: ";
	if (this->testConnectionMap.find(3)->second.r.RequestBuffer.empty() == false) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//----------------------------------------------------------//
//=========================Check 2==========================//
//----------------------------------------------------------//
	std::cout << "Header Buffer filled: ";
	if (this->testConnectionMap.find(3)->second.r.HeaderBuffer.empty() == true) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//----------------------------------------------------------//
//=========================Check 3==========================//
//----------------------------------------------------------//
	std::cout << "Request Type found: ";
	if (this->testConnectionMap.find(3)->second.r.ReqType != GET) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//----------------------------------------------------------//
//=========================Check 4==========================//
//----------------------------------------------------------//
	std::cout << "Request Path found: ";
	if (this->testConnectionMap.find(3)->second.r.RequestedPath != "/index.html") {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//----------------------------------------------------------//
//=========================Check 5==========================//
//----------------------------------------------------------//
	std::cout << "Header fields decapitalized correctly: ";
	if (this->testConnectionMap.find(3)->second.r.HeaderBuffer != "GET /index.html HTTP/1.1\r\nhost: www.example.re\r\nuser-agent: Mozilla/5.0 (Windows; U; Windows NT 5.0; en-US; rv:1.1)\r\naccept: text/html\r\naccept-language: en-US, en; q=0.5\r\naccept-encoding: gzip, deflate\r\n\r\n") {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//----------------------------------------------------------//
//=========================Check 6==========================//
//----------------------------------------------------------//
	std::cout << "Header fields extracted correctly: ";
	if(this->testConnectionMap.find(3)->second.r.HeaderFields != expectedHeaderFields) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//----------------------------------------------------------//
//=========================Check 7==========================//
//----------------------------------------------------------//
	std::cout << "Body empty: ";
	if(this->testConnectionMap.find(3)->second.r.Body.empty() == false) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
}

void Tests::testMethodExecutor() {

}

void Tests::testServer() {

}

void Tests::testStatusCodes() {

}

void Tests::testing() {
	testConfigLocationParser();
	testConfigParser();
	testHttpInterpreter();
	testHttpParser();
	testMethodExecutor();
	testServer();
	testStatusCodes();
}
