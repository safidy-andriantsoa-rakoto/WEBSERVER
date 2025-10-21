{
  # Headers
  printf "POST /uppercase.php HTTP/1.1\r\n"
  printf "Host: localhost:2004\r\n"
  printf "User-Agent: chunk-tester\r\n"
  printf "Transfer-Encoding: chunked\r\n"
  printf "Content-Type: text/plain\r\n"
  printf "\r\n"

  # Send chunks with delays
  printf "7\r\nMozilla\r\n"
  printf "9\r\nDeveloper\r\n"
  printf "7\r\nNetwork\r\n"

  # End of chunks
  printf "0\r\n\r\n"
} | nc localhost 2004
