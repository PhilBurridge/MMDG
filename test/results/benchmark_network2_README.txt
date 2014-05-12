test:
	application tells server to ping all clients. A burst of client responses is sent back to the application. Application measure the time it take for each respons. 

info: 
	server is run at computer 1.
	application is run at computer 2.
	clients are all on the same computer 3.
	Google Chrome browser is used to simulate the clients

computer 1:
	Asus U36s 2,7 GHz Intel Core i7

computer 2: 
	MacBook Air 1.3 GHz Intel Core i5
	Haswell 

computer 3:  
	Macbook Pro 2 GHz Intel Core i7


test results are stored in benchmark_network1.cvs. The file have the following values defined. RT = Response Time in milliseconds.

<number of clients>, <minimum RT>, <average RT>, <maximum RT>
<number of clients>, <minimum RT>, <average RT>, <maximum RT>
<number of clients>, <minimum RT>, <average RT>, <maximum RT>
...