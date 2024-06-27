
#include "Tests.hpp"


void Tests::testHttpInterpreter() {
	std::cout << "Testing the httpInterpreter: " << std::endl;
	std::cout <<"[1;34m--------------findRoute---------------[0m" << std::endl;
//==========================================================//
//-------------------Preparing for Test 1-------------------//
//==========================================================//
	testRequest.RequestedPath = "testing";
	testRequest.RequestIntegrity = OK_HTTP;
	testSettings.locations.insert(std::make_pair("testing",testLocation));
//----------------------------------------------------------//
//======================Running Test 1======================//
//----------------------------------------------------------//
	std::cout << "Correct full Path: ";
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 2-------------------//
//==========================================================//
	testRequest.RequestedPath = "testing/everything";
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 2======================//
//----------------------------------------------------------//
	std::cout << "Correct Partial Path: ";
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 3-------------------//
//==========================================================//
	testRequest.RequestedPath = "";
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 3======================//
//----------------------------------------------------------//
	std::cout << "Empty Path: ";
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != NOT_FOUND) {
		std::cout << "\033[1;31mFAILED\033[0m" << std::endl;
	} else {
		std::cout << "\033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 4-------------------//
//==========================================================//
	testRequest.RequestedPath = "hubkhbkf";
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 4======================//
//----------------------------------------------------------//
	std::cout << "Incorrect Path: ";
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != NOT_FOUND) {
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
