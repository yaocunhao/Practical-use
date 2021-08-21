BIN=http_server
SRC=main.cpp
FLAG=-lpthread #-DDEBUG
CURR_PATH=$(shell pwd)

.PHONY:all
all:CGI $(BIN)

CGI:
	cd ./cgi;\
	make;\
	cd ..

$(BIN):$(SRC)
	g++ -o $@ $^  $(FLAG)

.PHONY:clean
clean:
	rm -rf $(BIN)
	cd ./cgi;\
	make clean;\
	cd ..
	rm -rf output

.PHONY:output
output:
	make clean
	make
	mkdir -p     output/wwwroot/cgi
	cp $(BIN)    output -rf
	cp wwwroot/* output/wwwroot -rf
	cp cgi/cgi 	 output/wwwroot/cgi
