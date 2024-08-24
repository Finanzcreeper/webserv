#include "../CommonIncludes.hpp"
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <sys/wait.h>
#include <fstream>

pid_t g_child_pid = -1;  // Global variable to hold pid for cgi timeout handling
bool g_timeout_occured = false;  // Global variable to hold pid for cgi timeout handling

std::map<std::string,std::string>	prepareEnvVariables(Request& requ, const t_server* settings){
	std::map<std::string,std::string> env;
	std::string programPath;
	std::string	queryString;
	if (requ.ReqType == GET) {
		size_t queryIdx = requ.RoutedPath.find('?');
		if (queryIdx != std::string::npos) {
			programPath = requ.RoutedPath.substr(0, queryIdx);
			queryString = requ.RoutedPath.substr(queryIdx + 1, requ.RoutedPath.length() - (queryIdx + 1));
		}
	} else if (requ.ReqType == POST){
		programPath = requ.RoutedPath;
		queryString = requ.Body;
	}
	if (requ.HeaderFields.find("content-length") != requ.HeaderFields.end()){
		env["CONTENT_LENGTH"] = requ.HeaderFields["content-length"];
	}
	if (requ.HeaderFields.find("content-type") != requ.HeaderFields.end()){
		env["CONTENT_TYPE"] = requ.HeaderFields["content-type"];
	}
	if (requ.HeaderFields.find("authorization") != requ.HeaderFields.end()){
		env["REMOTE_USER"] = requ.HeaderFields["authorization"];
		env["REMOTE_IDENT"] = requ.HeaderFields["authorization"];
		env["AUTH_TYPE"] = requ.HeaderFields["authorization"];
	}
	if (requ.ReqType == GET) {
		env["REQUEST_METHOD"] = "GET";
	} else {
		env["REQUEST_METHOD"] = "POST";
	}
	env["REMOTE_ADDR"] = settings->host;
	env["QUERY_STRING"] = queryString;
	env["SCRIPT_NAME"] = programPath;
	env["SERVER_NAME"] = settings->serverName;
	env["SERVER_PORT"] = settings->port;
	env["SERVER_PROTOCOL"] = HTTP_PROTOCOL;
	env["GATEWAY_INTERFACE"] = "CGI/1.1";
	env["SERVER_SOFTWARE"] = "webserv";
	return env;
}

void	cleanupCGI(char **env){
	size_t len = 0;
	while (env[len]){
		free(env[len++]);
	}
	delete[] env;
}

char **convertStringMapToArray(std::map<std::string,std::string> env){
	size_t len = env.size();
	std::string variable;
	std::map<std::string,std::string>::iterator iter = env.begin();
	char **array;
	try {
		int i = 0;
		array = new char*[len + 1];
		while (iter != env.end()){
			variable = iter->first + "=" + iter->second;
			array[i++] = strdup(variable.c_str());
			iter++;
		}
		array[i] = NULL;
	} catch (std::bad_alloc &e){
		array = NULL;
	}
	return array;
}

void handle_alarm(int sig) {
    if (g_child_pid != -1) {
        // Send SIGKILL to the child process to terminate it
        kill(g_child_pid, SIGKILL);
		g_timeout_occured = true;
    }
	sig++; //just to avoid compiler warning
}

void	executeCGI(Request& requ, Response& resp, const t_server* settings) {
	std::map<std::string,std::string> env;
	env = prepareEnvVariables(requ, settings);
	char **env_c = convertStringMapToArray(env);
	if (env_c == NULL){
		resp.httpStatus = INTERNAL_SERVER_ERROR;
		return ;
	}

	pid_t	pid;
	int		stdIn = dup(STDIN_FILENO);
	int		stdOut = dup(STDOUT_FILENO);

	FILE* tmpf_in = tmpfile();
	FILE* tmpf_out = tmpfile();
	int		fd_in = fileno(tmpf_in);
	int		fd_out = fileno(tmpf_out);

	if (write(fd_in, env["QUERY_STRING"].c_str(), env["QUERY_STRING"].length()) == -1) {
		resp.httpStatus = INTERNAL_SERVER_ERROR;
		return;
	}
	lseek(fd_in, 0, SEEK_SET);
	pid = fork();
	if (pid == -1){
		resp.httpStatus = INTERNAL_SERVER_ERROR;
		return ;
	} else if (pid == 0) {
		dup2(fd_in, STDIN_FILENO);
		dup2(fd_out, STDOUT_FILENO);
		close(fd_in);
		close(fd_out);
		char *argv[] = { (char *)"/usr/bin/python3", (char *)env["SCRIPT_NAME"].c_str(), NULL};
		execve("/usr/bin/python3", argv, env_c);
		cleanupCGI(env_c);
		exit(EXIT_FAILURE);
	} else {
		g_child_pid = pid;
		signal(SIGALRM, handle_alarm);
        alarm(CGI_TIMEOUT);

		int	status;
		waitpid(pid, &status, 0);
		alarm(0);
		if (g_timeout_occured == true){
			resp.httpStatus = GATEWAY_TIMEOUT;
		} else if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
			lseek(fd_out, 0, SEEK_SET);
			char buffer[4096];
            size_t bytesRead;
            while ((bytesRead = fread(buffer, 1, sizeof(buffer), tmpf_out)) > 0) {
                resp.responseBuffer.append(buffer, bytesRead);
            }
		} else {
			resp.httpStatus = INTERNAL_SERVER_ERROR;
		}
	}
	g_child_pid = -1;
	g_timeout_occured = false;
	dup2(stdIn, STDIN_FILENO);
	dup2(stdOut, STDOUT_FILENO);
	close(fd_in);
	close(fd_out);
	fclose(tmpf_in);
	fclose(tmpf_out);
	cleanupCGI(env_c);
}