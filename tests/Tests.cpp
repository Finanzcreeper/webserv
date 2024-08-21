#include "Tests.hpp"

void Tests::testHttpInterpreter() {
	std::cout << "\033[1;95mTesting the httpInterpreter:\033[0m " << std::endl;
	std::cout <<"[1;34m--------------findRoute----------------[0m" << std::endl;

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

	this->testLocation.locationName = "/testing/";
	this->testLocation.root = "/folder/";
	testSettings.locations.insert(std::make_pair("/testing/",testLocation));
	
	this->testLocation.locationName = "/";
	this->testLocation.root = "/ha/ha/ha/";
	testSettings.locations.insert(std::make_pair("/",testLocation));
	
	this->testLocation.locationName = "/index.html";
	this->testLocation.root = "/gerhard/content.html";
	testSettings.locations.insert(std::make_pair("/index.html",testLocation));
	
	this->testLocation.locationName = "/testing/index.html";
	this->testLocation.root = "/content/braunbär.html";
	testSettings.locations.insert(std::make_pair("/testing/index.html",testLocation));

	this->testRequest.UsedRoute = empty;
	testRequest.RequestedPath = "/testing/";
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 1======================//
//----------------------------------------------------------//
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != OK_HTTP || testRequest.RoutedPath != "/folder/") {
		std::cout << "Correct full Path: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Correct full Path: \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 2-------------------//
//==========================================================//
	this->testRequest.UsedRoute = empty;
	testRequest.RequestedPath = "/testing/everything/";
	testRequest.RoutedPath = "";
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 2======================//
//----------------------------------------------------------//
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != OK_HTTP || testRequest.RoutedPath != "/folder/everything/") {
		std::cout << "Correct Partial Path: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Correct Partial Path: \033[1;32mOK\033[0m" << std::endl;
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
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != NOT_FOUND || testRequest.RoutedPath != "") {
		std::cout << "Empty Path: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Empty Path: \033[1;32mOK\033[0m" << std::endl;
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
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != NOT_FOUND || testRequest.RoutedPath != "") {
		std::cout << "Incorrect Path: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Incorrect Path: \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 5-------------------//
//==========================================================//
	testRequest.RequestedPath = "/testing.html";
	testRequest.RequestIntegrity = OK_HTTP;
	this->testRequest.UsedRoute = empty;
	testRequest.RoutedPath = "";
//----------------------------------------------------------//
//======================Running Test 5======================//
//----------------------------------------------------------//
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != OK_HTTP || testRequest.RoutedPath != "/ha/ha/ha/testing.html") {
		std::cout << "requested file with same name as location: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "requested file with same name as location: \033[1;32mOK\033[0m" << std::endl;
	}

//==========================================================//
//-------------------Preparing for Test 6-------------------//
//==========================================================//
	testRequest.RequestedPath = "/index.html";
	testRequest.RequestIntegrity = OK_HTTP;
	this->testRequest.UsedRoute = empty;
	testRequest.RoutedPath = "";
//----------------------------------------------------------//
//======================Running Test 6======================//
//----------------------------------------------------------//
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != OK_HTTP || testRequest.RoutedPath != "/gerhard/content.html") {
		std::cout << "requested file with other (longer) path possible: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "requested file with other (longer) path possible: \033[1;32mOK\033[0m" << std::endl;
	}

	//==========================================================//
//-------------------Preparing for Test 7-------------------//
//==========================================================//
	testRequest.RequestedPath = "/testing/index.html";
	testRequest.RequestIntegrity = OK_HTTP;
	this->testRequest.UsedRoute = empty;
	testRequest.RoutedPath = "";
//----------------------------------------------------------//
//======================Running Test 7======================//
//----------------------------------------------------------//
	findRoute(testRequest, testSettings);
	if (testRequest.RequestIntegrity != OK_HTTP || testRequest.RoutedPath != "/content/braunbär.html") {
		std::cout << "requested file with other longer path: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "requested file with other longer path: \033[1;32mOK\033[0m" << std::endl;
	}


	std::cout <<"[1;34m-----checkIfMethodIsAllowedOnRoute-----[0m" << std::endl;
//==========================================================//
//-------------------Preparing for Test 1-------------------//
//==========================================================//
	testRequest.ReqType = GET;
	testRequest.RequestIntegrity = OK_HTTP;
	testRequest.UsedRoute.httpMethods = GET;
//----------------------------------------------------------//
//======================Running Test 1======================//
//----------------------------------------------------------//
	checkIfMethodIsAllowedOnRoute(testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "Allowed Method (one is allowed): \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Allowed Method (one is allowed): \033[1;32mOK\033[0m" << std::endl;
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
	checkIfMethodIsAllowedOnRoute(testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "Allowed Method (multiple are allowed): \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Allowed Method (multiple are allowed): \033[1;32mOK\033[0m" << std::endl;
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
	checkIfMethodIsAllowedOnRoute(testRequest);
	if (testRequest.RequestIntegrity != METHOD_NOT_ALLOWED) {
		std::cout << "Disallowed Method (one is allowed): \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Disallowed Method (one is allowed): \033[1;32mOK\033[0m" << std::endl;
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
	checkIfMethodIsAllowedOnRoute(testRequest);
	if (testRequest.RequestIntegrity != METHOD_NOT_ALLOWED) {
		std::cout << "Disallowed Method (multiple are allowed): \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Disallowed Method (multiple are allowed): \033[1;32mOK\033[0m" << std::endl;
	}

	std::cout <<"[1;34m-------------checkBodySize-------------[0m" << std::endl;
//==========================================================//
//-------------------Preparing for Test 1-------------------//
//==========================================================//
	testRequest.Body = "FOO";
	testSettings.clientMaxBodySize = 10;
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 1======================//
//----------------------------------------------------------//
	checkBodySize(testRequest,testSettings);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "Body size ok: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Body size ok: \033[1;32mOK\033[0m" << std::endl;
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
	checkBodySize(testRequest,testSettings);
	if (testRequest.RequestIntegrity != PAYLOAD_TO_LARGE) {
		std::cout << "Body size to big: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Body size to big: \033[1;32mOK\033[0m" << std::endl;
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
	checkBodySize(testRequest,testSettings);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "Body size same as clientMaxBodySize: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Body size same as clientMaxBodySize: \033[1;32mOK\033[0m" << std::endl;
	}

	std::cout <<"[1;34m-----------redirectionChecker-----------[0m" << std::endl;
//==========================================================//
//-------------------Preparing for Test 1-------------------//
//==========================================================//
	testRequest.UsedRoute.redirect = "";
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 1======================//
//----------------------------------------------------------//
	redirectionChecker(testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "Redirect empty: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Redirect empty: \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 2-------------------//
//==========================================================//
	testRequest.UsedRoute.redirect = "eeEEeeEeE";
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 2======================//
//----------------------------------------------------------//
	redirectionChecker(testRequest);
	if (testRequest.RequestIntegrity != MOVED_PERMANENTLY) {
		std::cout << "Redirect not empty: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Redirect not empty: \033[1;32mOK\033[0m" << std::endl;
	}

	std::cout <<"[1;34m------------checkContentType------------[0m" << std::endl;
//==========================================================//
//-------------------Preparing for Test 1-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 1======================//
//----------------------------------------------------------//
	checkContentType(testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "No content type: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "No content type: \033[1;32mOK\033[0m" << std::endl;
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
	checkContentType(testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "accepted content & subcontent type: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "accepted content & subcontent type: \033[1;32mOK\033[0m" << std::endl;
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
	checkContentType(testRequest);
	if (testRequest.RequestIntegrity != UNSUPPORTED_MEDIA_TYPE) {
		std::cout << "not accepted content type: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "not accepted content type: \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 4-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "text/rain"));
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 4======================//
//----------------------------------------------------------//
	checkContentType(testRequest);
	if (testRequest.RequestIntegrity != UNSUPPORTED_MEDIA_TYPE) {
		std::cout << "not accepted subContent type: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "not accepted subContent type: \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 5-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "text/form-data"));
	testRequest.RequestIntegrity = OK_HTTP;
//----------------------------------------------------------//
//======================Running Test 5======================//
//----------------------------------------------------------//
	checkContentType(testRequest);
	if (testRequest.RequestIntegrity != UNSUPPORTED_MEDIA_TYPE) {
		std::cout << "Mismatched Content and Subcontent type: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Mismatched Content and Subcontent type: \033[1;32mOK\033[0m" << std::endl;
	}
}

void Tests::testConfigParser() {

}

void Tests::testConfigLocationParser() {

}

void Tests::testHttpParser() {
	std::cout << std::endl << "\033[1;95mTesting the httpParser:\033[0m " << std::endl;
	std::cout <<"[1;34m-------------GetRequestType-------------[0m" << std::endl;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "GETTTT jfqwfqwfqwefq fwqef";
	this->testRequest.ReqType = INVALID;
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	GetRequestType(this->testRequest);
	if (testRequest.ReqType != INVALID) {
		std::cout << "invalid request: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "invalid request: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "GET /index.html HTTP/1.1\r\n";
	this->testRequest.ReqType = INVALID;
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	GetRequestType(this->testRequest);
	if (testRequest.ReqType != GET) {
		std::cout << "GET: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "GET: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 3-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "HEAD /index.html HTTP/1.1\r\n";
	this->testRequest.ReqType = INVALID;
	//----------------------------------------------------------//
	//======================Running Test 3======================//
	//----------------------------------------------------------//
	GetRequestType(this->testRequest);
	if (testRequest.ReqType != HEAD) {
		std::cout << "HEAD: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "HEAD: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 4-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "POST /index.html HTTP/1.1\r\n";
	this->testRequest.ReqType = INVALID;
	//----------------------------------------------------------//
	//======================Running Test 4======================//
	//----------------------------------------------------------//
	GetRequestType(this->testRequest);
	if (testRequest.ReqType != POST) {
		std::cout << "POST: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "POST: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 5-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "PUT /index.html HTTP/1.1\r\n";
	this->testRequest.ReqType = INVALID;
	//----------------------------------------------------------//
	//======================Running Test 5======================//
	//----------------------------------------------------------//
	GetRequestType(this->testRequest);
	if (testRequest.ReqType != PUT) {
		std::cout << "PUT: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "PUT: \033[1;32mOK\033[0m" << std::endl;
	}	//==========================================================//
	//-------------------Preparing for Test 6-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "DELETE /index.html HTTP/1.1\r\n";
	this->testRequest.ReqType = INVALID;
	//----------------------------------------------------------//
	//======================Running Test 6======================//
	//----------------------------------------------------------//
	GetRequestType(this->testRequest);
	if (testRequest.ReqType != DELETE) {
		std::cout << "DELETE: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "DELETE: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 7-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "CONNECT /index.html HTTP/1.1\r\n";
	this->testRequest.ReqType = INVALID;
	//----------------------------------------------------------//
	//======================Running Test 7======================//
	//----------------------------------------------------------//
	GetRequestType(this->testRequest);
	if (testRequest.ReqType != CONNECT) {
		std::cout << "CONNECT: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "CONNECT: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 8-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "OPTIONS /index.html HTTP/1.1\r\n";
	this->testRequest.ReqType = INVALID;
	//----------------------------------------------------------//
	//======================Running Test 8======================//
	//----------------------------------------------------------//
	GetRequestType(this->testRequest);
	if (testRequest.ReqType != OPTIONS) {
		std::cout << "OPTIONS: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "OPTIONS: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 9-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "TRACE /index.html HTTP/1.1\r\n";
	this->testRequest.ReqType = INVALID;
	//----------------------------------------------------------//
	//======================Running Test 9======================//
	//----------------------------------------------------------//
	GetRequestType(this->testRequest);
	if (testRequest.ReqType != TRACE) {
		std::cout << "TRACE: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "TRACE: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 10-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "PATCH /index.html HTTP/1.1\r\n";
	this->testRequest.ReqType = INVALID;
	//----------------------------------------------------------//
	//======================Running Test 10======================//
	//----------------------------------------------------------//
	GetRequestType(this->testRequest);
	if (testRequest.ReqType != PATCH) {
		std::cout << "PATCH: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "PATCH: \033[1;32mOK\033[0m" << std::endl;
	}

	std::cout <<"[1;34m-------------GetRequestPath-------------[0m" << std::endl;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "GET /index.html HTTP/1.1\r\n";
	this->testRequest.RoutedPath.clear();
	this->testRequest.RequestedPath.clear();
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	GetRequestedPath(this->testRequest);
	if (testRequest.RequestedPath != "/index.html") {
		std::cout << "Valid Request Pattern: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Valid Request Pattern: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "";
	this->testRequest.RoutedPath.clear();
	this->testRequest.RequestedPath.clear();
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	GetRequestedPath(this->testRequest);
	if (testRequest.RequestedPath != "") {
		std::cout << "Empty Request Pattern: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Empty Request Pattern: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 3-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "NoSpacesInThisTest\r\n";
	this->testRequest.RoutedPath.clear();
	this->testRequest.RequestedPath.clear();
	//----------------------------------------------------------//
	//======================Running Test 3======================//
	//----------------------------------------------------------//
	GetRequestedPath(this->testRequest);
	if (testRequest.RequestedPath != "") {
		std::cout << "Invalid Request Pattern 1: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Invalid Request Pattern 1: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 4-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "IHDEB MultipleSpaces in this test) wue0v";
	this->testRequest.RoutedPath.clear();
	this->testRequest.RequestedPath.clear();
	//----------------------------------------------------------//
	//======================Running Test 4======================//
	//----------------------------------------------------------//
	GetRequestedPath(this->testRequest);
	if (testRequest.RequestedPath != "MultipleSpaces") {
		std::cout << "Invalid Request Pattern 2: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Invalid Request Pattern 2: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 5-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "Only OneSpaceInThisTest!";
	this->testRequest.RoutedPath.clear();
	this->testRequest.RequestedPath.clear();
	//----------------------------------------------------------//
	//======================Running Test 5======================//
	//----------------------------------------------------------//
	GetRequestedPath(this->testRequest);
	if (testRequest.RequestedPath != "OneSpaceInThisTest!") {
		std::cout << "Invalid Request Pattern 3: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Invalid Request Pattern 3: \033[1;32mOK\033[0m" << std::endl;
	}

	std::cout <<"[1;34m--------decapitalizeHeaderFields--------[0m" << std::endl;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "GET / HTTP/1.1\r\nHost: www.example.com\r\nConnection: close\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nAccept-Language: en-US,en;q=0.9\r\n\r\n";
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	decapitalizeHeaderFields(this->testRequest.HeaderBuffer);
	if (testRequest.HeaderBuffer != "GET / HTTP/1.1\r\nhost: www.example.com\r\nconnection: close\r\nuser-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3\r\naccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\naccept-language: en-US,en;q=0.9\r\n\r\n") {
		std::cout << "Valid Header Pattern: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Valid Header Pattern: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "InVaLid StUfF";
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	decapitalizeHeaderFields(this->testRequest.HeaderBuffer);
	if (testRequest.HeaderBuffer != "InVaLid StUfF") {
		std::cout << "Single Line: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Single Line: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 3-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "";
	//----------------------------------------------------------//
	//======================Running Test 3======================//
	//----------------------------------------------------------//
	decapitalizeHeaderFields(this->testRequest.HeaderBuffer);
	if (testRequest.HeaderBuffer != "") {
		std::cout << "Empty Line: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Empty Line: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 4-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "AAAAAAAAAAAAA AAAAAAA AAAAA AAAA\nBBBBB BBBBB: BBBBB: BBBBB\n";
	//----------------------------------------------------------//
	//======================Running Test 4======================//
	//----------------------------------------------------------//
	decapitalizeHeaderFields(this->testRequest.HeaderBuffer);
	if (testRequest.HeaderBuffer != "AAAAAAAAAAAAA AAAAAAA AAAAA AAAA\nbbbbb bbbbb: BBBBB: BBBBB\n") {
		std::cout << "Multiple \":\"on line: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Multiple \":\"on line: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 5-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "a\nb:c";
	//----------------------------------------------------------//
	//======================Running Test 5======================//
	//----------------------------------------------------------//
	decapitalizeHeaderFields(this->testRequest.HeaderBuffer);
	if (testRequest.HeaderBuffer != "a\nb:c") {
		std::cout << "No \\n on end of line: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "No \\n on end of line: \033[1;32mOK\033[0m" << std::endl;
	}

	std::cout <<"[1;34m-----------extractHeaderFields----------[0m" << std::endl;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "GET / HTTP/1.1\r\nhost: www.example.com\r\nconnection: close\r\nuser-agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3\r\naccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\naccept-language: en-US,en;q=0.9\r\n\r\n";
	this->testRequest.HeaderFields.clear();
	std::map<std::string,std::string> testHeaderFieldMap;
	testHeaderFieldMap.insert(std::make_pair("connection","close"));
	testHeaderFieldMap.insert(std::make_pair("user-agent","Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3"));
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	extractHeaderFields(this->testRequest);
	if (testRequest.HeaderFields != testHeaderFieldMap) {
	std::cout << "Searched & not searched Header Fields: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Searched & not searched Header Fields: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer = "";
	this->testRequest.HeaderFields.clear();
	testHeaderFieldMap.clear();
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	extractHeaderFields(this->testRequest);
	if (testRequest.HeaderFields != testHeaderFieldMap) {
		std::cout << "not searched Header Fields: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "not searched Header Fields: \033[1;32mOK\033[0m" << std::endl;
	}

	std::cout <<"[1;34m---------------handleBody---------------[0m" << std::endl;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	this->testRequest.HeaderFields.clear();
	this->testRequest.RequestBuffer.clear();
	this->testRequest.Body.clear();
	this->testRequest.requestCompletlyRecieved = false;

	this->testRequest.HeaderFields.insert(std::make_pair("content-length","12"));
	this->testRequest.RequestBuffer = "a small body";
	std::string comparisonBody = "a small body";

	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	handleBody(this->testRequest);
	if (testRequest.Body != comparisonBody && testRequest.requestCompletlyRecieved != false) {
		std::cout << "correct body: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "correct body: \033[1;32mOK\033[0m" << std::endl;
	}

	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	this->testRequest.HeaderFields.clear();
	this->testRequest.RequestBuffer.clear();
	this->testRequest.Body.clear();
	this->testRequest.requestCompletlyRecieved = false;

	this->testRequest.HeaderFields.insert(std::make_pair("content-length","12"));
	this->testRequest.RequestBuffer = "a not so small body";
	comparisonBody = "a not so sma";

	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	handleBody(this->testRequest);
	if (testRequest.Body != comparisonBody && testRequest.requestCompletlyRecieved != false) {
		std::cout << "content length to small: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "content length to small: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 3-------------------//
	//==========================================================//
	this->testRequest.HeaderFields.clear();
	this->testRequest.RequestBuffer.clear();
	this->testRequest.Body.clear();
	this->testRequest.requestCompletlyRecieved = false;

	this->testRequest.HeaderFields.insert(std::make_pair("content-length","12"));
	this->testRequest.RequestBuffer = "a not";
	comparisonBody = "";

	//----------------------------------------------------------//
	//======================Running Test 3======================//
	//----------------------------------------------------------//
	handleBody(this->testRequest);
	if (testRequest.Body != comparisonBody && testRequest.requestCompletlyRecieved != true) {
		std::cout << "RequestBuffer to small: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "RequestBuffer to small: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 4-------------------//
	//==========================================================//
	this->testRequest.HeaderFields.clear();
	this->testRequest.RequestBuffer.clear();
	this->testRequest.Body.clear();
	this->testRequest.requestCompletlyRecieved = false;

	this->testRequest.HeaderFields.insert(std::make_pair("content-length","5"));
	this->testRequest.RequestBuffer = "a not so small body";
	comparisonBody = "a not";

	//----------------------------------------------------------//
	//======================Running Test 4======================//
	//----------------------------------------------------------//
	handleBody(this->testRequest);
	if (testRequest.Body != comparisonBody && testRequest.requestCompletlyRecieved != false) {
		std::cout << "Body with excess length: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Body with excess length: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 5-------------------//
	//==========================================================//
	this->testRequest.HeaderFields.clear();
	this->testRequest.RequestBuffer.clear();
	this->testRequest.Body.clear();
	this->testRequest.requestCompletlyRecieved = false;

	this->testRequest.HeaderFields.insert(std::make_pair("transfer-encoding","chunked"));
	this->testRequest.RequestBuffer = "5\r\na not\r\n7\r\n small \r\n4\r\nbody\r\n0\r\n\r\n";
	comparisonBody = "a not small body";

	//----------------------------------------------------------//
	//======================Running Test 5======================//
	//----------------------------------------------------------//
	handleBody(this->testRequest);
	if (testRequest.Body != comparisonBody && testRequest.requestCompletlyRecieved != false) {
		std::cout << "Correctly Chunked Body: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Correctly Chunked Body: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 6-------------------//
	//==========================================================//
	this->testRequest.HeaderFields.clear();
	this->testRequest.RequestBuffer.clear();
	this->testRequest.Body.clear();
	this->testRequest.requestCompletlyRecieved = false;

	this->testRequest.HeaderFields.insert(std::make_pair("content-length","5"));
	this->testRequest.HeaderFields.insert(std::make_pair("transfer-encoding","chunked"));
	this->testRequest.RequestBuffer = "5\r\na not\r\n7\r\n small \r\n4\r\nbody\r\n0\r\n\r\n";
	comparisonBody = "a not small body";

	//----------------------------------------------------------//
	//======================Running Test 6======================//
	//----------------------------------------------------------//
	handleBody(this->testRequest);
	if (testRequest.Body != comparisonBody && testRequest.requestCompletlyRecieved != false) {
		std::cout << "Content-length and chunked: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Content-length and chunked: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 7-------------------//
	//==========================================================//
	this->testRequest.HeaderFields.clear();
	this->testRequest.RequestBuffer.clear();
	this->testRequest.Body.clear();
	this->testRequest.requestCompletlyRecieved = false;

	this->testRequest.HeaderFields.insert(std::make_pair("content-length","5"));
	this->testRequest.HeaderFields.insert(std::make_pair("transfer-encoding","chunked"));
	this->testRequest.RequestBuffer = "5\r\na not\r\n7\r\n small \r\n3\r\nbod\r\n1\r\ny\r\n";
	comparisonBody = "";

	//----------------------------------------------------------//
	//======================Running Test 7======================//
	//----------------------------------------------------------//
	handleBody(this->testRequest);
	if (testRequest.Body != comparisonBody && testRequest.requestCompletlyRecieved != true) {
		std::cout << "no last Chunk: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "no last Chunk: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 8-------------------//
	//==========================================================//
	this->testRequest.HeaderFields.clear();
	this->testRequest.RequestBuffer.clear();
	this->testRequest.Body.clear();
	this->testRequest.requestCompletlyRecieved = false;

	this->testRequest.HeaderFields.insert(std::make_pair("content-length","5"));
	this->testRequest.HeaderFields.insert(std::make_pair("transfer-encoding","chunked"));
	this->testRequest.RequestBuffer = "20\r\na not\r\n7\r\n small \r\n4\r\nbod\r\n0\r\n\r\n";
	comparisonBody = "a not\r\n7\r\n small \r\n3\r\nbod\r\n1\r\ny\r small body";

	//----------------------------------------------------------//
	//======================Running Test 8======================//
	//----------------------------------------------------------//
	handleBody(this->testRequest);
	if (testRequest.Body != comparisonBody && testRequest.requestCompletlyRecieved != false) {
		std::cout << "Corrupted Chunking: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Corrupted Chunking: \033[1;32mOK\033[0m" << std::endl;
	}

	std::cout <<"[1;34m---------checkMultipartDelimiter--------[0m" << std::endl;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	this->testRequest.HeaderBuffer.clear();
	this->testRequest.HeaderFields.clear();
	this->testRequest.RequestIntegrity = OK_HTTP;
	testRequest.HeaderFields.insert(std::make_pair("connection","close"));
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	checkMultipartDelimiter(this->testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "No Multipart: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "No Multipart: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	this->testRequest.HeaderFields.clear();
	this->testRequest.RequestIntegrity = OK_HTTP;
	testRequest.HeaderFields.insert(std::make_pair("content-type","multipart/form-data"));
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	checkMultipartDelimiter(this->testRequest);
	if (testRequest.RequestIntegrity != BAD_REQUEST) {
		std::cout << "Invalid Multipart: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Invalid Multipart: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 3-------------------//
	//==========================================================//
	this->testRequest.HeaderFields.clear();
	this->testRequest.RequestIntegrity = OK_HTTP;
	testRequest.HeaderFields.insert(std::make_pair("content-type","multipart/form-data; boundary=delim"));
	//----------------------------------------------------------//
	//======================Running Test 3======================//
	//----------------------------------------------------------//
	checkMultipartDelimiter(this->testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "Valid Multipart 1: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Valid Multipart 1: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 4-------------------//
	//==========================================================//
	this->testRequest.HeaderFields.clear();
	this->testRequest.RequestIntegrity = OK_HTTP;
	testRequest.HeaderFields.insert(std::make_pair("content-type","multipart/form-data;boundary=delim"));
	//----------------------------------------------------------//
	//======================Running Test 4======================//
	//----------------------------------------------------------//
	checkMultipartDelimiter(this->testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "Valid Multipart 2: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Valid Multipart2 : \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 5-------------------//
	//==========================================================//
	this->testRequest.HeaderFields.clear();
	this->testRequest.RequestIntegrity = OK_HTTP;
	testRequest.HeaderFields.insert(std::make_pair("content-type","multipart/form-data;               boundary=delim"));
	//----------------------------------------------------------//
	//======================Running Test 5======================//
	//----------------------------------------------------------//
	checkMultipartDelimiter(this->testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "Valid Multipart 2: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Valid Multipart2 : \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 6-------------------//
	//==========================================================//
	this->testRequest.HeaderFields.clear();
	this->testRequest.RequestIntegrity = OK_HTTP;
	//----------------------------------------------------------//
	//======================Running Test 6======================//
	//----------------------------------------------------------//
	checkMultipartDelimiter(this->testRequest);
	if (testRequest.RequestIntegrity != OK_HTTP) {
		std::cout << "Empty Header Fields: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Empty Header Fields: \033[1;32mOK\033[0m" << std::endl;
	}

}

void Tests::testServer() {

}

void Tests::testStatusCodes() {
	statusCode	testCode;
	std::string	expectedOutput;
	std::string	output;
	std::cout <<"[1;34m-----------getStatusCodeMessage----------[0m" << std::endl;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	testCode = OK_HTTP;
	expectedOutput = "OK";
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	output = getStatusCodeMessage(testCode);
	if (expectedOutput != output) {
		std::cout << "Return right message to input status code: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Return right message to input status code: \033[1;32mOK\033[0m" << std::endl;
	}
	std::cout <<"[1;34m-----------getStatusCodeDescription----------[0m" << std::endl;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	testCode = HTTP_VERSION_NOT_SUPPORTED;
	expectedOutput = "HTTP Version Not Supported";
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	output = getStatusCodeDescription(testCode);
	if (expectedOutput != output) {
		std::cout << "Return right description to input status code: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Return right description to input status code: \033[1;32mOK\033[0m" << std::endl;
	}
}

void Tests::testBodyGeneration(){
	std::cout << "\033[1;95mTesting the bodyGeneration:\033[0m " << std::endl;
	MethodExecutor testObj = MethodExecutor(&(this->testSettings));
	testObj.silent = this->silent;
	std::cout <<"[1;34m-----------checkAndReplace----------[0m" << std::endl;
	// unit tests implemented in source file, since static function
	testObj.testCheckandReplace();
	std::cout <<"[1;34m-----------createIndexPage----------[0m" << std::endl;
	testObj.testCreateIndexPage();
	std::cout <<"[1;34m-----------generateErrorBody----------[0m" << std::endl;
	std::string errorPagePath1 = "tests/testContent/dummyErrorPage.txt";
	std::string errorPagePath2 = "tests/testContent/notExisting.txt";
	testSettings.errorPages.insert(std::make_pair(INTERNAL_SERVER_ERROR, errorPagePath1));
	testSettings.errorPages.insert(std::make_pair(NOT_FOUND, errorPagePath2));
	testObj.testGenerateErrorBody();
}

void Tests::testHeaderGeneration(){
	std::cout << "\033[1;95mTesting the headerGeneration:\033[0m " << std::endl;
	MethodExecutor testObj = MethodExecutor(&(this->testSettings));
	testObj.silent = this->silent;
	std::cout <<"[1;34m-----------generateDateField----------[0m" << std::endl;
	// unit tests implemented in source file, since static function
	testObj.testGenerateDateField();
	std::cout <<"[1;34m-----------generateContentLengthField----------[0m" << std::endl;
	testObj.testGenerateContentLengthField();
	std::cout <<"[1;34m-----------testGenerateAllowField----------[0m" << std::endl;
	testObj.testGenerateAllowField();
	std::cout <<"[1;34m-----------generateSpecialErrorFields----------[0m" << std::endl;
	testObj.testGenerateSpecialErrorFields();
	std::cout <<"[1;34m-----------generateCommonHeaderField----------[0m" << std::endl;
	testSettings.serverName = "example-name";
	testObj.testGenerateCommonHeaderField();
	std::cout <<"[1;34m-----------writeHeaderFields----------[0m" << std::endl;
	testObj.testWriteHeaderFields();
}

void Tests::testing() {
	testConfigLocationParser();
	testConfigParser();
	testHttpInterpreter();
	testHttpParser();
	testMethodExecutor();
	testBodyGeneration();
	testHeaderGeneration();
	testServer();
	testStatusCodes();
}

Tests::Tests(int argument): silent(false) {
	if (argument == SILENT) {
		this->silent = true;
	}
}
