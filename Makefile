client : 
	clang++ chat_client/*.cpp -o chat_client/a.out

run-client : client
	./chat_client/a.out


server : 
	clang++ chat_server/*.cpp -o chat_server/a.out

run-server: server
	./chat_server/a.out
	

clean:
	rm -f chat_client/a.out chat_server/a.out