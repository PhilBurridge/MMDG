test:
	application tells server to ping all clients. A burst of client responses is sent back to the application. Application measure the time it take for each respons. 

info: 
	server, application and clients are all on the same physical computer.
	Google Chrome browser is used to simulate the clients

computer: 
	Macbook Pro 2 GHz Intel Core i7


rest results are stored in benchmark_localhost1.cvs. The file have the following values defined. RT = Response Time in milliseconds.

<number of clients>, <minimum RT>, <average RT>, <maximum RT>
<number of clients>, <minimum RT>, <average RT>, <maximum RT>
<number of clients>, <minimum RT>, <average RT>, <maximum RT>
...