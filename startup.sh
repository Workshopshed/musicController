# startup webserver and mosca
node webServer.js &
mosca -v --http-port 1884 --only-http &