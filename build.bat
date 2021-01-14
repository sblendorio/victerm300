cl65 -t vic20 --config vic20-unexpanded.cfg -Cl -O -o victerm.prg victerm.c
cl65 -t vic20 --config vic20-32k.cfg -Cl -O -o victerm-expanded.prg -DEXP8K victerm.c
cl65 -t vic20 --config vic20-3k.cfg -Cl -O -o victerm-exp3k.prg victerm.c
