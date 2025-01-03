
all:out.8xk

clean:
	$(RM) *.bin *.map *.8xk

obj/main.bin: main.c
	zcc +ti83p main.c -o obj/main.bin -subtype=app_first_page

obj/console.bin : console.c console.h
	zcc +ti83p console.c -o obj/console.bin -subtype=app_sub_page

obj/parser.bin : parser.c parser.h
	zcc +ti83p parser.c -o obj/parser.bin -subtype=app_sub_page


out.8xk: obj/main.bin obj/console.bin obj/parser.bin
	z88dk-appmake +ti83papp --combine-pages --output out.8xk --other-pages obj/main.bin,obj/console.bin,obj/parser.bin

	rm obj/*.map obj/*.bin
