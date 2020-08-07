cd sane_engine && g++ -O2 -Wall -Wextra ../thirdparty/glad/src/glad.c src/main.cpp src/**/*.cpp -I src/ -I ../thirdparty/glad/include -I ../thirdparty -lglfw -ldl
