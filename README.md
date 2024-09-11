# How to use the webserver
This page gives an overview on how to work with the current webserv project.
## Running the server
To successfully setup the server one needs a cofiguration file. It can be given as an argument while running the server like
```
./webserv  tutorialConfig.conf
```
or runs currently by default with the configuration `parsers/sampleConfig.conf`. This tutorial is based on the configuration `tutorialConfig.conf`. I used the brave browser to execute the next steps.
## GET method
To serve static websites, the GET method is used. I use the brave browser to connect with the server. For example typing the URL
```
http://localhost:8080/example.html
```
in the browser should display the website which is stored in `content/example.html`, since `/example.html` is mapped to the location `/` which has the path `/content`. Instead of files one can also display directories, if dirlisting is set to ON in the server config. The URL
```
http://localhost:8080/templates
```
will show the files which are contained in the folder `content/templates`. **You can put own html files in the folder `content` and easily reach them with the browser.**

It is possible to request a page and the server redirects you to another page. For example
```
http://localhost:8080/redirect/arte
```
should route the user to the page
http://arte.tv

## POST method
Uploading files to the webserver is implemented via POST requests. To test it in the browser, we need the webpage `content/file_upload_test.html`. So we start by requesting
```
http://localhost:8080/file_upload_test.html
```
This page allows to upload files. Right now any textfile can be uploaded and by configuration of the page `content/file_upload_test.html`, the file then is uploaded to `content/upload` because the url is mapped to the location `upload` which has the path `/content/upload` and you should be able to see it in the file tree. **A file can be uploaded only once, since files on the webserver are not overwritten if they already exist.**

## DELETE method
Deleting files on the webserver can be executed with a DELETE request. The link 
```
http://localhost:8080/file_delete_test.html
```
provides an example. Pressing the button sends a DELETE request with the path `/delete/text.txt`.