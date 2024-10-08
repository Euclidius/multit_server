# Multithread server example
## Build
```sh
mkdir build
cd build
cmake ..
cd ..
cmake --build build
```
## Usage
Create a listener:
```sh
./build/server
```
Client:
```sh
nc <serverip> 12345
```
