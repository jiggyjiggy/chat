client : 
	clang++ chat_client/*.cpp -o chat_client/a.out

run-client : client
	./chat_client/a.out


server : 
	clang++ -I chat_server chat_server/*.cpp chat_server/command/*.cpp -o chat_server/a.out

run-server: server
	./chat_server/a.out


clean:
	rm chat_client/a.out chat_server/a.out