# FILES
files = glad.c legal_moves.c search2.0.c evaluate2.0.c frontend.c log2.c ps.c ./graphics_dependencies/*.o

# MACOS VENTURA DEPENDENCIES
mac_libs = -L /System/Volumes/Data/opt/homebrew/Cellar/cglm/0.8.9/lib -L /System/Volumes/Data/opt/homebrew/Cellar/glfw/3.3.8/lib -L /System/Volumes/Data/opt/homebrew/Cellar/libx11/1.8.4/lib -L /System/Volumes/Data/opt/homebrew/Cellar/libxrandr/1.5.3/lib -L /System/Volumes/Data/opt/homebrew/Cellar/libXi/1.8/lib -L /System/Volumes/Data/opt/homebrew/Cellar/glfw/3.3.8/lib
mac_include = -I ../include -I /System/Volumes/Data/opt/homebrew/Cellar/cglm/0.8.9/include -I/System/Volumes/Data/opt/homebrew/Cellar/glfw/3.3.8/include -I /System/Volumes/Data/opt/homebrew/Cellar/cglm/0.8.9/include
mac_link = /usr/local/lib/libglfw.dylib -framework OpenGL /System/Volumes/Data/opt/homebrew/Cellar/libx11/1.8.4/lib/libX11.dylib /System/Volumes/Data/opt/homebrew/Cellar/libxrandr/1.5.3/lib/libXrandr.dylib /System/Volumes/Data/opt/homebrew/Cellar/libxi/1.8/lib/libXi.dylib -ldl -lm /System/Volumes/Data/opt/homebrew/Cellar/cglm/0.8.9/lib/libcglm.dylib

# UBUNTU 22.04 DEPENDENCIES
linux_libs = -L usr/lib/x86_64-linux-gnu/
linux_include = -I ../include
linux_link = -l:libglfw.so.3.3 -lGL -lX11 -l:libXrandr.so.2 -l:libXi.so.6 -ldl -lm -lcglm -lpthread


default:
	gcc -Wall -g $(linux_include) $(linux_libs) -o 3DChessOp $(files) $(linux_link)
	gcc -Wall -O3 $(linux_include) $(linux_libs) -o 3DChessOp $(files) $(linux_link)

mac:
	gcc -Wall -g -pthread $(mac_include) $(mac_libs) -o 3DChess glad.c legal_moves.c search2.0.c evaluate2.0.c frontend.c log2.c ps.c graphics_dependencies/*.o $(mac_link)
#	gcc -Wall -O3 -pthread $(mac_include) $(mac_libs) -o 3DChessOp glad.c legal_moves.c search2.0.c evaluate2.0.c frontend.c log2.c ps.c graphics_dependencies/*.o $(mac_link)

tests:
	rm 3DChess*
	gcc -Wall -g -pthread -I ../include -o 3DChess_start legal_moves.c search2.0.c evaluate2.0.c ./board_configs/frontend_start.c log2.c ps.c
	gcc -Wall -g -pthread -I ../include -o 3DChess_check1 legal_moves.c search2.0.c evaluate2.0.c ./board_configs/frontend_check1.c log2.c ps.c
	gcc -Wall -g -pthread -I ../include -o 3DChess_check2 legal_moves.c search2.0.c evaluate2.0.c ./board_configs/frontend_check2.c log2.c ps.c
	gcc -Wall -O3 -pthread -I ../include -o 3DChess_start_op legal_moves.c search2.0.c evaluate2.0.c ./board_configs/frontend_start.c log2.c ps.c
	gcc -Wall -O3 -pthread -I ../include -o 3DChess_check1_op legal_moves.c search2.0.c evaluate2.0.c ./board_configs/frontend_check1.c log2.c ps.c
	gcc -Wall -O3 -pthread -I ../include -o 3DChess_check2_op legal_moves.c search2.0.c evaluate2.0.c ./board_configs/frontend_check2.c log2.c ps.c

all:
	rm 3DChess*
	gcc -Wall -O3 -pthread -I ../include -o 3DChess legal_moves.c search2.0.c evaluate2.0.c frontend.c log2.c ps.c
	gcc -Wall -O3 -pthread -I ../include -o 3DChess legal_moves.c search2.0.c evaluate2.0.c frontend.c log2.c ps.c
	gcc -Wall -g -pthread -I ../include -o 3DChess legal_moves.c search2.0.c evaluate2.0.c frontend.c log2.c ps.c
	gcc -Wall -g -pthread -I ../include -o 3DChess_start legal_moves.c search2.0.c evaluate2.0.c ./board_configs/frontend_start.c log2.c ps.c
	gcc -Wall -g -pthread -I ../include -o 3DChess_check1 legal_moves.c search2.0.c evaluate2.0.c ./board_configs/frontend_check1.c log2.c ps.c
	gcc -Wall -g -pthread -I ../include -o 3DChess_check2 legal_moves.c search2.0.c evaluate2.0.c ./board_configs/frontend_check2.c log2.c ps.c
	gcc -Wall -O3 -pthread -I ../include -o 3DChess_start_op legal_moves.c search2.0.c evaluate2.0.c ./board_configs/frontend_start.c log2.c ps.c
	gcc -Wall -O3 -pthread -I ../include -o 3DChess_check1_op legal_moves.c search2.0.c evaluate2.0.c ./board_configs/frontend_check1.c log2.c ps.c
	gcc -Wall -O3 -pthread -I ../include -o 3DChess_check2_op legal_moves.c search2.0.c evaluate2.0.c ./board_configs/frontend_check2.c log2.c ps.c

clean:
	rm 3DChess*
