#!/bin/bash

# echo -e "POST / HTTP/1.1\r\nHost: 127.1.1.1:8001\r\nUser-Agent: Go-http-client/1.1\r\nTransfer-Encoding: chunked\r\nContent-Type: test/file\r\nAccept-Encoding: gzip\r\n\r\n"


# # Envoyer les données en chunks
# while read -r chunk; do
#     chunk_length=$(printf "%x\r\n" $(echo -n "$chunk" | wc -c))
#     echo -e "$chunk_length$chunk\r\n" | nc $SERVER $PORT
# done < <(fold -w $CHUNK_SIZE $DATA_FILE)

# # Chunk de fin (taille 0)
# echo -e "0\r\n\r\n" | nc $SERVER $PORT


(echo -ne "POST / HTTP/1.1\r\nHost: 127.1.1.1:8001\r\nUser-Agent: netcat/1.1\r\nTransfer-Encoding: chunked\r\nContent-Type: application/octet-stream\r\n\r\n"; \
for i in {1..10}; do echo -ne "2000\r\n$(head -c 8192 < /dev/zero)\r\n"; done; \
echo -ne "0\r\n\r\n") | nc 127.1.1.1 8001
