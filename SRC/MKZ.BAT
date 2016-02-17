copy kc1.zip cut
del kc1.zip
pkzip kc1.zip *.html *.txt *.bat
pkzip kc1.zip sfchar.* *.c  *.h
djp kc.exe
pkzip kc1.zip kc.exe

