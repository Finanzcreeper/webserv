#include "Tests.hpp"

void Tests::testMethodExecutor(void){
	std::cout << "\033[1;95mTesting the methodExecutor:\033[0m " << std::endl;
	MethodExecutor testObj = MethodExecutor(&(this->testSettings));
	testObj.silent = this->silent;
	std::cout <<"[1;34m-----------writeStatusLine----------[0m" << std::endl;
	testObj.testWriteStatusLine();
	std::cout <<"[1;34m-----------executeDelete----------[0m" << std::endl;
	testObj.testExecuteDelete();
	std::cout <<"[1;34m-----------executePost----------[0m" << std::endl;
	testObj.testExecutePost();
	std::cout <<"[1;34m-----------executeGet----------[0m" << std::endl;
	testObj.testExecuteGet();
	std::cout <<"[1;34m-----------wrapperRequest----------[0m" << std::endl;
	testObj.testWrapperRequest();
}

void	MethodExecutor::testCreateIndexPage(){
	Response resp;
	std::string path;
	std::string requPath;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	path = "tests/testContent/indexPageTestsDir";
	requPath = "/testDir/indexPageTestDir";
	resp.httpStatus = OK_HTTP;
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	_createIndexPage(path, requPath, resp);
	if (resp.httpStatus != OK_HTTP || resp.body.find(">veryUniqueFileName.txt<") != 813) {
		std::cout << "Simple index page: \033[1;31mFAILED\033[0m" << std::endl;
		if (resp.httpStatus != OK_HTTP)
			std::cout << "Status is not OK_HTTP" << std::endl;
		if (resp.body.find(">veryUniqueFileName.txt<") != 813)
			std::cout << "Position of filename in index template doesnt match hardcoded value" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Simple index page: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	path = "/tests/invalidPath";
	requPath = "/invalidPath";
	resp.body.clear();
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	_createIndexPage(path, requPath, resp);
	if (resp.httpStatus != INTERNAL_SERVER_ERROR || resp.body != "") {
		std::cout << "Invalid directory path: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Invalid directory path: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 3-------------------//
	//==========================================================//
	std::string templatePath = "templates/dir_listing_page.html";
	std::string templatePathModified = "templates/dir_listing_page_temp___.html";
	path = "tests/testContent";
	requPath = "/testContent";
	std::rename(templatePath.c_str(), templatePathModified.c_str());
	resp.body.clear();
	//----------------------------------------------------------//
	//======================Running Test 3======================//
	//----------------------------------------------------------//
	_createIndexPage(path, requPath, resp);
	if (resp.httpStatus != INTERNAL_SERVER_ERROR || resp.body != "") {
		std::cout << "Template missing: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Template missing: \033[1;32mOK\033[0m" << std::endl;
	}
	std::rename(templatePathModified.c_str(), templatePath.c_str());
}

void	MethodExecutor::testGenerateErrorBody(){
	Response resp;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	resp.httpStatus = INTERNAL_SERVER_ERROR;
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	_generateErrorBody(resp);
	if (resp.httpStatus != INTERNAL_SERVER_ERROR || resp.body != "dummy") {
		std::cout << "Costum error page: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Costum error page: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	resp.httpStatus = NOT_FOUND;
	resp.body.clear();
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	_generateErrorBody(resp);
	if (resp.httpStatus != NOT_FOUND || resp.body.find("404") != 153) {
		std::cout << "Invalid error page: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Invalid error page: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 3-------------------//
	//==========================================================//
	resp.httpStatus = BAD_REQUEST;
	resp.body.clear();
	std::ifstream	is("tests/testContent/400errorPage.html");
	std::ostringstream ss;
	ss << is.rdbuf();
	//----------------------------------------------------------//
	//======================Running Test 3======================//
	//----------------------------------------------------------//
	_generateErrorBody(resp);
	if (resp.httpStatus != BAD_REQUEST || resp.body != ss.str()) {
		std::cout << "Default generation: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Default generation: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 4-------------------//
	//==========================================================//
	std::string templatePath = "templates/error_page.html";
	std::string templatePathModified = "templates/error_page_temp___.html";
	std::rename(templatePath.c_str(), templatePathModified.c_str());
	resp.body.clear();
	resp.httpStatus = REQUEST_TIMEOUT;
	//----------------------------------------------------------//
	//======================Running Test 4======================//
	//----------------------------------------------------------//
	_generateErrorBody(resp);
	if (resp.httpStatus != REQUEST_TIMEOUT || resp.body != "") {
		std::cout << "Template missing: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Template missing: \033[1;32mOK\033[0m" << std::endl;
	}
	std::rename(templatePathModified.c_str(), templatePath.c_str());
}

void	MethodExecutor::testGenerateCommonHeaderField(){
	Response	resp;

	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	resp.body = "HALLO";
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	_generateCommonHeaderFields(resp);
	if (resp.headerFields["server"] != "example-name" || \
		resp.headerFields["accept-ranges"] != "0" || \
		resp.headerFields["content-length"] != "5") {
		std::cout << "Normal response: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Normal response: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	resp.body = "";
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	_generateCommonHeaderFields(resp);
	if (resp.headerFields["server"] != "example-name" || \
		resp.headerFields["accept-ranges"] != "0" || \
		resp.headerFields["content-length"] != "0") {
		std::cout << "Empty body: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Empty body: \033[1;32mOK\033[0m" << std::endl;
	}
}

void	MethodExecutor::testGenerateSpecialErrorFields(){
	Response	resp;
	Request		req;
	location	route;

	route.httpMethods = GET | POST | DELETE;
	req.UsedRoute = route;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	resp.httpStatus = METHOD_NOT_ALLOWED;
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	_generateSpecialErrorFields(req, resp);
	if (resp.headerFields["allow"] != "GET POST DELETE") {
		std::cout << "Method not found case: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Method not found case: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	resp.httpStatus = OK_HTTP;
	resp.headerFields.clear();
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	_generateSpecialErrorFields(req, resp);
	if (resp.headerFields.find("allow") != resp.headerFields.end()) {
		std::cout << "Method found case: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Method found case: \033[1;32mOK\033[0m" << std::endl;
	}
}

void MethodExecutor::testWrapperRequest(void){
	Request			requ;
	Response		resp;
	std::string		suffix;
	std::ifstream	is;
	std::ofstream	os;

	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	requ.UsedRoute.redirect = "hallohallo";
	requ.UsedRoute.locationName = "dummy";
	requ.RequestIntegrity = MOVED_PERMANENTLY;
	requ.ReqType = GET;
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	wrapperRequest(requ, resp);
	if (resp.httpStatus != MOVED_PERMANENTLY || \
		resp.responseBuffer.find("\nlocation: hallohallo\r\n") == std::string::npos) {
		std::cout << "Redirection: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Redirection: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	requ.RequestIntegrity = BAD_REQUEST;
	requ.RoutedPath = "tests/testDir/dummy.txt";
	requ.ReqType = GET;
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	wrapperRequest(requ, resp);
	if (resp.httpStatus != BAD_REQUEST || \
		resp.responseBuffer.find("HTTP/1.1 400 Bad Request") != 0 || \
		resp.responseBuffer.find("Server cannot process the request") == std::string::npos) {
		std::cout << "Error page returned: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Error page returned: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 3-------------------//
	//==========================================================//
	requ.RequestIntegrity = OK_HTTP;
	requ.RoutedPath = "tests/testContent/dummy.txt";
	requ.RequestedPath = "/dummy.txt";
	requ.ReqType = HEAD;
	suffix = "\r\n\r\n";
	//----------------------------------------------------------//
	//======================Running Test 3======================//
	//----------------------------------------------------------//
	wrapperRequest(requ, resp);
	if (resp.httpStatus != OK_HTTP || \
		resp.responseBuffer.find("HTTP/1.1 200 OK") != 0 || \
		!std::equal(suffix.rbegin(), suffix.rend(), resp.responseBuffer.rbegin())) {
		std::cout << "HEAD request: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "HEAD request: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 4-------------------//
	//==========================================================//
	requ.RequestIntegrity = OK_HTTP;
	requ.RoutedPath = "tests/testContent/dummy.txt";
	requ.RequestedPath = "/dummy.txt";
	requ.ReqType = GET;
	suffix = "hallihallo";
	//----------------------------------------------------------//
	//======================Running Test 4======================//
	//----------------------------------------------------------//
	wrapperRequest(requ, resp);
	if (resp.httpStatus != OK_HTTP || \
		resp.responseBuffer.find("HTTP/1.1 200 OK") != 0 || \
		!std::equal(suffix.rbegin(), suffix.rend(), resp.responseBuffer.rbegin())) {
		std::cout << "GET request: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "GET request: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 5-------------------//
	//==========================================================//
	requ.RequestIntegrity = OK_HTTP;
	requ.RoutedPath = "tests/testContent/upload2.txt";
	requ.RequestedPath = "/upload2.txt";
	requ.Body = "anything";
	requ.ReqType = POST;
	suffix = "\r\n\r\n";
	//----------------------------------------------------------//
	//======================Running Test 5======================//
	//----------------------------------------------------------//
	wrapperRequest(requ, resp);
	is.open("tests/testContent/upload2.txt");
	std::stringstream buffer;
	if (is)
		buffer << is.rdbuf();
	if (resp.httpStatus != CREATED || \
		resp.responseBuffer.find("HTTP/1.1 201 Created") != 0 || \
		buffer.str() != "anything") {
		std::cout << "POST request: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "POST request: \033[1;32mOK\033[0m" << std::endl;
	}
	is.close();
	std::remove("tests/testContent/upload2.txt");
	//==========================================================//
	//-------------------Preparing for Test 6-------------------//
	//==========================================================//
	requ.RequestIntegrity = OK_HTTP;
	os.open("tests/testContent/deletionFile.txt");
	os << "Dummy stuff";
	os.close();
	requ.RoutedPath = "tests/testContent/deletionFile.txt";
	requ.ReqType = DELETE;
	suffix = "\r\n\r\n";
	//----------------------------------------------------------//
	//======================Running Test 6======================//
	//----------------------------------------------------------//
	wrapperRequest(requ, resp);
	is.open("tests/testContent/deletionFile.txt");
	if (resp.httpStatus != NO_CONTENT || \
		resp.responseBuffer.find("HTTP/1.1 204 No Content") != 0 || \
		is) {
		std::cout << "DELETE request: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "DELETE request: \033[1;32mOK\033[0m" << std::endl;
	}
	is.close();
	std::remove("tests/testContent/deletionFile.txt");
}

void MethodExecutor::testExecuteGet(void){
	Request		requ;
	Response	resp;
	std::ifstream is;
	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	requ.RoutedPath = "tests/testContent/dummy.txt";
	resp.httpStatus = OK_HTTP;
	resp.body = "";
	resp.headerFields["last-modified"] = "";
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	_executeGet(requ, resp);
	if (resp.httpStatus != OK_HTTP || resp.body != "hallihallo" || \
		resp.headerFields["last-modified"].length() == 0) {
		std::cout << "Simple download: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Simple download: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	requ.RoutedPath = "tests/testContent/noAccessFile.txt";
	requ.RequestedPath = "noAccessFile.txt";
	resp.httpStatus = OK_HTTP;
	resp.body = "";
	resp.headerFields["last-modified"] = "";
	mode_t noAccessMode = 0000;
	mode_t original;
	struct stat st;
	if (stat(requ.RoutedPath.c_str(), &st) == 0) {
		original = st.st_mode;
	}
	if (chmod(requ.RoutedPath.c_str(), noAccessMode) != 0) {
        std::cerr << "TEST ERROR: Failed to change permissions on " << requ.RoutedPath << std::endl;
	} else {
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	_executeGet(requ, resp);
	if (resp.httpStatus != UNAUTHORIZED || !resp.headerFields["last-modified"].empty()) {
		std::cout << "No permission: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "No permission: \033[1;32mOK\033[0m" << std::endl;
	}
	if (chmod(requ.RoutedPath.c_str(), original) != 0) {
        std::cerr << "TEST ERROR: Failed to change permissions on " << requ.RoutedPath << std::endl;
	}
	}
	//==========================================================//
	//-------------------Preparing for Test 3-------------------//
	//==========================================================//
	requ.RoutedPath = "tests/testContent/notExisting.txt";
	requ.RequestedPath = "notExisting.txt";
	resp.httpStatus = OK_HTTP;
	resp.body = "";
	resp.headerFields["last-modified"] = "";
	//----------------------------------------------------------//
	//======================Running Test 3======================//
	//----------------------------------------------------------//
	_executeGet(requ, resp);
	if (resp.httpStatus != NOT_FOUND || !resp.headerFields["last-modified"].empty()) {
		std::cout << "Wrong file name: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Wrong file name: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 4-------------------//
	//==========================================================//
	requ.RoutedPath = "tests/testContent";
	requ.RequestedPath = "/";
	resp.httpStatus = OK_HTTP;
	resp.body = "";
	resp.headerFields["last-modified"] = "";
	requ.UsedRoute.dirListing = false;
	//----------------------------------------------------------//
	//======================Running Test 4======================//
	//----------------------------------------------------------//
	_executeGet(requ, resp);
	if (resp.httpStatus != OK_HTTP || resp.body.find("Default Index") == std::string::npos) {
		std::cout << "Directory listing off: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Directory listing off: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 5-------------------//
	//==========================================================//
	std::string templatePath = "templates/dir_listing_page.html";
	std::string templatePathModified = "templates/dir_listing_page_temp___.html";
	requ.UsedRoute.dirListing = true;
	std::rename(templatePath.c_str(), templatePathModified.c_str());
	requ.RoutedPath = "tests/testContent";
	requ.RequestedPath = "/";
	resp.httpStatus = OK_HTTP;
	resp.body = "";
	resp.headerFields["last-modified"] = "";
	//----------------------------------------------------------//
	//======================Running Test 5======================//
	//----------------------------------------------------------//
	_executeGet(requ, resp);
	if (resp.httpStatus != INTERNAL_SERVER_ERROR || !resp.headerFields["last-modified"].empty() || \
		!resp.body.empty()) {
		std::cout << "Directory listing ON fails: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Directory listing ON fails: \033[1;32mOK\033[0m" << std::endl;
	}
	std::rename(templatePathModified.c_str(), templatePath.c_str());
	//==========================================================//
	//-------------------Preparing for Test 6-------------------//
	//==========================================================//
	requ.UsedRoute.dirListing = true;
	requ.RoutedPath = "tests/testContent/indexPageTestsDir";
	requ.RequestedPath = "/indexPageTestsDir";
	resp.httpStatus = OK_HTTP;
	resp.body = "";
	resp.headerFields["last-modified"] = "";
	//----------------------------------------------------------//
	//======================Running Test 6======================//
	//----------------------------------------------------------//
	_executeGet(requ, resp);
	if (resp.httpStatus != OK_HTTP || !resp.headerFields["last-modified"].empty() || \
		resp.body.find("veryUniqueFileName") != 783) {
		std::cout << "Directory listing ON success: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Directory listing ON success: \033[1;32mOK\033[0m" << std::endl;
	}
}

void MethodExecutor::testExecutePost(void){
	Request		requ;
	Response	resp;
	std::ofstream os;
	std::ifstream is;

	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	requ.RoutedPath = "tests/testContent/upload1.txt";
	resp.httpStatus = OK_HTTP;
	resp.headerFields["location"] = "";
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	_executePost(requ, resp);
	if (resp.httpStatus != FORBIDDEN || !resp.headerFields["location"].empty()) {
		std::cout << "Upload fails because file exists: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Upload fails because file exists: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	requ.RoutedPath = "tests/testContent/upload2.txt";
	resp.httpStatus = OK_HTTP;
	requ.Body = "anything";
	requ.RequestedPath = "/upload2.txt";
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	_executePost(requ, resp);
	is.open("tests/testContent/upload2.txt");
	std::stringstream buffer;
	if (is)
		buffer << is.rdbuf();
	if (resp.httpStatus != CREATED || buffer.str() != "anything" || \
		resp.headerFields["location"] != "/upload2.txt") {
		std::cout << "Upload simple body: \033[1;31mFAILED\033[0m" << std::endl;
		std::remove(requ.RoutedPath.c_str());
	} else if (this->silent == false) {
		std::cout << "Upload simple body: \033[1;32mOK\033[0m" << std::endl;
	}
	is.close();
	std::remove("tests/testContent/upload2.txt");
	//==========================================================//
	//-------------------Preparing for Test 3-------------------//
	//==========================================================//
	requ.RoutedPath = "tests/testContent/nonsensePath/upload1.txt";
	resp.httpStatus = OK_HTTP;
	resp.headerFields["location"] = "";
	//----------------------------------------------------------//
	//======================Running Test 3======================//
	//----------------------------------------------------------//
	_executePost(requ, resp);
	is.open("tests/testContent/nonsensePath/upload1.txt");
	if (resp.httpStatus != INTERNAL_SERVER_ERROR || !resp.headerFields["location"].empty() || is) {
		std::cout << "Invalid upload path: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Invalid upload path: \033[1;32mOK\033[0m" << std::endl;
	}
}

void MethodExecutor::testExecuteDelete(void){
	Request		requ;
	Response	resp;
	std::ofstream os;
	std::ifstream is;

	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	requ.RoutedPath = "tests/invalidDir/deletionFile.txt";
	resp.httpStatus = OK_HTTP;
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	_executeDelete(requ, resp);
	if (resp.httpStatus != NOT_FOUND) {
		std::cout << "Invalid directory for deletion: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Invalid directory for deletion: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	os.open("tests/testContent/deletionFile.txt");
	os << "Dummy stuff";
	os.close();
	requ.RoutedPath = "tests/testContent/deletionFile.txt";
	resp.httpStatus = OK_HTTP;
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	_executeDelete(requ, resp);
	is.open(requ.RoutedPath.c_str());
	if (resp.httpStatus != NO_CONTENT || is) {
		std::cout << "Deletion in subfolder: \033[1;31mFAILED\033[0m" << std::endl;
		is.close();
	} else if (this->silent == false) {
		std::cout << "Deletion in subfolder: \033[1;32mOK\033[0m" << std::endl;
	}
	// just for clean up
	std::remove(requ.RoutedPath.c_str());
	//==========================================================//
	//-------------------Preparing for Test 3-------------------//
	//==========================================================//
	requ.RoutedPath = "tests/testContent/wrongFileName.txt";
	resp.httpStatus = OK_HTTP;
	//----------------------------------------------------------//
	//======================Running Test 3======================//
	//----------------------------------------------------------//
	_executeDelete(requ, resp);
	is.open(requ.RoutedPath.c_str());
	if (resp.httpStatus != NOT_FOUND) {
		std::cout << "Nonexisting file: \033[1;31mFAILED\033[0m" << std::endl;
		is.close();
	} else if (this->silent == false) {
		std::cout << "Nonexisting file: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 4-------------------//
	//==========================================================//
	os.open("tempFileForDeletion.txt");
	os << "Dummy stuff";
	os.close();
	requ.RoutedPath = "tempFileForDeletion.txt";
	resp.httpStatus = OK_HTTP;
	//----------------------------------------------------------//
	//======================Running Test 4======================//
	//----------------------------------------------------------//
	_executeDelete(requ, resp);
	is.open(requ.RoutedPath.c_str());
	if (resp.httpStatus != NO_CONTENT && resp.httpStatus != UNAUTHORIZED) {
		std::cout << "Deletion in working dir: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Deletion in working dir: \033[1;32mOK\033[0m" << std::endl;
	}
	// just for clean up
	is.close();
	std::remove(requ.RoutedPath.c_str());
	//==========================================================//
	//-------------------Preparing for Test 5-------------------//
	//==========================================================//
	requ.RoutedPath = "tests/testContent/noAccess/noDeletionPossible.txt";
	std::string directory = "tests/testContent/noAccess";
	resp.httpStatus = OK_HTTP;
	mode_t noAccessMode = 0000;
	mode_t original;
	struct stat st;
	if (stat(directory.c_str(), &st) == 0) {
		original = st.st_mode;
	}
	if (chmod(directory.c_str(), noAccessMode) != 0) {
        std::cerr << "TEST ERROR: Failed to change permissions on " << directory;
	} else {
	//----------------------------------------------------------//
	//======================Running Test 5======================//
	//----------------------------------------------------------//
	_executeDelete(requ, resp);
	if (resp.httpStatus != UNAUTHORIZED) {
		std::cout << "Wrong access rights: \033[1;31mFAILED\033[0m" << std::endl;
		is.close();
	} else if (this->silent == false) {
		std::cout << "Wrong access rights: \033[1;32mOK\033[0m" << std::endl;
	}
	if (chmod(directory.c_str(), original) != 0) {
        std::cerr << "TEST ERROR: Failed to change permissions on " << directory;
	}
	}
}

void	MethodExecutor::testWriteStatusLine(){
	Response	resp;

	//==========================================================//
	//-------------------Preparing for Test 1-------------------//
	//==========================================================//
	resp.httpStatus = OK_HTTP;
	resp.responseBuffer.clear();
	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	_writeStatusLine(resp);
	if (resp.responseBuffer != "HTTP/1.1 200 OK\n") {
		std::cout << "200 Status: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "200 Status: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	resp.httpStatus = NOT_FOUND;
	resp.responseBuffer.clear();
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	_writeStatusLine(resp);
	if (resp.responseBuffer != "HTTP/1.1 404 Not Found\n") {
		std::cout << "404 Status: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "404 Status: \033[1;32mOK\033[0m" << std::endl;
	}
}

void	MethodExecutor::testWriteHeaderFields(){
	Response	resp;

	//----------------------------------------------------------//
	//======================Running Test 1======================//
	//----------------------------------------------------------//
	_writeHeaderFields(resp);
	if (resp.responseBuffer != "\r\n") {
		std::cout << "No header fields: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "No header fields: \033[1;32mOK\033[0m" << std::endl;
	}
	//==========================================================//
	//-------------------Preparing for Test 2-------------------//
	//==========================================================//
	resp.responseBuffer.clear();
	resp.headerFields["testField"] = "testValue";
	resp.headerFields["testField2"] = "testValue2";
	resp.headerFields["testField3"] = "testValue3";
	resp.headerFields["testField4"] = "testValue4";
	std::string expected = "testField: testValue\r\ntestField2: testValue2\r\ntestField3: testValue3\r\ntestField4: testValue4\r\n\r\n";
	//----------------------------------------------------------//
	//======================Running Test 2======================//
	//----------------------------------------------------------//
	_writeHeaderFields(resp);
	if (resp.responseBuffer != expected) {
		std::cout << "Normal header fields: \033[1;31mFAILED\033[0m" << std::endl;
	} else if (this->silent == false) {
		std::cout << "Normal header fields: \033[1;32mOK\033[0m" << std::endl;
	}
}