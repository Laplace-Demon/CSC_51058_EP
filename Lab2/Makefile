.PHONY: all clean ex1 ex2 ex3 ex4

all: Lib_light ex1 ex2 ex3 ex4

Lib_light:
	unzip Lib_light.zip; cd Lib_light; make clean; make; \
	      cp inf558.a *.h * ../

ex1:
	@make -sf MakefileEx1

ex2:
	@make -sf MakefileEx2

ex3:
	@make -sf MakefileEx3

ex4:
	@make -sf MakefileEx4

clean:
	@make -sf MakefileEx1 clean
	@make -sf MakefileEx2 clean
	@make -sf MakefileEx3 clean
	@make -sf MakefileEx4 clean
