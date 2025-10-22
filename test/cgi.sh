{
  # Headers
  printf "POST /test.php HTTP/1.1\r\n"
  printf "Host: localhost:2004\r\n"
  printf "User-Agent: chunk-tester\r\n"
  printf "Transfer-Encoding: chunked\r\n"
  printf "Content-Type: text/plain\r\n"
  printf "\r\n"

  # Send chunks with delays
  printf "7\r\nMozilla\r\n"
  sleep 2
  printf "9\r\nDeveloper\r\n"
  sleep 2
  printf "7\r\nNetwork\r\n"
  sleep 2
  # End of chunks
  printf "0\r\n\r\n"
} | nc localhost 2004
