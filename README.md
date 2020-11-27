**Usage**

	- Parameters:
		1. FD to read from
		2. Pointer to char* where line will be stored in.
	- Allocation is done by GNL for line, so this should be freed by the user.
	- Return values:
		 1: line read.
		 0: End of File read.
		-1: Error (read or malloc)
	- Each call will return the next line of the FD.
	- If FD changes files between calls, behavior is undefined.

**Mandatory part**

	- Line will be malloced inside GNL, and should be freed by the user.
	- Works for single FD only
	- No leaks if you stop calling GNL before EOF

**Bonus part**
	- Works for multiple FDs.
	- Will have unfreed memory that was allocated if GNL calls stop before EOF.
	- Uses linked list to handle multiple FDs, one list element per FD.
