all: collect2.exe
	@echo "Компиляция завершена!"

collect2.exe: main.o polymorph.o lineal_form.o menu.o
	gcc main.o polymorph.o lineal_form.o menu.o -o collect2.exe

collect1.exe: run_tests.o tests.o polymorph.o lineal_form.o menu.o
	gcc run_tests.o tests.o polymorph.o lineal_form.o menu.o -o collect1.exe 

main.o: main.c polymorph.h lineal_form.h menu.h
	gcc -c main.c -o main.o

polymorph.o: polymorph.c polymorph.h
	gcc -c polymorph.c -o polymorph.o

lineal_form.o: lineal_form.c lineal_form.h
	gcc -c lineal_form.c -o lineal_form.o

menu.o: menu.c menu.h
	gcc -c menu.c -o menu.o

run_tests.o: run_tests.c tests.h
	gcc -c run_tests.c -o run_tests.o

tests.o: tests.c tests.h polymorph.h lineal_form.h
	gcc -c tests.c -o tests.o

run: collect2.exe
	@echo "Запуск программы..."
	./collect2.exe

start: collect2.exe
	@echo "Запуск программы..."
	collect2.exe

test: collect1.exe
	@echo "Запуск тестов..."
	./collect1.exe

clean:
	rm -f collect2.exe collect1.exe *.o
	@echo "Удаление файлов..."