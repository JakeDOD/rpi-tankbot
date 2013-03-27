# compile PWMserver.c
gcc -o PWMserver -l rt PWMserver.c -l bcm2835 -std=c99

# copy the file to the server directory
cp PWMserver /var/server/PWMserver