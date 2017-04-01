# startup webserver, mosca, and music listener
./webServer.js &
mosca -v --http-port 1884 --only-http &
./musicListener.js &