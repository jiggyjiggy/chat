client :
	clang++ chat_client/*.cpp -o chat_client/chat_client.out -lssl -lcrypto

run-client : client
	cd chat_client && ./chat_client.out


server :
	clang++ -I chat_server chat_server/*.cpp chat_server/command/*.cpp -o chat_server/chat_server.out -lssl -lcrypto

run-server: server
	cd chat_server && ./chat_server.out


clean:
	rm chat_client/*.out chat_server/*.out

aaa:
	chmod +x ./test/many_client.sh
	./test/many_client.sh
