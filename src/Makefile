# FILES
FILES = glad.c legal_moves.c search2.0.c evaluate2.0.c frontend.c log2.c ps.c ./graphics_dependencies/*.o
FILES_OUTPUT = glad.c search2.0.c legal_moves.c search_output.c evaluate2.0.c frontend_output.c log2.c ps.c

# OS-SPECIFIC SETTINGS
ifeq ($(OS),Windows_NT)
  # Link libgcc and libstdc++ statically on Windows
  LDFLAGS += -static-libgcc -static-libstdc++
  LIBS +=
  INCLUDE +=
  LINK +=
else
  detected_OS := $(shell uname)
  ifeq ($(detected_OS),Linux)
    # LINUX-DEPENDENT SETTINGS
    LIBS += -L usr/lib/x86_64-linux-gnu/
    INCLUDE += -I ../include
    LINK += -l:libglfw.so.3.3 -lGL -lX11 -l:libXrandr.so.2 -l:libXi.so.6 -ldl -lm -lcglm -lpthread
  else
    # MACOS-DEPENDENT SETTINGS
    LIBS += -L /System/Volumes/Data/opt/homebrew/Cellar/cglm/0.8.9/lib -L /System/Volumes/Data/opt/homebrew/Cellar/glfw/3.3.8/lib -L /System/Volumes/Data/opt/homebrew/Cellar/libx11/1.8.4/lib -L /System/Volumes/Data/opt/homebrew/Cellar/libxrandr/1.5.3/lib -L /System/Volumes/Data/opt/homebrew/Cellar/libXi/1.8/lib -L /System/Volumes/Data/opt/homebrew/Cellar/glfw/3.3.8/lib
    INCLUDE += -I ../include -I /System/Volumes/Data/opt/homebrew/Cellar/cglm/0.8.9/include -I/System/Volumes/Data/opt/homebrew/Cellar/glfw/3.3.8/include -I /System/Volumes/Data/opt/homebrew/Cellar/cglm/0.8.9/include
    LINK += /usr/local/lib/libglfw.dylib -framework OpenGL /System/Volumes/Data/opt/homebrew/Cellar/libx11/1.8.4/lib/libX11.dylib /System/Volumes/Data/opt/homebrew/Cellar/libxrandr/1.5.3/lib/libXrandr.dylib /System/Volumes/Data/opt/homebrew/Cellar/libxi/1.8/lib/libXi.dylib -ldl -lm /System/Volumes/Data/opt/homebrew/Cellar/cglm/0.8.9/lib/libcglm.dylib -pthread
  endif
endif


.PHONY: default
default:
	gcc -Wall -g $(INCLUDE) $(LIBS) -o 3DChess $(FILES) $(LINK)
	gcc -Wall -O3 $(INCLUDE) $(LIBS) -o 3DChessOp $(FILES) $(LINK)
	rm -rf *.dSYM

.PHONY: output
output:
	gcc -Wall -g $(INCLUDE) $(LIBS) -o outreg $(FILES_OUTPUT) $(LINK)
	gcc -Wall -O3 $(INCLUDE) $(LIBS) -o outop $(FILES_OUTPUT) $(LINK)
	rm -rf *.dSYM

.PHONY: compile
compile: object
	gcc -Wall -g $(INCLUDE) $(LIBS) -o outreg *.o $(LINK)
	rm -rf *.dSYM
	rm -rf *.o

.PHONY: object
object:
	gcc -Wall -g $(INCLUDE) $(LIBS) -c $(FILES_OUTPUT) $(LINK)
	rm -rf *.dSYM

.PHONY: line_changer
line_changer:
	gcc -O3 -g -o line_changer ./model/line_changer.c

.PHONY: man_eval
man_eval:
	gcc -O3 -g $(INCLUDE) -o man_eval ./model/man_eval.c

.PHONY: clean
clean:
	rm -f 3DChess
	rm -f 3DChessOp
	rm -f outreg
	rm -f outop
	rm -f line_changer
	rm -f man_eval
	rm -rf *.dSYM
	rm -rf *.o
	rm -rf *.i
	rm -rf *.bc
	rm -rf *.s
