#Install packages with conan

#Create build folder
mkdir build
cd build

#Install dependencies
conan install . --build=missing

#Generate build files
cmake ..

#Build the project
cmake --build .
