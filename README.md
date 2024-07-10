# How to use the webserver
This page gives an overview on how to work with the current webserv project.
## Running the server
To successfully setup the server one needs a cofiguration file. It can be given as an argument while running the server like
```
./webserv  tutorialConfig.conf
```
or runs currently by default with the configuration `parsers/sampleConfig.conf`. This tutorial is based on the configuration `tutorialConfig.conf`. I used the brave browser to execute the next steps.
## GET method
To serve static websites, the GET method is used. I use the brave brave to connect with the server. For example typing the URL
```
http://localhost:8080/example.html
```
in the browser should display the website which is stored in `content/example.html`.
