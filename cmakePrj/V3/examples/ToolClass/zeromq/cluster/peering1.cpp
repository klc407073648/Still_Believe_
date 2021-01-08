//
//  Broker peering simulation (part 1)
//  Prototypes the state flow
//
#include "zhelpers.hpp"

int main (int argc, char *argv [])
{
    //  First argument is this broker's name
    //  Other arguments are our peers' names
    //
    if (argc < 2) {
        printf ("syntax: peering1 me {you}...\n");
        exit (EXIT_FAILURE);
    }
    char *self = argv [1];
    printf ("I: preparing broker at %s...\n", self);
    srandom ((unsigned) time (NULL));

    zmq::context_t context(1);
    
    //  Bind state backend to endpoint
	std::string temp1=self;
	zmq::socket_t statebe (context, ZMQ_PUB);
    statebe.bind("ipc://"+temp1+"-state.ipc");
    
    //  Connect statefe to all peers
	zmq::socket_t statefe (context, ZMQ_SUB);
    statefe.setsockopt(ZMQ_SUBSCRIBE, "", 0);
	
    int argn;
    for (argn = 2; argn < argc; argn++) {
        char *peer = argv [argn];
		std::string temp2=peer;
        printf ("I: connecting to state backend at '%s'\n", peer);
        statefe.connect( "ipc://"+ temp2 +"-state.ipc");
    }
    //  .split main loop
    //  The main loop sends out status messages to peers, and collects
    //  status messages back from peers. The zmq_poll timeout defines
    //  our own heartbeat:

    while (1) {
        //  Poll for activity, or 1 second timeout
		zmq::message_t message;
        zmq::pollitem_t items []  = { { statefe, 0, ZMQ_POLLIN, 0 } };
        int rc = zmq::poll (&items [0], 1, 1000 * 10);
        if (rc == -1)
            break;              //  Interrupted

        //  Handle incoming status messages
        if (items [0].revents & ZMQ_POLLIN) {
            std::string peer_name = s_recv(statefe);
            std::string available = s_recv(statefe);
            printf ("%s - %s workers free\n", peer_name.c_str(), available.c_str());
        }
        else {
            //  Send random values for worker availability
            s_sendmore(statebe, self);
			std::string strnum = std::to_string(within(10));
            s_send(statebe, strnum);
        }
    }
    return EXIT_SUCCESS;
}
