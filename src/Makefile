# FILES
files = glad.c legal_moves.c search2.0.c evaluate2.0.c frontend.c log2.c ps.c ./graphics_dependencies/*.o
files_output = glad.c search2.0.c legal_moves.c search_output.c evaluate2.0.c frontend_output.c log2.c ps.c

# MACOS VENTURA DEPENDENCIES
mac_libs = -L /System/Volumes/Data/opt/homebrew/Cellar/cglm/0.8.9/lib -L /System/Volumes/Data/opt/homebrew/Cellar/glfw/3.3.8/lib -L /System/Volumes/Data/opt/homebrew/Cellar/libx11/1.8.4/lib -L /System/Volumes/Data/opt/homebrew/Cellar/libxrandr/1.5.3/lib -L /System/Volumes/Data/opt/homebrew/Cellar/libXi/1.8/lib -L /System/Volumes/Data/opt/homebrew/Cellar/glfw/3.3.8/lib
mac_include = -I ../include -I /System/Volumes/Data/opt/homebrew/Cellar/cglm/0.8.9/include -I/System/Volumes/Data/opt/homebrew/Cellar/glfw/3.3.8/include -I /System/Volumes/Data/opt/homebrew/Cellar/cglm/0.8.9/include
mac_link = /usr/local/lib/libglfw.dylib -framework OpenGL /System/Volumes/Data/opt/homebrew/Cellar/libx11/1.8.4/lib/libX11.dylib /System/Volumes/Data/opt/homebrew/Cellar/libxrandr/1.5.3/lib/libXrandr.dylib /System/Volumes/Data/opt/homebrew/Cellar/libxi/1.8/lib/libXi.dylib -ldl -lm /System/Volumes/Data/opt/homebrew/Cellar/cglm/0.8.9/lib/libcglm.dylib -pthread

# UBUNTU 22.04 DEPENDENCIES
linux_libs = -L usr/lib/x86_64-linux-gnu/
linux_include = -I ../include
linux_link = -l:libglfw.so.3.3 -lGL -lX11 -l:libXrandr.so.2 -l:libXi.so.6 -ldl -lm -lcglm -lpthread

.PHONY: linux
linux:
	gcc -Wall -g $(linux_include) $(linux_libs) -o 3DChessOp $(files) $(linux_link)
	gcc -Wall -O3 $(linux_include) $(linux_libs) -o 3DChessOp $(files) $(linux_link)

.PHONY: output
output:
	gcc -Wall -g $(linux_include) $(linux_libs) -o outreg $(files_output) $(linux_link)
	gcc -Wall -O3 $(linux_include) $(linux_libs) -o outop $(files_output) $(linux_link)

.PHONY: object_output_mac
object_output_mac:
	gcc -Wall -g -save-temps $(mac_include) $(mac_libs) -c $(files_output) $(mac_link)
	rm -rf *.dSYM

.PHONY: output_mac
output_mac:
	gcc -Wall -g $(mac_include) $(mac_libs) -o outreg $(files_output) $(mac_link)
	gcc -Wall -O3 $(mac_include) $(mac_libs) -o outop $(files_output) $(mac_link)
	rm -rf *.dSYM

.PHONY: mac
mac:
	gcc -Wall -g $(mac_include) $(mac_libs) -o 3DChess $(files) $(mac_link)
	gcc -Wall -O3 $(mac_include) $(mac_libs) -o 3DChessOp $(files) $(mac_link)
	rm -rf *.dSYM

.PHONY: clean
clean:
	rm -f 3DChess
	rm -f 3DChessOp
	rm -f outreg
	rm -f outop
	rm -rf *.dSYM
	rm -rf *.o
	rm -rf *.i
	rm -rf *.bc
	rm -rf *.s
