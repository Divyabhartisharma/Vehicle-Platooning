C++ implementation of Server & Client connection with multiclient support using pthreads. 
Works only on Windows OS
For comiling, you might need to use the -lws2_32 linker like:  
g++ -o server.exe server.cpp -lws2_32
g++ -o client.exe client.cpp -lws2_32