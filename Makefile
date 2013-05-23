CXX=g++
CXXFLAGS="-std=c++11"
FILES="src/disassembler.cc"

all:
	${CXX} ${CXXFLAGS} ${FILES} -o Chip8-Disassembler

clean:
	rm Chip8-Disassembler
