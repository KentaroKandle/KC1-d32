@rem gxx -Wall -march=k6 -ffast-math -O3 kcutil.c 
@rem gxx -Wall -march=k6 -ffast-math -O3 kc.C kcutil.o -lalleg -o kc.exe 
gxx -Wall -march=k6 -ffast-math -O3 kc.C -lalleg -o kc.exe 