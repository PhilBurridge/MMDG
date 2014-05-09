To connect multiple clients to the MMDG server, use the bash script: 
test/help_scripts/open_multiple

EXAMPLE OF A PERFORMANCE AND CONCURRENCY TEST
---------------------------------------------
1. Make sure you have an OSX computer
2. Make sure you have chrome on that OSX computer
3. Start the MMDG server
4. Connect many clients by running the script from Terminal.
	4.1. Run and provide the following parameters as arguments
		sh open_multiple <address> <numberOfConnections> <timeIntervall>
		(example: sh open_multiple http://www.dn.se 5 2.5)
5. From the server or application, ping all clients at once
