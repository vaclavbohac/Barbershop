#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "process.h"
#include "barber.h"
#include "shmemory/shared.h"
#include "protocol/request.h"
#include "protocol/response.h"

void process(int handle, struct sockaddr_in* clientAddr)
{
#ifdef DEBUG
	printf("Creating client structure.\n");
#endif
	struct message msg;
	struct client cli;
	cli.handle = handle;

	while (1) {
		if (get_request(&cli, &msg) == -1) {
			fprintf(stderr, "Error while getting request.\n");
			break;
		}
		if (!strcmp(msg.text, "bye")) {
			printf("User requested 'close' connection.\n");
			break;
		}
		if (handle_request(&cli, &msg) == -1) {
			fprintf(stderr, "Error when handling request.\n");
			break;
		}
	}
}

int handle_request(struct client* cli, struct message* request)
{
	char buf[256];
	int length, chair = 0;
	printf("Processing command with code: %d (%s).\n", request->code, request->text);
	switch (request->type) {
		case COMMAND:
			if (!strcmp(request->text, "enter")) {
				struct shared* data = get_shared(getuid());
				struct message response;
				if (data->custommers < MAX_CHAIRS) {
					length = sprintf(buf, "chair %d is empty", data->custommers + 1);
					buf[length] = '\0';
					message_init(&response, INFORMATION, 1, buf);
				}
				else {
					message_init(&response, INFORMATION, 0, "chairnotfree");
				}
				send_response(cli, &response);
			}
		break;
		case ANSWER:
			if (!strcmp(request->text, "chair")) {
				struct message time_req;

				chair = request->code;
				printf("Received chair: %d\n", chair);

				get_request(cli, &time_req);
				printf("%s\n", time_req.text);

				if (!strcmp(time_req.text, "seconds")) {
					struct shared* data = get_shared(getuid());	
					data->times[chair] = time_req.code;
					data->handles[chair] = cli->handle;
				}
				else {
					fprintf(stderr, "Wrong communication.\n");
					return -1;
				}
			}
		break;
		default:
			fprintf(stderr, "Invalid message type.\n");
			return -1;
	}
	return 0;
}
