all: pyrule

queue.o: queue.c queue.h
	gcc queue.c -c -lpthread

compute_thread.o: compute_thread.c compute_thread.h
	gcc compute_thread.c -c `pkg-config --cflags --libs cairo` -lpthread -lm

python_module.o: python_module.c python_module.h python_module_definitions.h
	gcc python_module.c -c -I/usr/include/python3.6m -I/usr/include/python3.6m  -Wno-unused-result -Wsign-compare -g -fdebug-prefix-map=/build/python3.6-mTGOPz/python3.6-3.6.0~b2=. -fstack-protector-strong -Wformat -Werror=format-security  -DNDEBUG -g -fwrapv -O3 -Wall -Wstrict-prototypes `pkg-config --cflags --libs cairo` -lpthread -lm

common.o: common.c common.h
	gcc common.c -c `pkg-config --cflags --libs cairo` -lpthread

column_matrix.o: column_matrix.c column_matrix.h
	gcc column_matrix.c -c -lm

object.o: object.c object.h
	gcc object.c -c 

message_queue.o: message_queue.c message_queue.h
	gcc message_queue.c -c -lpthread

gui.o: gui.c gui.h
	gcc gui.c -c `pkg-config --cflags --libs gtk+-3.0` -lpthread

pyrule: gui.o message_queue.o object.o column_matrix.o common.o python_module.o compute_thread.o queue.o main.c
	gcc main.c gui.o message_queue.o object.o column_matrix.o common.o python_module.o compute_thread.o queue.o -o pyrule `pkg-config --cflags --libs gtk+-3.0` -L/usr/lib/python3.6/config-3.6m-x86_64-linux-gnu -L/usr/lib -lpython3.6m -lpthread -ldl  -lutil -lm  -Xlinker -export-dynamic -Wl,-O1 -Wl,-Bsymbolic-functions -lpthread -lm

clean:
	rm -f pyrule gui.o message_queue.o object.o column_matrix.o common.o python_module.o compute_thread.o queue.o
