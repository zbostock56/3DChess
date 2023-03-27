default:
	gcc -Wall -O3 -pthread -I ../include -L usr/lib/x86_64-linux-gnu/ -o 3DChessOp glad.c legal_moves.c search2.0.c evaluate2.0.c frontend.c log2.c ps.c -l:libglfw.so.3.3 -lGL -lX11 -l:libXrandr.so.2 -l:libXi.so.6 -ldl -lm
	gcc -Wall -g -pthread -I ../include -L usr/lib/x86_64-linux-gnu/ -o 3DChess glad.c legal_moves.c search2.0.c evaluate2.0.c frontend.c log2.c ps.c -l:libglfw.so.3.3 -lGL -lX11 -l:libXrandr.so.2 -l:libXi.so.6 -ldl -lm

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
