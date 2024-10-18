SRC=src/app.cpp src/server.cpp src/http.cpp
OUT=$(SRC:src/%.cpp=bin/%.o)
CFLAGS=$$(wx-config --cflags) -I include/
LIBS=$$(wx-config --libs)
EXE=bin/main
.PHONY: build
build: $(EXE)
$(EXE): $(OUT)
	g++ -o $@ $^ $(LIBS)
bin/%.o: src/%.cpp
	g++ -c -o $@ $^ $(CFLAGS)
.PHONY: run
run: $(EXE)
	cd bin;../$<
