cd .\Debug
start ServerConnector.exe localhost:8080
cd ..\Project3\bin\Debug
start Project3.exe localhost:8082 localhost:8080 AutoDemo1
start Project3.exe localhost:8081 localhost:8080 AutoDemo2