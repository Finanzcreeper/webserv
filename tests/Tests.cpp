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

	std::cout <<"[1;34m-------MultipartDelimiterValidation-----[0m" << std::endl;
	std::string delimiter;
	std::string result;
//==========================================================//
//-------------------Preparing for Test 1-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=\"delimiterTesting\""));
	testRequest.RequestIntegrity = OK_HTTP;
	Multipart testMultipart;
	delimiter = "\r\n--delimiterTesting";
//----------------------------------------------------------//
//======================Running Test 1======================//
//----------------------------------------------------------//
	result = MultipartDelimiterValidation(testRequest);
	if ( testRequest.RequestIntegrity != OK_HTTP || result != delimiter) {
		std::cout << "delimiter in \"\": \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "delimiter in \"\": \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 2-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=delimiterTesting"));
	testRequest.RequestIntegrity = OK_HTTP;
	delimiter = "\r\n--delimiterTesting";
//----------------------------------------------------------//
//======================Running Test 2======================//
//----------------------------------------------------------//
	result = MultipartDelimiterValidation(testRequest);
	if ( testRequest.RequestIntegrity  != OK_HTTP || result != delimiter) {
		std::cout << "delimiter without \"\": \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "delimiter without \"\": \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 3-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=a"));
	testRequest.RequestIntegrity = OK_HTTP;
	delimiter = "\r\n--a";
//----------------------------------------------------------//
//======================Running Test 3======================//
//----------------------------------------------------------//
	result = MultipartDelimiterValidation(testRequest);
	if ( testRequest.RequestIntegrity  != OK_HTTP || result != delimiter) {
		std::cout << "single char delimiter no \"\": \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "single char delimiter no \"\": \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 4-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"));
	testRequest.RequestIntegrity = OK_HTTP;
	delimiter = "\r\n--qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq";
//----------------------------------------------------------//
//======================Running Test 4======================//
//----------------------------------------------------------//
	result = MultipartDelimiterValidation(testRequest);
	if ( testRequest.RequestIntegrity  != OK_HTTP || result != delimiter) {
		std::cout << "70 char delimiter no \"\": \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "70 char delimiter no \"\": \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 5-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=\"a\""));
	testRequest.RequestIntegrity = OK_HTTP;
	delimiter = "\r\n--a";
//----------------------------------------------------------//
//======================Running Test 5======================//
//----------------------------------------------------------//
	result = MultipartDelimiterValidation(testRequest);
	if ( testRequest.RequestIntegrity  != OK_HTTP || result != delimiter) {
		std::cout << "single char delimiter with \"\": \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "single char delimiter with \"\": \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 6-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=\"qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq\""));
	testRequest.RequestIntegrity = OK_HTTP;
	delimiter = "\r\n--qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq";
//----------------------------------------------------------//
//======================Running Test 6======================//
//----------------------------------------------------------//
	result = MultipartDelimiterValidation(testRequest);
	if ( testRequest.RequestIntegrity  != OK_HTTP || result != delimiter) {
		std::cout << "70 char delimiter with \"\": \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "70 char delimiter with \"\": \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 7-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=\"\""));
	testRequest.RequestIntegrity = OK_HTTP;
	delimiter = "";
//----------------------------------------------------------//
//======================Running Test 7======================//
//----------------------------------------------------------//
	result = MultipartDelimiterValidation(testRequest);
	if ( testRequest.RequestIntegrity != BAD_REQUEST || result != delimiter) {
		std::cout << "just\"\": \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "just \"\": \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 8-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary="));
	testRequest.RequestIntegrity = OK_HTTP;
	delimiter = "";
