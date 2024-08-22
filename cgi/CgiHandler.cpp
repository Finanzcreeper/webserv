#include "../CommonIncludes.hpp"
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <cstdio>
#include <sys/wait.h>
#include <fstream>

//extern char **environ;

char	**prepareEnvVariables(Request& requ){
	std::map<std::string,std::string> env;
	if (requ.HeaderFields.find("content-length") != requ.HeaderFields.end()){
		env["content-length"] = requ.HeaderFields["content-length"];
	} else {
		env["content-length"] = "0";
	}
	if (requ.HeaderFields.find("content-type") != requ.HeaderFields.end()){
		env["content-type"] = requ.HeaderFields["content-type"];
	}
	env["URI-path"] = requ.RequestedPath;
	env["routed-path"] = requ.RoutedPath;
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

void	cleanupCGI(char **env){
	size_t len = 0;
	while (env[len]){
		free(env[len++]);
	}
	delete[] env;
}

void	executeCGI(std::string programPath, Request& requ, Response& resp) {
	char **env_c = prepareEnvVariables(requ);
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

	if (write(fd_in, requ.Body.c_str(), requ.Body.length()) == -1) {
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
		char *argv[] = { (char *)"python3", (char *)"tests/testContent/cgi/test.py", NULL };
		execve(programPath.c_str(), argv, env_c);
		cleanupCGI(env_c);
		exit(EXIT_FAILURE);
	} else {
		int	status;
		waitpid(pid, &status, 0);
		std::cout << (WIFEXITED(status) && WEXITSTATUS(status)) << std::endl;
		if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS) {
			lseek(fd_out, 0, SEEK_SET);
			char buffer[4096];
            size_t bytesRead;
            while ((bytesRead = fread(buffer, 1, sizeof(buffer), tmpf_out)) > 0) {
                resp.body.append(buffer, bytesRead);
            }
		} else {
			resp.httpStatus = INTERNAL_SERVER_ERROR;
		}
	}
	dup2(stdIn, STDIN_FILENO);
	dup2(stdOut, STDOUT_FILENO);
	close(fd_in);
	close(fd_out);
	fclose(tmpf_in);
	fclose(tmpf_out);
	cleanupCGI(env_c);
}