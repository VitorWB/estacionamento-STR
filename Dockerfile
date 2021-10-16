FROM gcc:4.9
COPY . /usr/src/estacionamento
WORKDIR /usr/src/estacionamento
RUN gcc -o estacionamento estacionamento.c -lpthread
CMD ["./estacionamento"]