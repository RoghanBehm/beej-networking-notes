#include <stdio.h>
#include <poll.h>

// in real use, would initialise the pfds array with desired socket fds
int main(void)
{
	struct pollfd pfds[1]; // More if you want to monitor more

	pfds[0].fd = 0;          // Standard input
	pfds[0].events = POLLIN; // Tell me when ready to read

	// If you needed to monitor other things, as well:
	//pfds[1].fd = some_socket; // Some socket descriptor
	//pfds[1].events = POLLIN;  // Tell me when ready to read

	printf("Hit RETURN or wait 2.5 seconds for timeout\n");

	int num_events = poll(pfds, 1, 2500); // 2.5 second timeout

	if (num_events == 0) {
		printf("Poll timed out!\n");
	} else {
		int pollin_happened = pfds[0].revents & POLLIN;

		if (pollin_happened) {
			printf("File descriptor %d is ready to read\n", pfds[0].fd);
		} else {
			printf("Unexpected event occurred: %d\n", pfds[0].revents);
		}
	}

	return 0;
}
// to add new files descriptors to the set of fds passed to poll, either
// make sure theres enough space in the array for all needed fds, or
//  realloc() more space as needed

// to delete items from the set, copy the last element in the array over-top of
// the one you're deleting, then pass in one fewer as the count to poll()
// another option is to set any fd field to a negative number, which poll() will ignore