//----------------------------------------------------------//
//======================Running Test 8======================//
//----------------------------------------------------------//
	result = MultipartDelimiterValidation(testRequest);
	if ( testRequest.RequestIntegrity != BAD_REQUEST || result != delimiter) {
		std::cout << "empty boundary: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "empty boundary: \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 9-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"));
	testRequest.RequestIntegrity = OK_HTTP;
	delimiter = "";
//----------------------------------------------------------//
//======================Running Test 9======================//
//----------------------------------------------------------//
	result = MultipartDelimiterValidation(testRequest);
	if ( testRequest.RequestIntegrity != BAD_REQUEST || result != delimiter) {
		std::cout << "delimiter to long by one: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "delimiter to long by one: \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 10-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq"));
	testRequest.RequestIntegrity = OK_HTTP;
	delimiter = "";
//----------------------------------------------------------//
//======================Running Test 10======================//
//----------------------------------------------------------//
	result = MultipartDelimiterValidation(testRequest);
	if ( testRequest.RequestIntegrity != BAD_REQUEST || result != delimiter) {
		std::cout << "delimiter to long: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "delimiter to long: \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 11-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=simple "));
	testRequest.RequestIntegrity = OK_HTTP;
	delimiter = "";
//----------------------------------------------------------//
//======================Running Test 11======================//
//----------------------------------------------------------//
	result = MultipartDelimiterValidation(testRequest);
	if ( testRequest.RequestIntegrity  != BAD_REQUEST || result != delimiter) {
		std::cout << "Space as last char: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Space as last char: \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 12-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=abcdefghijklmnopqrstuvwxyz'()+_,-./:=?ABCDEFGHIJKLMNOPQRSTUVWXYZ"));
	testRequest.RequestIntegrity = OK_HTTP;
	delimiter = "\r\n--abcdefghijklmnopqrstuvwxyz'()+_,-./:=?ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//----------------------------------------------------------//
//======================Running Test 12======================//
//----------------------------------------------------------//
	result = MultipartDelimiterValidation(testRequest);
	if ( testRequest.RequestIntegrity  != OK_HTTP || result != delimiter) {
		std::cout << "All allowed symbols 1: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "All allowed symbols 1: \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 13-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=0123456789"));
	testRequest.RequestIntegrity = OK_HTTP;
	delimiter = "\r\n--0123456789";
//----------------------------------------------------------//
//======================Running Test 13======================//
//----------------------------------------------------------//
	result = MultipartDelimiterValidation(testRequest);
	if ( testRequest.RequestIntegrity  != OK_HTTP || result != delimiter) {
		std::cout << "All allowed symbols 2: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "All allowed symbols 2: \033[1;32mOK\033[0m" << std::endl;
	}
//==========================================================//
//-------------------Preparing for Test 14-------------------//
//==========================================================//
	testRequest.HeaderFields.clear();
	testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=#"));
	testRequest.RequestIntegrity = OK_HTTP;
	delimiter = "";
//----------------------------------------------------------//
//======================Running Test 14======================//
//----------------------------------------------------------//
	result = MultipartDelimiterValidation(testRequest);
	if ( testRequest.RequestIntegrity  != BAD_REQUEST || result != delimiter) {
		std::cout << "Disallowed symbol: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Disallowed symbol: \033[1;32mOK\033[0m" << std::endl;
	}
	std::cout <<"[1;34m----------------Multipart---------------[0m" << std::endl;
	{
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
		testRequest.HeaderFields.clear();
		testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=delimiterTesting"));
		testRequest.HeaderFields.insert(std::make_pair("content-length", "396"));
		testRequest.RequestIntegrity = OK_HTTP;
		testRequest.Body = "\r\n--delimiterTesting\r\nContent-Disposition: form-data; name=\"text\"\r\n\r\ntext default\r\n--delimiterTesting\r\nContent-Disposition: form-data; name=\"file1\"; filename=\"a.txt\"\r\nContent-Type: text/plain\r\n\r\nContent of a.txt.\r\n--delimiterTesting\r\nContent-Disposition: form-data; name=\"file2\"; filename=\"a.html\"\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><title>Content of a.html.</title>\r\n--delimiterTesting--";

		std::vector<Multipart> testBodyParts;
		testMultipart.MultipartHeaderFields.clear();
		testMultipart.Body.clear();
		testMultipart.MultipartHeaderFields.insert(std::make_pair("Content-Disposition", "form-data; name=\"text\""));
		testMultipart.Body = "text default";
		testBodyParts.push_back(testMultipart);

		testMultipart.MultipartHeaderFields.clear();
		testMultipart.Body.clear();
		testMultipart.MultipartHeaderFields.insert(std::make_pair("Content-Disposition","form-data; name=\"file1\"; filename=\"a.txt\""));
		testMultipart.MultipartHeaderFields.insert((std::make_pair("Content-Type","text/plain")));
		testMultipart.Body = "Content of a.txt.";
		testBodyParts.push_back(testMultipart);

		testMultipart.MultipartHeaderFields.clear();
		testMultipart.Body.clear();
		testMultipart.MultipartHeaderFields.insert(std::make_pair("Content-Disposition","form-data; name=\"file2\"; filename=\"a.html\""));
		testMultipart.MultipartHeaderFields.insert(std::make_pair("Content-Type","text/html"));
		testMultipart.Body = "<!DOCTYPE html><title>Content of a.html.</title>";
		testBodyParts.push_back(testMultipart);
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
		handleMultipart(testRequest);
		bool OK = true;
		if (testRequest.RequestIntegrity == OK_HTTP) {
			std::vector<Multipart>::iterator mit;
			std::vector<Multipart>::iterator testmit;
			testmit = testBodyParts.begin();
			mit = testRequest.bodyParts.begin();
			while (mit != testBodyParts.end() && testmit != testBodyParts.end()) {
				if (mit->MultipartHeaderFields != testmit->MultipartHeaderFields) {
					std::cout << "Header wrong" << std::endl;


					std::map<std::string,std::string>::iterator Headt;
					std::map<std::string,std::string>::iterator Head;
					Head = mit->MultipartHeaderFields.begin();
					Headt = testmit->MultipartHeaderFields.begin();
					while (Head != mit->MultipartHeaderFields.end() && Headt != testmit->MultipartHeaderFields.end()) {
						std::cout << "\033[1;31mgot VS wanted\033[0m" << std::endl;
						std::cout << "|" << Head->first << "|" << " VS " << "|" << Headt->first << "|" << std::endl;
						std::cout << "++++++++++++++++++++++++++++++++++" << std::endl;
						std::cout << "|" << Head->second << "|" << " VS " << "|" << Headt->second << "|" << std::endl;
						++Head;
						++Headt;
					}

					OK = false;
				}
				if (mit->Body != testmit->Body) {
					std::cout << "Body wrong" << std::endl;
					std::cout << mit->Body << " VS " << testmit->Body << std::endl;
					std::cout << "-----------------------------------------------------" << std::endl;
					OK = false;
				}
				++mit;
				++testmit;
			}
		} else {
			OK = false;
		}
		if (OK != true) {
			std::cout << "normal Multipart: \033[1;31mFAILED\033[0m" << std::endl;
		} else if (this->silent == false) {
			std::cout << "normal Multipart: \033[1;32mOK\033[0m" << std::endl;
		}
	}
	{
		//==========================================================//
		//-------------------Preparing for Test 2-------------------//
		//==========================================================//
		testRequest.bodyParts.clear();
		testRequest.HeaderFields.clear();
		testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=delimiterTesting"));
		testRequest.RequestIntegrity = OK_HTTP;
		testRequest.Body = "\r\n--delimiterTesting\r\nContent-Disposition: form-data; name=\"text\"\r\n\r\ntext default\r\n--brokendelimiterTesting\r\nContent-Disposition: form-data; name=\"file1\"; filename=\"a.txt\"\r\nContent-Type: text/plain\r\n\r\nContent of a.txt.\r\n--delimiterTesting\r\nContent-Disposition: form-data; name=\"file2\"; filename=\"a.html\"\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><title>Content of a.html.</title>\r\n--delimiterTesting--";
		testRequest.HeaderFields.insert(std::make_pair("content-length", "396"));
		std::vector<Multipart> testBodyParts;
		testBodyParts.clear();

		testMultipart.MultipartHeaderFields.clear();
		testMultipart.Body.clear();
		testMultipart.MultipartHeaderFields.insert(std::make_pair("Content-Disposition", "form-data; name=\"text\""));
		testMultipart.Body = "text default";
		testBodyParts.push_back(testMultipart);

		testMultipart.MultipartHeaderFields.clear();
		testMultipart.Body.clear();
		testMultipart.MultipartHeaderFields.insert(std::make_pair("Content-Disposition","form-data; name=\"file1\"; filename=\"a.txt\""));
		testMultipart.MultipartHeaderFields.insert((std::make_pair("Content-Type","text/plain")));
		testMultipart.Body = "Content of a.txt.";
		testBodyParts.push_back(testMultipart);

		testMultipart.MultipartHeaderFields.clear();
		testMultipart.Body.clear();
		testMultipart.MultipartHeaderFields.insert(std::make_pair("Content-Disposition","form-data; name=\"file2\"; filename=\"a.html\""));
		testMultipart.MultipartHeaderFields.insert(std::make_pair("Content-Type","text/html"));
		testMultipart.Body = "<!DOCTYPE html><title>Content of a.html.</title>";
		std::vector<Multipart>::iterator mit;
		std::vector<Multipart>::iterator testmit;
		testBodyParts.push_back(testMultipart);
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
		handleMultipart(testRequest);
		if (testRequest.RequestIntegrity != BAD_REQUEST) {
			std::cout << "Wrong length body: \033[1;31mFAILED\033[0m" << std::endl;
		} else if (this->silent == false) {
			std::cout << "Wrong length body: \033[1;32mOK\033[0m" << std::endl;
		}
	}
	{
		//==========================================================//
		//-------------------Preparing for Test 3-------------------//
		//==========================================================//
		testRequest.HeaderFields.clear();
		testRequest.HeaderFields.insert(std::make_pair("content-type", "multipart/form-data; boundary=delimiterTesting"));
		testRequest.HeaderFields.insert(std::make_pair("content-length", "379"));
		testRequest.RequestIntegrity = OK_HTTP;
		testRequest.Body = "\r\n--delimiterTesting\r\nContent-Disposition: form-data; name=\"text\"\r\n\r\ntext default\r\n--delimiterTesting\r\nContent-Disposition: form-data; name=\"file1\"; filename=\"a.txt\"\r\nContent-Type: text/plain\r\n\r\n\r\n--delimiterTesting\r\nContent-Disposition: form-data; name=\"file2\"; filename=\"a.html\"\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE html><title>Content of a.html.</title>\r\n--delimiterTesting--";

		std::vector<Multipart> testBodyParts;
		testMultipart.MultipartHeaderFields.clear();
		testMultipart.Body.clear();
		testMultipart.MultipartHeaderFields.insert(std::make_pair("Content-Disposition", "form-data; name=\"text\""));
		testMultipart.Body = "text default";
		testBodyParts.push_back(testMultipart);

		testMultipart.MultipartHeaderFields.clear();
		testMultipart.Body.clear();
		testMultipart.MultipartHeaderFields.insert(std::make_pair("Content-Disposition","form-data; name=\"file1\"; filename=\"a.txt\""));
		testMultipart.MultipartHeaderFields.insert((std::make_pair("Content-Type","text/plain")));
		testMultipart.Body = "";
		testBodyParts.push_back(testMultipart);

		testMultipart.MultipartHeaderFields.clear();
		testMultipart.Body.clear();
		testMultipart.MultipartHeaderFields.insert(std::make_pair("Content-Disposition","form-data; name=\"file2\"; filename=\"a.html\""));
		testMultipart.MultipartHeaderFields.insert(std::make_pair("Content-Type","text/html"));
		testMultipart.Body = "<!DOCTYPE html><title>Content of a.html.</title>";
		testBodyParts.push_back(testMultipart);
		//----------------------------------------------------------//
		//======================Running Test 3======================//
		//----------------------------------------------------------//
		handleMultipart(testRequest);
		bool OK = true;
		if (testRequest.RequestIntegrity == OK_HTTP) {
			std::vector<Multipart>::iterator mit;
			std::vector<Multipart>::iterator testmit;
			testmit = testBodyParts.begin();
			mit = testRequest.bodyParts.begin();
			while (mit != testBodyParts.end() && testmit != testBodyParts.end()) {
				if (mit->MultipartHeaderFields != testmit->MultipartHeaderFields) {
					std::cout << "Header wrong" << std::endl;


					std::map<std::string,std::string>::iterator Headt;
					std::map<std::string,std::string>::iterator Head;
					Head = mit->MultipartHeaderFields.begin();
					Headt = testmit->MultipartHeaderFields.begin();
					while (Head != mit->MultipartHeaderFields.end() && Headt != testmit->MultipartHeaderFields.end()) {
						std::cout << "\033[1;31mgot VS wanted\033[0m" << std::endl;
						std::cout << "|" << Head->first << "|" << " VS " << "|" << Headt->first << "|" << std::endl;
						std::cout << "++++++++++++++++++++++++++++++++++" << std::endl;
						std::cout << "|" << Head->second << "|" << " VS " << "|" << Headt->second << "|" << std::endl;
						++Head;
						++Headt;
					}

					OK = false;
				}
				if (mit->Body != testmit->Body) {
					std::cout << "Body wrong" << std::endl;
					std::cout  << "|" << mit->Body << "|"  << " VS " << "|"  << testmit->Body << "|"  << std::endl;
					std::cout << "-----------------------------------------------------" << std::endl;
					OK = false;
				}
				++mit;
				++testmit;
			}
		} else {
			OK = false;
		}
		if (OK != true) {
			std::cout << "No body in one multipart: \033[1;31mFAILED\033[0m" << std::endl;
		} else if (this->silent == false) {
			std::cout << "No body in one multipart: \033[1;32mOK\033[0m" << std::endl;
		}
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

	std::cout <<"[1;34m----------------handlBody---------------[0m" << std::endl;
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

void Tests::testCgiHandler(){
	std::cout << "\033[1;95mTesting the cgiHandler:\033[0m " << std::endl;
	Request requ;
	Response resp;
	t_server settings;

	std::cout <<"[1;34m-----------prepareEnvVariables----------[0m" << std::endl;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	settings.host = "127.0.0.65";
	settings.port = "500";
	settings.serverName = "testServer";
	requ.HeaderFields["content-length"] = "40";
	requ.HeaderFields["content-type"] = "text/plain";
	requ.HeaderFields["authorization"] = "userxy";
	requ.ReqType = GET;
	requ.RequestedPath = "testPath2?here_begins_query_string";
	requ.RoutedPath = "content/testPath1?here_begins_query_string";
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	std::map<std::string,std::string> output = prepareEnvVariables(requ, &settings);
	std::string concat_string;
	std::map<std::string,std::string>::iterator it = output.begin();
	while (it != output.end()){
		concat_string.append(it->first + "=" + it->second + '\n');
		it++;
	}
	if (concat_string != "AUTH_TYPE=userxy\nCONTENT_LENGTH=40\nCONTENT_TYPE=text/plain\n\
GATEWAY_INTERFACE=CGI/1.1\nQUERY_STRING=here_begins_query_string\nREMOTE_ADDR=127.0.0.65\nREMOTE_IDENT=userxy\n\
REMOTE_USER=userxy\nREQUEST_METHOD=GET\nSCRIPT_NAME=content/testPath1\nSERVER_NAME=testServer\n\
SERVER_PORT=500\nSERVER_PROTOCOL=HTTP/1.1\nSERVER_SOFTWARE=webserv\n"){
		std::cout << "All mandatory cgi headers get: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "All mandatory cgi headers get: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	settings.host = "127.0.0.65";
	settings.port = "500";
	settings.serverName = "testServer";
	requ.HeaderFields["content-length"] = "40";
	requ.HeaderFields["content-type"] = "text/plain";
	requ.HeaderFields["authorization"] = "userxy";
	requ.ReqType = POST;
	requ.RequestedPath = "testPath2";
	requ.RoutedPath = "content/testPath1";
	requ.Body = "here_begins_query_string";
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	output = prepareEnvVariables(requ, &settings);
	concat_string = "";
	it = output.begin();
	while (it != output.end()){
		concat_string.append(it->first + "=" + it->second + '\n');
		it++;
	}
	if (concat_string != "AUTH_TYPE=userxy\nCONTENT_LENGTH=40\nCONTENT_TYPE=text/plain\n\
GATEWAY_INTERFACE=CGI/1.1\nQUERY_STRING=here_begins_query_string\nREMOTE_ADDR=127.0.0.65\nREMOTE_IDENT=userxy\n\
REMOTE_USER=userxy\nREQUEST_METHOD=POST\nSCRIPT_NAME=content/testPath1\nSERVER_NAME=testServer\n\
SERVER_PORT=500\nSERVER_PROTOCOL=HTTP/1.1\nSERVER_SOFTWARE=webserv\n"){
		std::cout << "All mandatory cgi headers post: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "All mandatory cgi headers post: \033[1;32mOK\033[0m" << std::endl;
	}
	std::cout <<"[1;34m-----------executeCGI----------[0m" << std::endl;
	//==========================================================//
	//-------------------Preparing for Test 3-------------------//
	//==========================================================//
	requ.Body = "blabla";
	requ.HeaderFields["content-length"] = "6";
	requ.HeaderFields["content-type"] = "test";
	requ.Body = "bodyContent";
	requ.RoutedPath = "tests/testContent/cgi/test.py?thisisthestring";
	requ.ReqType = GET;
	resp.responseBuffer = "";
	resp.httpStatus = OK_HTTP;
	//----------------------------------------------------------//
	//======================Running Test 3======================//
	//----------------------------------------------------------//
	executeCGI(requ, resp, &settings);
	if (resp.responseBuffer != "hallo: thisisthestring\n"){
		std::cout << "Simple cgi script get: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Simple cgi script get: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 4-------------------//
	//==========================================================//
	requ.Body = "blabla";
	requ.HeaderFields["content-length"] = "6";
	requ.HeaderFields["content-type"] = "test";
	requ.Body = "bodyContent";
	requ.RoutedPath = "tests/testContent/cgi/test.py";
	requ.ReqType = POST;
	resp.responseBuffer = "";
	resp.httpStatus = OK_HTTP;
	//----------------------------------------------------------//
	//======================Running Test 4======================//
	//----------------------------------------------------------//
	executeCGI(requ, resp, &settings);
	if (resp.responseBuffer != "hallo: bodyContent\n"){
		std::cout << "Simple cgi script post: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Simple cgi script post: \033[1;32mOK\033[0m" << std::endl;
	}
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
	testCgiHandler();
}

Tests::Tests(int argument): silent(false) {
	if (argument == SILENT) {
		this->silent = true;
	}
}
