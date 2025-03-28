CC = g++
FLAGS = -pthread -lrt

main: producer.cpp consumer.cpp
	$(CC) producer.cpp $(FLAGS) -o producer
	$(CC) consumer.cpp $(FLAGS) -o consumer
	./producer & ./consumer &

rm: 
	rm producer consumer